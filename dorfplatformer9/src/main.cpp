#include "Campaign.h"
#include "InputSystem.h"
#include "SimulationClock.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>

namespace {
namespace fs = std::filesystem;
constexpr int width = 640;
constexpr int height = 360;
constexpr Uint64 renderFramesPerSecond = 30;
constexpr Uint64 renderFrameBudget =
    (SDL_NS_PER_SECOND + renderFramesPerSecond - 1) / renderFramesPerSecond;
using Key = std::pair<std::string, int>;

template<class T, auto destroy>
using Owned = std::unique_ptr<T, decltype(destroy)>;

struct Image {
    Owned<SDL_Texture, SDL_DestroyTexture> texture{nullptr, SDL_DestroyTexture};
    Owned<SDL_Surface, SDL_DestroySurface> colorSource{nullptr, SDL_DestroySurface};
    mutable std::map<int, Owned<SDL_Texture, SDL_DestroyTexture>> colors;
    float x = 0, y = 0, w = 0, h = 0;
};

SDL_Color shiftHue(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, int effect) {
    const float r = red / 255.0f;
    const float g = green / 255.0f;
    const float b = blue / 255.0f;
    const float value = std::max({r, g, b});
    const float chroma = value - std::min({r, g, b});
    float hue = 0;
    if (chroma > 0) {
        hue = value == r ? (g - b) / chroma :
              value == g ? (b - r) / chroma + 2 : (r - g) / chroma + 4;
    }
    hue += effect * (6.0f / 200.0f);
    const auto channel = [&](float offset) {
        const float k = std::fmod(hue + offset + 12, 6.0f);
        return static_cast<Uint8>(std::lround(255 * (value - chroma * std::clamp(
            std::min(k, 4 - k), 0.0f, 1.0f))));
    };
    return {channel(5), channel(3), channel(1), alpha};
}

struct Audio {
    Owned<MIX_Mixer, MIX_DestroyMixer> mixer{nullptr, MIX_DestroyMixer};
    struct MusicTrack {
        Owned<MIX_Track, MIX_DestroyTrack> track{nullptr, MIX_DestroyTrack};
        Key current;
        float gain = -1;
        float frequency = 1;
    };

    MusicTrack dayMusic;
    MusicTrack nightMusic;
    std::map<Key, Owned<MIX_Audio, MIX_DestroyAudio>> clips;

    Audio() {
        if (MIX_Init()) {
            mixer.reset(MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr));
            if (mixer) {
                dayMusic.track.reset(MIX_CreateTrack(mixer.get()));
                nightMusic.track.reset(MIX_CreateTrack(mixer.get()));
            }
        }
    }

    ~Audio() {
        dayMusic.track.reset();
        nightMusic.track.reset();
        clips.clear();
        mixer.reset();
        MIX_Quit();
    }

    MIX_Audio* get(const Key& key) const {
        const auto it = clips.find(key);
        return it == clips.end() ? nullptr : it->second.get();
    }

    void play(const Key& key) const {
        if (auto* clip = get(key)) MIX_PlayAudio(mixer.get(), clip);
    }

    void loop(MusicTrack& music, const Key& key, float gain, float frequency = 1) {
        auto* track = music.track.get();
        if (!track) return;
        if (gain != music.gain) {
            MIX_SetTrackGain(track, gain);
            music.gain = gain;
        }
        if (frequency != music.frequency) {
            MIX_SetTrackFrequencyRatio(track, frequency);
            music.frequency = frequency;
        }
        if (key == music.current) return;
        music.current = key;
        MIX_StopTrack(track, 0);
        auto* clip = get(key);
        if (!clip || !MIX_SetTrackAudio(track, clip)) return;
        const auto options = SDL_CreateProperties();
        if (options) {
            SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
            MIX_PlayTrack(track, options);
            SDL_DestroyProperties(options);
        }
    }
};

bool tileCollides(int type) {
    switch (type) {
    case 1: case 6: case 7: case 15: case 17: case 19: case 24: case 25:
        return false;
    default:
        return true;
    }
}

