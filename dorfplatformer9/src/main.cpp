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
constexpr int width = 480;
constexpr int height = 360;
constexpr Uint64 renderFramesPerSecond = 30;
constexpr Uint64 renderFrameBudget =
    (SDL_NS_PER_SECOND + renderFramesPerSecond - 1) / renderFramesPerSecond;
using Key = std::pair<std::string, int>;

template<class T, auto destroy>
using Owned = std::unique_ptr<T, decltype(destroy)>;

struct Image {
    Owned<SDL_Texture, SDL_DestroyTexture> texture{nullptr, SDL_DestroyTexture};
    float x = 0, y = 0, w = 0, h = 0;
};

struct Audio {
    Owned<MIX_Mixer, MIX_DestroyMixer> mixer{nullptr, MIX_DestroyMixer};
    Owned<MIX_Track, MIX_DestroyTrack> music{nullptr, MIX_DestroyTrack};
    std::map<Key, Owned<MIX_Audio, MIX_DestroyAudio>> clips;
    Key current;

    Audio() {
        if (MIX_Init()) {
            mixer.reset(MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr));
            if (mixer) {
                music.reset(MIX_CreateTrack(mixer.get()));
                if (music) MIX_SetTrackGain(music.get(), 0.25f);
            }
        }
    }

    ~Audio() {
        music.reset();
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

    void loop(const Key& key) {
        if (!music || key == current) return;
        current = key;
        MIX_StopTrack(music.get(), 0);
        auto* clip = get(key);
        if (!clip || !MIX_SetTrackAudio(music.get(), clip)) return;
        const auto options = SDL_CreateProperties();
        if (options) {
            SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
            MIX_PlayTrack(music.get(), options);
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

    void load(const fs::path& root, Campaign& game, bool verifyPlayerPixels) {
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
            if (verifyPlayerPixels && (group == "player" || group == "player_face")) {
                Uint8 alpha = 0;
                for (int y = 0; y < surface->h && alpha == 0; ++y) {
                    for (int x = 0; x < surface->w && alpha == 0; ++x) {
                        if (!SDL_ReadSurfacePixel(surface.get(), x, y, nullptr, nullptr, nullptr, &alpha)) {
                            throw std::runtime_error(SDL_GetError());
                        }
                    }
                }
                if (alpha == 0) throw std::runtime_error("Transparent player costume: " + fullPath);
            }
            image.x /= resolution;
            image.y /= resolution;
            image.w = surface->w / resolution;
            image.h = surface->h / resolution;
            image.texture.reset(SDL_CreateTextureFromSurface(renderer, surface.get()));
            if (!image.texture) throw std::runtime_error(SDL_GetError());
            if (group == "tile") importMask(surface.get(), image, index, game);
            images.emplace(key, std::move(image));
        }
    }

    void sprite(const Key& key, float x, float y, float scale = 1, bool flip = false) const {
        const auto& image = images.at(key);
        const float pivot = flip ? image.w - image.x : image.x;
        const SDL_FRect rect{x - pivot * scale, y - image.y * scale, image.w * scale, image.h * scale};
        SDL_RenderTextureRotated(renderer, image.texture.get(), nullptr, &rect, 0, nullptr,
                                 flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }

    void backdrop(const Key& key) const {
        const SDL_FRect rect{0, 0, width, height};
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
            if (p.y < 288 || p.y > height) continue;
            left |= p.x >= 0 && p.x < 64;
            right |= p.x >= 64 && p.x < 126;
            result.down |= p.x > 350 && p.x <= 414;
            result.jump |= p.x > 414 && p.x <= width;
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
        media.backdrop({"menu", 1});
        fill(renderer, {32, 261, 416, 83}, {12, 20, 40, 220});
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
        if (std::abs(object.position.x - game.camera) > 330 ||
            std::abs(object.position.y - game.upper) > 450) continue;
        const auto x = xMid + static_cast<float>(object.position.x);
        const auto y = yMid - static_cast<float>(object.position.y);
        switch (object.type) {
        case 1: case 15: case 24: case 25: break;
        case 19:
            media.sprite({"flag", 1}, x, y, 0.6f);
            break;
        case 6:
            if (!game.collected[i]) media.sprite({"tile", 6}, x, y, 0.5f);
            break;
        default:
            media.sprite({"tile", object.type}, x, y - (object.type == 16 ? 3 : 0), 0.5f);
            break;
        }
    }
    for (const auto& actor : game.actors) {
        const bool defeated = actor.health <= 0;
        if (game.theme() == 4 || std::abs(actor.position.x - game.camera) > 320 ||
            (defeated && (actor.type == 24 || actor.cooldown <= 0))) continue;
        const auto x = xMid + static_cast<float>(actor.position.x);
        const auto y = yMid - static_cast<float>(actor.position.y) - 13;
        const auto frame = static_cast<int>(game.elapsed * 10);
        if (actor.type == 24) media.sprite({"guard", frame % 3 + 1}, x, y, 0.4f, actor.direction < 0);
        else {
            // Ball_1 is the live costume. Ball_3 through Ball_7 are the defeat sequence.
            const int costume = defeated ? 6 + std::clamp(static_cast<int>((0.5 - actor.cooldown) * 10), 0, 4) : 2;
            media.sprite({"enemy", costume}, x, y, 0.6f);
        }
    }
    if (game.dragonActive()) {
        media.sprite({"dragon", 1 + static_cast<int>(game.dragonTime * 7) % 3},
                     xMid + static_cast<float>(game.dragon.x), yMid - static_cast<float>(game.dragon.y),
                     0.4f + 0.08f * game.dragonPhase, game.player.x > game.dragon.x);
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
    } else if (game.invincible == 0 || static_cast<int>(game.invincible * 30) % 2 == 0) {
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
        media.sprite({"player", bodyFrame}, playerX, playerY - pivotLift, playerScale, flip);
    }
    if (game.darkness() > 0) {
        fill(renderer, {0, 0, width, height}, {8, 12, 47, static_cast<Uint8>(game.darkness() * 146)});
    }
    fill(renderer, {0, 0, width, 25}, {10, 15, 31, 216});
    text(renderer, 9, 8, game.level().name);
    text(renderer, 57, 8, "Day " + std::to_string(game.day()) + "/3");
    text(renderer, 155, 8, game.night() ? "Night" : "Daylight");
    text(renderer, 388, 8, game.special ? "Powered" : game.power == Power::Full ? "Full" : small ? "Small" : "Big");
    if (game.dragonActive()) {
        fill(renderer, {152, 31, 176, 6}, {20, 10, 30, 220});
        const auto limit = game.dragonPhase == 3 ? 5 : 3;
        fill(renderer, {152, 31, static_cast<float>(176 * std::clamp(game.dragonHealth / limit, 0.0, 1.0)), 6},
             {215, 61, 70, 255});
    }
    if (game.paused || game.scene == Scene::Intro || game.scene == Scene::Clear ||
        game.scene == Scene::Ending || game.scene == Scene::Expired) {
        fill(renderer, {0, 0, width, height}, {10, 15, 31, 205});
        if (game.paused) {
            centered(renderer, 150, "Paused");
            centered(renderer, 174, "Esc / Start to resume  |  M for title");
        } else if (game.scene == Scene::Intro) {
            media.backdrop({"ending", 1});
        } else if (game.scene == Scene::Clear) {
            centered(renderer, 168, "Level clear");
        } else {
            if (game.scene == Scene::Ending) media.sprite({"ending", 2}, 240, 94, 0.28f);
            centered(renderer, 150, game.scene == Scene::Ending ? "Campaign complete" : "The world ran out of time");
            centered(renderer, 174, "Enter / A / Tap to return to title");
        }
    } else if (touch) {
        fill(renderer, {6, 300, 50, 51}, {10, 15, 31, 160});
        fill(renderer, {65, 300, 50, 51}, {10, 15, 31, 160});
        fill(renderer, {356, 300, 50, 51}, {10, 15, 31, 160});
        fill(renderer, {417, 300, 55, 51}, {10, 15, 31, 160});
        text(renderer, 26, 321, "<");
        text(renderer, 85, 321, ">");
        text(renderer, 367, 321, "DOWN");
        text(renderer, 428, 321, "JUMP");
    }
    SDL_RenderPresent(renderer);
}

void updateAudio(Audio& audio, Campaign& game) {
    const bool terminal = game.scene == Scene::Ending || game.scene == Scene::Expired;
    if (game.paused || terminal || game.scene == Scene::Clear || game.scene == Scene::Death) audio.loop({});
    else if (game.scene == Scene::Title || game.scene == Scene::Intro) audio.loop({"title_sound", 3});
    else if (game.dragonActive()) audio.loop({"day", 6});
    else audio.loop({game.night() ? "night" : "day", game.levelIndex + 1});
    for (const auto sound : game.sounds) {
        switch (sound) {
        case Sound::Jump: audio.play({"player_sound", game.power == Power::Small ? 2 : 3}); break;
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
            keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_SPACE] ||
              pads.gameplayJumpHeld() || fingers.jump,
            keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] || pads.gameplayDownHeld() || fingers.down};
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
    const bool created = SDL_CreateWindowAndRenderer("Dorfplatformer 9 Native Edition", 960, 720, flags, &window, &renderer);
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
                       (event.type == SDL_EVENT_FINGER_DOWN && event.tfinger.y < 0.10f &&
                        event.tfinger.x > 0.79f)) {
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