void importMask(SDL_Surface* source, const Image& image, int type, Campaign& game) {
    if (!tileCollides(type)) return;
    auto& boxes = game.collision.at(type);
    if (type == 16 || type == 18) {
        boxes.push_back({-33, 33, -18, 18});
        return;
    }
    const int columns = static_cast<int>(std::ceil(image.w * 0.5f));
    const int rows = static_cast<int>(std::ceil(image.h * 0.5f));
    const double left = -image.x * 0.5;
    const double top = image.y * 0.5;
    for (int y = 0; y < rows; ++y) {
        int run = -1;
        for (int x = 0; x <= columns; ++x) {
            Uint8 alpha = 0;
            if (x < columns) {
                const int sx = std::min(source->w - 1, (2 * x + 1) * source->w / (2 * columns));
                const int sy = std::min(source->h - 1, (2 * y + 1) * source->h / (2 * rows));
                SDL_ReadSurfacePixel(source, sx, sy, nullptr, nullptr, nullptr, &alpha);
            }
            if (alpha >= 128 && run < 0) run = x;
            if (alpha < 128 && run >= 0) {
                const double bottom = top - y - 1;
                const double right = left + x;
                const auto previous = std::find_if(boxes.rbegin(), boxes.rend(), [&](const Box& box) {
                    return box.left == left + run && box.right == right && box.bottom == bottom + 1;
                });
                if (previous != boxes.rend()) previous->bottom = bottom;
                else boxes.push_back({left + run, right, bottom, bottom + 1});
                run = -1;
            }
        }
    }
}

struct Media {
    SDL_Renderer* renderer;
    std::map<Key, Image> images;
    Audio audio;
    std::size_t soundCount = 0;

    explicit Media(SDL_Renderer* renderer) : renderer(renderer) {}

    void load(const fs::path& root, Campaign& game, bool verifyUiPixels) {
        auto file = openGameData(root / "media.txt");
        std::string kind, group, path;
        int index;
        while (file >> kind >> group >> index >> path) {
            const auto fullPath = (root / path).string();
            const Key key{group, index};
            if (kind == "sound") {
                if (audio.mixer) {
                    auto* clip = MIX_LoadAudio(audio.mixer.get(), fullPath.c_str(), true);
                    if (!clip) throw std::runtime_error("Could not decode " + fullPath + " (" + SDL_GetError() + ")");
                    audio.clips.emplace(key, Owned<MIX_Audio, MIX_DestroyAudio>(clip, MIX_DestroyAudio));
                }
                ++soundCount;
                continue;
            }
            Image image;
            float resolution;
            if (kind != "image" || !(file >> image.x >> image.y >> resolution) || resolution <= 0) {
                throw std::runtime_error("Invalid media entry for " + path);
            }
            Owned<SDL_Surface, SDL_DestroySurface> surface(IMG_Load(fullPath.c_str()), SDL_DestroySurface);
            if (!surface) throw std::runtime_error("Could not load " + fullPath + " (" + SDL_GetError() + ")");
            if (verifyUiPixels && (group == "player" || group == "player_face" ||
                                   group == "hud_day" || group == "hud_digit" || group == "touch_button")) {
                Uint8 alpha = 0;
                for (int y = 0; y < surface->h && alpha == 0; ++y) {
                    for (int x = 0; x < surface->w && alpha == 0; ++x) {
                        if (!SDL_ReadSurfacePixel(surface.get(), x, y, nullptr, nullptr, nullptr, &alpha)) {
                            throw std::runtime_error(SDL_GetError());
                        }
                    }
                }
                if (alpha == 0) throw std::runtime_error("Transparent game artwork: " + fullPath);
            }
            image.x /= resolution;
            image.y /= resolution;
            image.w = surface->w / resolution;
            image.h = surface->h / resolution;
            image.texture.reset(SDL_CreateTextureFromSurface(renderer, surface.get()));
            if (!image.texture) throw std::runtime_error(SDL_GetError());
            if (group == "tile") importMask(surface.get(), image, index, game);
            if (group == "player") image.colorSource = std::move(surface);
            images.emplace(key, std::move(image));
        }
    }

    SDL_Texture* spriteTexture(const Image& image, int color) const {
        if (color == 0 || !image.colorSource) return image.texture.get();
        const auto existing = image.colors.find(color);
        if (existing != image.colors.end()) return existing->second.get();
        Owned<SDL_Surface, SDL_DestroySurface> surface(
            SDL_ConvertSurface(image.colorSource.get(), SDL_PIXELFORMAT_RGBA32), SDL_DestroySurface);
        if (!surface) return image.texture.get();
        for (int y = 0; y < surface->h; ++y) {
            for (int x = 0; x < surface->w; ++x) {
                Uint8 r, g, b, a;
                if (!SDL_ReadSurfacePixel(surface.get(), x, y, &r, &g, &b, &a) || a == 0) continue;
                const auto pixel = shiftHue(r, g, b, a, color);
                SDL_WriteSurfacePixel(surface.get(), x, y, pixel.r, pixel.g, pixel.b, pixel.a);
            }
        }
        Owned<SDL_Texture, SDL_DestroyTexture> texture(SDL_CreateTextureFromSurface(renderer, surface.get()),
                                                       SDL_DestroyTexture);
        if (!texture) return image.texture.get();
        return image.colors.emplace(color, std::move(texture)).first->second.get();
    }

    void sprite(const Key& key, float x, float y, float scale = 1, bool flip = false, int color = 0) const {
        const auto& image = images.at(key);
        const float pivot = flip ? image.w - image.x : image.x;
        const SDL_FRect rect{x - pivot * scale, y - image.y * scale, image.w * scale, image.h * scale};
        SDL_RenderTextureRotated(renderer, spriteTexture(image, color), nullptr, &rect, 0, nullptr,
                                 flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }

    void fitted(const Key& key, SDL_FRect rect) const {
        SDL_RenderTexture(renderer, images.at(key).texture.get(), nullptr, &rect);
    }

    void backdrop(const Key& key, bool widescreen = true) const {
        const float artworkWidth = widescreen ? width : 480;
        const SDL_FRect rect{(width - artworkWidth) / 2, 0, artworkWidth, height};
        SDL_RenderTexture(renderer, images.at(key).texture.get(), nullptr, &rect);
    }
};

void fill(SDL_Renderer* renderer, SDL_FRect box, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &box);
}

void text(SDL_Renderer* renderer, float x, float y, const std::string& value) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, x, y, value.c_str());
}

void centered(SDL_Renderer* renderer, float y, const std::string& value) {
    text(renderer, (width - value.size() * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2.0f, y, value);
}

constexpr SDL_FRect leftButton{14, 262, 84, 84};
constexpr SDL_FRect rightButton{110, 262, 84, 84};
constexpr SDL_FRect jumpButton{width - 98, 262, 84, 84};
constexpr SDL_FRect downButton{width - 194, 262, 84, 84};
constexpr SDL_FRect fireButton{width - 98, 166, 84, 84};
constexpr SDL_FRect pauseButton{width - 56, 12, 44, 44};

struct Touch {
    std::map<SDL_FingerID, SDL_FPoint> points;
    bool visible = false;

    void handle(const SDL_Event& event, SDL_Renderer* renderer) {
        if (event.type == SDL_EVENT_FINGER_UP) {
            points.erase(event.tfinger.fingerID);
        } else if (event.type == SDL_EVENT_FINGER_DOWN || event.type == SDL_EVENT_FINGER_MOTION) {
            int w = 0, h = 0;
            if (auto* window = SDL_GetRenderWindow(renderer)) SDL_GetWindowSize(window, &w, &h);
            SDL_FPoint point{};
            SDL_RenderCoordinatesFromWindow(renderer, event.tfinger.x * w, event.tfinger.y * h,
                                            &point.x, &point.y);
            points[event.tfinger.fingerID] = point;
            visible = true;
        }
    }

    Controls controls() const {
        Controls result;
        bool left = false, right = false;
        for (const auto& [id, p] : points) {
            left |= SDL_PointInRectFloat(&p, &leftButton);
            right |= SDL_PointInRectFloat(&p, &rightButton);
            result.down |= SDL_PointInRectFloat(&p, &downButton);
            result.jump |= SDL_PointInRectFloat(&p, &jumpButton);
            result.fire |= SDL_PointInRectFloat(&p, &fireButton);
        }
        result.move = float(right) - float(left);
        return result;
    }
};

void draw(const Media& media, const Campaign& game, int selected, bool touch, const std::string& status) {
    auto* renderer = media.renderer;
    SDL_SetRenderDrawColor(renderer, 40, 81, 135, 255);
    SDL_RenderClear(renderer);
    if (game.scene == Scene::Title) {
        media.backdrop({"sky", 1});
        media.backdrop({"menu", 1}, false);
        fill(renderer, {(width - 416) / 2.0f, 261, 416, 83}, {12, 20, 40, 220});
        centered(renderer, 273, "Level " + std::to_string(selected + 1) + " / 6");
        centered(renderer, 291, "Arrows select  |  Enter / A / Tap to play");
        centered(renderer, 309, "F5 copy save  |  F9 import from clipboard");
        if (!status.empty()) centered(renderer, 326, status);
        SDL_RenderPresent(renderer);
        return;
    }

    const float xMid = width / 2.0f - static_cast<float>(game.camera);
    const float yMid = height / 2.0f + game.upper;
    const auto sky = std::clamp(game.theme(), 1, 6);
    media.backdrop({"sky", sky});
    const auto& objects = game.level().objects;
    for (std::size_t i = 0; i < objects.size(); ++i) {
        const auto& object = objects[i];
        if (std::abs(object.position.x - game.camera) > width / 2 + 90 ||
            std::abs(object.position.y - game.upper) > 450) continue;
        const auto x = xMid + static_cast<float>(object.position.x);
        const auto y = yMid - static_cast<float>(object.position.y);
        switch (object.type) {
        case 1: case 15: case 19: case 24: case 25: break;
        case 6:
            if (!game.collected[i]) media.sprite({"tile", 6}, x, y, 0.5f);
            break;
        default:
            media.sprite({"tile", object.type}, x, y - (object.type == 16 ? 3 : 0), 0.5f);
            break;
        }
    }
    const Point exit = game.exitPosition();
    if ((exit.y > 400) == (game.upper != 0) && std::abs(exit.x - game.camera) < width / 2 + 40) {
        const auto x = xMid + static_cast<float>(exit.x);
        const auto y = yMid - static_cast<float>(exit.y);
        media.sprite({"flag", 1}, x, y);
        media.sprite({"flag_icon", 1}, x, y);
    }
    for (const auto& actor : game.actors) {
        const bool defeated = actor.health <= 0;
        if (game.theme() == 4 || std::abs(actor.position.x - game.camera) > width / 2 + 80 ||
            (defeated && (actor.type != 15 || actor.cooldown <= 0))) continue;
        const auto x = xMid + static_cast<float>(actor.position.x);
        const auto y = yMid - static_cast<float>(actor.position.y) - 13;
        const auto frame = static_cast<int>(game.elapsed * 10);
        if (actor.type == 25) media.sprite({"boss_guard", 1}, x, y + 13, 0.6f, actor.direction < 0);
        else if (actor.type == 24) media.sprite({"guard", frame % 3 + 1}, x, y, 0.4f, actor.direction < 0);
        else {
            // Ball_1 is the live costume. Ball_3 through Ball_7 are the defeat sequence.
            const int costume = defeated ? 6 + std::clamp(static_cast<int>((0.5 - actor.cooldown) * 10), 0, 4) : 2;
            media.sprite({"enemy", costume}, x, y, 0.6f);
        }
    }
    if (game.dragonActive() && game.dragonTransition == 0) {
        const bool fallingPhase = game.dragonPhase == 3;
        media.sprite({"dragon", fallingPhase ? 4 : 1},
                     xMid + static_cast<float>(game.dragon.x), yMid - static_cast<float>(game.dragon.y),
                     fallingPhase ? 0.7f : 1.0f, game.dragonDirection > 0);
    }
    for (const auto& meteor : game.meteors) {
        media.sprite({"dragon", 4}, xMid + static_cast<float>(meteor.x),
                     yMid - static_cast<float>(meteor.y), 0.4f);
    }
    for (const auto& ball : game.fireballs) {
        if (ball.ticksLeft <= 0) continue;
        media.sprite({"fireball", 1}, xMid + static_cast<float>(ball.position.x),
                     yMid - static_cast<float>(ball.position.y), 0.6f, ball.velocity.x < 0);
    }
    const auto playerX = xMid + static_cast<float>(game.player.x);
    const auto playerY = yMid - static_cast<float>(game.player.y);
    const bool small = game.power == Power::Small;
    const auto playerScale = small ? 0.25f : 0.5f;
    if (game.scene == Scene::Pipe || game.scene == Scene::Death) {
        const auto variant = static_cast<int>(game.power) + 1;
        media.sprite({"transition", game.scene == Scene::Pipe ? variant + 3 : variant},
                     playerX, playerY - (game.scene == Scene::Pipe ? 20.0f - 32.0f * static_cast<float>(game.elapsed) : 15),
                     0.5f, game.facing < 0);
    } else if (game.special() || game.invincible == 0 || static_cast<int>(game.invincible * 30) % 2 == 0) {
        const bool swimming = game.theme() == 4;
        const float pivotLift = 11.875f * (playerScale / 0.25f) - 1.0f -
                                (game.crouched ? 7.0f : 0.0f);
        const int walk = 1 + static_cast<int>(game.walkFrame);
        const int stroke = static_cast<int>(game.elapsed * 6) % 2;
        const int bodyFrame = swimming ? 9 + stroke : game.crouched ? 8 :
                              !game.grounded ? 7 : walk;
        const int faceFrame = swimming ? 10 + stroke : game.crouched ? 9 :
                              !game.grounded ? 8 : walk + 1;
        const bool flip = game.facing < 0;
        media.sprite({"player_face", faceFrame}, playerX, playerY - pivotLift, playerScale, flip);
        const int invincibleColor = (1 + static_cast<int>(game.elapsed * 60)) % 40 * 5;
        const int color = game.special() ? invincibleColor : game.power == Power::Full ? -54 : 0;
        media.sprite({"player", bodyFrame}, playerX, playerY - pivotLift, playerScale, flip, color);
    }
    if (game.darkness() > 0) {
        fill(renderer, {0, 0, width, height}, {8, 12, 47, static_cast<Uint8>(game.darkness() * 146)});
    }
    const double remaining = game.finalNightSeconds();
    if (remaining >= 0) {
        const auto display = std::to_string(remaining);
        for (std::size_t i = 0; i < std::min<std::size_t>(display.size(), 5); ++i) {
            const char digit = display[i];
            const int costume = digit == '.' ? 11 : digit == '0' ? 10 : digit - '0';
            media.sprite({"hud_digit", costume}, 240 + 20 * static_cast<float>(i), 180);
        }
    } else {
        const float clockX = 240 + 80 * game.clockProgress() + (game.night() ? 100 : 0);
        media.sprite({"hud_day", game.day()}, clockX, 180);
        media.sprite({"hud_day", 5}, clockX, 180);
    }
    if (game.dragonActive()) {
        fill(renderer, {width / 2.0f - 88, 31, 176, 6}, {20, 10, 30, 220});
        const auto limit = game.dragonPhase == 3 ? 5 : 3;
        fill(renderer, {width / 2.0f - 88, 31, static_cast<float>(176 * std::clamp(game.dragonHealth / limit, 0.0, 1.0)), 6},
             {215, 61, 70, 255});
    }
    if (game.paused || game.scene == Scene::Intro || game.scene == Scene::Clear ||
        game.scene == Scene::Ending || game.scene == Scene::Expired) {
        fill(renderer, {0, 0, width, height}, {10, 15, 31, 205});
        if (game.paused) {
            centered(renderer, 150, "Paused");
            centered(renderer, 174, touch ? "Tap pause to resume  |  M for title" :
                                           "Esc / Start to resume  |  M for title");
            if (touch) {
                fill(renderer, pauseButton, {10, 15, 31, 160});
                text(renderer, pauseButton.x + 18, pauseButton.y + 18, ">");
            }
        } else if (game.scene == Scene::Intro) {
            media.backdrop({"ending", 1}, false);
        } else if (game.scene == Scene::Clear) {
            centered(renderer, 168, "Level clear");
        } else {
            if (game.scene == Scene::Ending) media.sprite({"ending", 2}, width / 2.0f, 94, 0.28f);
            centered(renderer, 150, game.scene == Scene::Ending ? "Campaign complete" : "The world ran out of time");
            centered(renderer, 174, "Enter / A / Tap to return to title");
        }
    } else if (touch) {
        media.fitted({"touch_button", 1}, leftButton);
        media.fitted({"touch_button", 2}, rightButton);
        media.fitted({"touch_button", 3}, jumpButton);
        media.fitted({"touch_button", 4}, downButton);
        if (game.power == Power::Full) media.fitted({"touch_button", 5}, fireButton);
        fill(renderer, pauseButton, {10, 15, 31, 160});
        fill(renderer, {pauseButton.x + 15, pauseButton.y + 12, 5, 20}, {255, 255, 255, 255});
        fill(renderer, {pauseButton.x + 24, pauseButton.y + 12, 5, 20}, {255, 255, 255, 255});
    }
    SDL_RenderPresent(renderer);
}

void updateAudio(Audio& audio, Campaign& game) {
    Key dayTrack, nightTrack;
    float dayGain = 0, nightGain = 0, pitch = 1;
    constexpr float musicVolume = 0.25f;
    if (!game.paused && (game.scene == Scene::Title || game.scene == Scene::Intro)) {
        dayTrack = {"title_sound", 3};
        dayGain = musicVolume;
    } else if (!game.paused && game.scene == Scene::Play) {
        if (game.special()) {
            dayTrack = {"day", 7};
            dayGain = musicVolume;
        } else {
            if (game.day() == 3) pitch = std::exp2(5.0f / 120);
            if (game.dragonActive()) {
                nightTrack = {"night", 6};
                nightGain = musicVolume;
            } else {
                const int headerTrack = game.theme();
                dayTrack = {"day", headerTrack};
                nightTrack = {"night", headerTrack};
                dayGain = game.night() ? 0 : musicVolume;
                nightGain = game.night() ? musicVolume : 0;
            }
        }
    }
    audio.loop(audio.dayMusic, dayTrack, dayGain, pitch);
    audio.loop(audio.nightMusic, nightTrack, nightGain, pitch);
    for (const auto sound : game.sounds) {
        switch (sound) {
        case Sound::Jump: audio.play({"player_sound", game.power == Power::Small ? 2 : 3}); break;
        case Sound::PowerUp: audio.play({"power_sound", 2}); break;
        case Sound::Fireball: audio.play({"fireball_sound", 1}); break;
        case Sound::FireballHit: audio.play({"dragon_sound", 2}); break;
        case Sound::Stomp: audio.play({"player_sound", 4}); break;
        case Sound::Pipe: audio.play({"pipe_sound", 2}); break;
        case Sound::Death: audio.play({"player_sound", 1}); break;
        case Sound::Clear: audio.play({"goal_sound", 4}); break;
        case Sound::Dragon: audio.play({"dragon_sound", 1}); break;
        case Sound::DragonDefeated: audio.play({"dragon_sound", 2}); break;
        }
    }
    game.sounds.clear();
}

Controls controls(const InputSystem& pads, const Touch& touch) {
    const auto* keys = SDL_GetKeyboardState(nullptr);
    const auto fingers = touch.controls();
    const float move = float(keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) -
                       float(keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]);
    const float padMove = pads.gameplayMoveX();
    return {padMove != 0 ? padMove : std::clamp(move + fingers.move, -1.0f, 1.0f),
            keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W] ||
              pads.gameplayJumpHeld() || fingers.jump,
            keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] || pads.gameplayDownHeld() || fingers.down,
            keys[SDL_SCANCODE_X] || keys[SDL_SCANCODE_SPACE] || pads.gameplayFireHeld() || fingers.fire};
}

int run(const Levels& levels, const fs::path& root, bool smoke) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) || !SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        if (!SDL_WasInit(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError());
        SDL_Log("Audio initialization failed: %s", SDL_GetError());
    }
    Campaign game(levels);
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    if (smoke) flags |= SDL_WINDOW_HIDDEN;
    const bool created = SDL_CreateWindowAndRenderer("Dorfplatformer 9 Native Edition", width * 2, height * 2, flags, &window, &renderer);
    Owned<SDL_Window, SDL_DestroyWindow> ownedWindow(window, SDL_DestroyWindow);
    Owned<SDL_Renderer, SDL_DestroyRenderer> ownedRenderer(renderer, SDL_DestroyRenderer);
    if (!created || !SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
        throw std::runtime_error(SDL_GetError());
    }
    SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_DISABLED);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    Media media(renderer);
    media.load(root, game, smoke);

    if (smoke) {
        const auto fireColor = shiftHue(0, 255, 0, 128, -54);
        if (fireColor.r != 255 || fireColor.g >= 110 || fireColor.b != 0 || fireColor.a != 128) return 1;
        if (!media.audio.mixer || media.audio.clips.size() != media.soundCount) return 1;
        draw(media, game, 0, false, {});
        game.start();
        for (int i = 0; i < 120; ++i) game.tick({});
        if (!game.grounded || game.scene != Scene::Play || game.player.y < -80) return 1;
        const auto x = game.player.x;
        for (int i = 0; i < 8; ++i) game.tick({1, true, false});
        if (game.player.x <= x || game.grounded) return 1;
        draw(media, game, 0, true, {});
        for (int i = 1; i < 6; ++i) {
            game.start(i);
            for (int j = 0; j < 90; ++j) game.tick({});
            draw(media, game, i, false, {});
            updateAudio(media.audio, game);
        }
        // Level 2's surface and upper world use different header selectors.
        game.start(1);
        game.scene = Scene::Play;
        updateAudio(media.audio, game);
        if (media.audio.dayMusic.current != Key{"day", 1} ||
            media.audio.nightMusic.current != Key{"night", 1} ||
            media.audio.dayMusic.gain <= 0 || media.audio.nightMusic.gain != 0) return 1;
        game.upper = 1100;
        game.worldTime = 68;
        updateAudio(media.audio, game);
        if (media.audio.dayMusic.current != Key{"day", 2} ||
            media.audio.nightMusic.current != Key{"night", 2} ||
            media.audio.dayMusic.gain != 0 || media.audio.nightMusic.gain <= 0) return 1;
        game.dragonPhase = 1;
        updateAudio(media.audio, game);
        if (media.audio.nightMusic.current != Key{"night", 6}) return 1;
        game.worldTime = 320;
        if (game.finalNightSeconds() <= 0) return 1;
        draw(media, game, 0, true, {});
        Touch touchControls;
        touchControls.points.emplace(1, SDL_FPoint{leftButton.x + 4, leftButton.y + 4});
        touchControls.points.emplace(2, SDL_FPoint{jumpButton.x + 4, jumpButton.y + 4});
        touchControls.points.emplace(3, SDL_FPoint{fireButton.x + 4, fireButton.y + 4});
        const auto pressed = touchControls.controls();
        if (pressed.move != -1 || !pressed.jump || !pressed.fire || pressed.down) return 1;
        return 0;
    }

    fs::path savePath;
    if (auto* pref = SDL_GetPrefPath("Benno111", "Dorfplatformer9")) {
        savePath = fs::path(pref) / "progress.txt";
        SDL_free(pref);
        std::ifstream save(savePath);
        std::string code;
        if (std::getline(save, code)) game.importSave(code);
    }
    auto saved = game.exportSave();
    int selected = std::min(game.unlocked, 5);
    std::string status;
    InputSystem pads;
    pads.scanConnected();
    Touch touch;
    bool running = true;
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
    auto previous = SDL_GetTicksNS();
    SimulationClock simulation;
    while (running) {
        const auto frameStart = SDL_GetTicksNS();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            pads.handleEvent(event);
            touch.handle(event, renderer);
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (event.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
                touch.points.clear();
                if (game.scene == Scene::Play) game.paused = true;
            }
            const bool keyDown = event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat;
            const auto key = keyDown ? event.key.scancode : SDL_SCANCODE_UNKNOWN;
            const bool accept = key == SDL_SCANCODE_RETURN || key == SDL_SCANCODE_SPACE ||
                                event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                                event.type == SDL_EVENT_FINGER_DOWN || InputSystem::isAcceptEvent(event);
            const bool padDown = event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN;
            if (game.scene == Scene::Title) {
                if (key == SDL_SCANCODE_LEFT || (padDown && event.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_LEFT)) {
                    selected = std::max(0, selected - 1);
                }
                if (key == SDL_SCANCODE_RIGHT || (padDown && event.gbutton.button == SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) {
                    selected = std::min(std::min(game.unlocked, 5), selected + 1);
                }
                if (key == SDL_SCANCODE_F5) {
                    status = SDL_SetClipboardText(game.exportSave().c_str()) ? "Save code copied" : "Clipboard unavailable";
                }
                if (key == SDL_SCANCODE_F9) {
                    auto* clipboard = SDL_GetClipboardText();
                    const bool ok = clipboard && game.importSave(clipboard);
                    SDL_free(clipboard);
                    status = ok ? "Save imported" : "Invalid save code";
                    if (ok) selected = std::min(game.unlocked, 5);
                }
                if (accept) {
                    status.clear();
                    game.start(selected);
                }
            } else if (game.scene == Scene::Ending || game.scene == Scene::Expired) {
                if (accept) {
                    game.scene = Scene::Title;
                    selected = std::min(game.unlocked, 5);
                }
            } else if (key == SDL_SCANCODE_ESCAPE || InputSystem::isPauseToggleEvent(event) ||
                       (event.type == SDL_EVENT_FINGER_DOWN &&
                        SDL_PointInRectFloat(&touch.points.at(event.tfinger.fingerID), &pauseButton))) {
                game.paused = !game.paused;
            } else if (game.paused && key == SDL_SCANCODE_M) {
                game.scene = Scene::Title;
                game.paused = false;
                selected = std::min(game.unlocked, 5);
            }
        }
        const auto now = SDL_GetTicksNS();
        const auto ticks = simulation.advance(now - previous);
        previous = now;
        const auto held = controls(pads, touch);
        for (unsigned tick = 0; tick < ticks; ++tick) {
            const auto before = game.scene;
            game.tick(held);
            if (before == Scene::Ending && game.scene == Scene::Title) selected = std::min(game.unlocked, 5);
        }
        updateAudio(media.audio, game);
        const auto code = game.exportSave();
        if (code != saved && !savePath.empty()) {
            const auto temporary = savePath.string() + ".tmp";
            std::ofstream file(temporary);
            file << code << '\n';
            file.close();
            std::error_code error;
            if (file) fs::rename(temporary, savePath, error);
            if (file && !error) saved = code;
        }
        draw(media, game, selected, touch.visible, status);
        const auto spent = SDL_GetTicksNS() - frameStart;
        if (spent < renderFrameBudget) SDL_DelayNS(renderFrameBudget - spent);
    }
    return 0;
}
}

int main(int argc, char** argv) {
    try {
        const bool check = argc > 1 && std::string_view(argv[1]) == "--check";
        const bool smoke = argc > 1 && std::string_view(argv[1]) == "--smoke";
        const int pathArg = check || smoke ? 2 : 1;
#if defined(SDL_PLATFORM_ANDROID)
        // Android's asset manager resolves paths relative to the APK asset directory.
        const fs::path defaultRoot = "assets";
#else
        const char* base = SDL_GetBasePath();
        const fs::path defaultRoot = (base ? fs::path(base) : fs::current_path()) / "assets";
#endif
        const fs::path root = argc > pathArg ? fs::path(argv[pathArg]) : defaultRoot;
        const auto levels = loadLevels(root / "levels.txt");
        if (check) return checkCampaign(levels) ? 0 : 1;
        const auto result = run(levels, root, smoke);
        SDL_Quit();
        return result;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        SDL_Quit();
        return 1;
    }
}
