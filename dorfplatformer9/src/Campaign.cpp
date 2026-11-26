#include "Campaign.h"
#include "SimulationClock.h"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <SDL3/SDL.h>

#include <memory>
#include <stdexcept>

namespace {
constexpr double tickSeconds = SimulationClock::tickSeconds;
constexpr double clockStep = 0.0513;
constexpr double daylightLength = 1320 * clockStep;
constexpr double sunset = 100 * clockStep;
constexpr double nightLength = 840 * clockStep;
constexpr double dayLength = daylightLength + sunset + nightLength;
constexpr double dawnLength = 100 * clockStep;
constexpr double campaignLength = 3 * dayLength + 2 * dawnLength;

bool near(Point a, Point b, double width, double height) {
    return std::abs(a.x - b.x) < width && std::abs(a.y - b.y) < height;
}

bool overlaps(const Box& a, const Box& b, Point offset) {
    return a.right > b.left + offset.x && a.left < b.right + offset.x &&
           a.top > b.bottom + offset.y && a.bottom < b.top + offset.y;
}

double playerHeight(Power power, bool crouched) {
    return power == Power::Small ? 22 : (crouched ? 25 : 43);
}

int readInt(std::string_view value, int fallback) {
    int result = 0;
    const auto end = value.data() + value.size();
    auto parsed = std::from_chars(value.data(), end, result);
    return parsed.ec == std::errc{} && parsed.ptr == end ? result : fallback;
}
}

std::istringstream openGameData(const std::filesystem::path& path) {
    std::size_t length = 0;
    const std::unique_ptr<char, decltype(&SDL_free)> content(
        static_cast<char*>(SDL_LoadFile(path.string().c_str(), &length)), SDL_free);
    if (!content) throw std::runtime_error(path.string() + ": " + SDL_GetError());
    return std::istringstream(std::string(content.get(), length));
}

Levels loadLevels(const std::filesystem::path& path) {
    auto input = openGameData(path);
    Levels levels;
    for (std::size_t i = 0; i < levels.size(); ++i) {
        auto& level = levels[i];
        std::size_t count = 0;
        if (!(input >> level.name >> level.theme >> level.upperTheme >> count) ||
            level.name != "1-" + std::to_string(i + 1) || count == 0 || count > 100000) {
            throw std::runtime_error("Invalid campaign level header in " + path.string());
        }
        level.objects.resize(count);
        for (auto& object : level.objects) {
            if (!(input >> object.position.x >> object.position.y >> object.type) ||
                !std::isfinite(object.position.x) || !std::isfinite(object.position.y) ||
                object.type < 1 || object.type > 30) {
                throw std::runtime_error("Invalid object in level " + level.name);
            }
        }
        for (std::size_t j = 0; j < count; ++j) {
            if (level.objects[j].type == 16 && (++j == count || level.objects[j].type != 18)) {
                throw std::runtime_error("Missing pipe destination in level " + level.name);
            }
        }
    }
    std::string extra;
    if (input >> extra) throw std::runtime_error("Unexpected data after the six campaign levels");
    return levels;
}

int Campaign::day() const {
    if (worldTime < dayLength) return 1;
    if (worldTime < 2 * dayLength + dawnLength) return 2;
    return 3;
}

int Campaign::theme() const {
    return upper == 0 ? level().theme : level().upperTheme;
}

float Campaign::darkness() const {
    const double start = (day() - 1) * (dayLength + dawnLength);
    const double localTime = worldTime - start;
    if (day() > 1 && localTime < 0) {
        return static_cast<float>(std::clamp(-localTime / dawnLength, 0.0, 1.0));
    }
    return static_cast<float>(std::clamp((localTime - daylightLength) / sunset, 0.0, 1.0));
}

bool Campaign::night() const {
    return darkness() >= 0.5f;
}

void Campaign::changeScene(Scene next) {
    scene = next;
    elapsed = 0;
}

void Campaign::start(int selected) {
    levelIndex = std::clamp(selected, 0, 5);
    worldTime = -6;
    paused = false;
    downWasHeld = false;
    jumpWasHeld = false;
    sounds.clear();
    spawn();
    changeScene(Scene::Intro);
}

void Campaign::resetActors() {
    actors.clear();
    for (const auto& object : level().objects) {
        if ((object.type == 15 || object.type == 24) &&
            (object.position.y > 400) == (upper != 0)) {
            actors.push_back({object.position, object.type, -1, 0, 0, object.type == 24 ? 3 : 1});
        }
    }
}

void Campaign::spawn() {
    upper = 0;
    player = {-100, 100};
    velocity = {};
    camera = 0;
    facing = 1;
    grounded = false;
    crouched = false;
    invincible = 0;
    walkFrame = 0;
    jumpTicks = 0;
    dragonPhase = 0;
    dragonHealth = 0;
    dragonGrace = 0;
    dragonTime = 0;
    collected.assign(level().objects.size(), false);
    resetActors();
}

bool Campaign::blocked(const Box& bounds) const {
    for (const auto& object : level().objects) {
        if (std::abs(object.position.x - (bounds.left + bounds.right) / 2) > 80 ||
            std::abs(object.position.y - bounds.bottom) > 100) continue;
        for (const auto& shape : collision[object.type]) {
            if (overlaps(bounds, shape, object.position)) return true;
        }
    }
    return false;
}

void Campaign::move(Point& point, double& speed, bool horizontal, double halfWidth, double height) {
    // Scratch rounds half ties toward positive infinity. Movement still uses the sign of velocity.
    const double distance = horizontal ? std::abs(std::floor(speed + 0.5)) : std::floor(std::abs(speed) + 0.5);
    const double direction = speed < 0 ? -1 : 1;
    for (int i = 0; i < distance; ++i) {
        Point next = point;
        (horizontal ? next.x : next.y) += direction;
        if (blocked({next.x - halfWidth, next.x + halfWidth, next.y, next.y + height})) {
            if (&point == &player && !horizontal && speed < 0) grounded = true;
            speed = 0;
            return;
        }
        point = next;
    }
}

void Campaign::damage() {
    if (invincible > 0 || special) return;
    if (power == Power::Small) {
        die();
        return;
    }
    power = static_cast<Power>(static_cast<int>(power) - 1);
    invincible = 1.34;
}

void Campaign::die() {
    sounds.push_back(Sound::Death);
    changeScene(Scene::Death);
}

void Campaign::finish(int next) {
    if (unlocked == levelIndex) ++unlocked;
    clearNext = next;
    sounds.push_back(Sound::Clear);
    changeScene(Scene::Clear);
}

void Campaign::interact(Controls controls) {
    const auto& objects = level().objects;
    const Point body{player.x, player.y + playerHeight(power, crouched) / 2};
    for (std::size_t i = 0; i < objects.size(); ++i) {
        const auto& object = objects[i];
        const int type = object.type;
        if (type == 19 && !dragonActive()) {
            if (near(body, {object.position.x, object.position.y + 22}, 23, 44) ||
                near(body, {object.position.x, object.position.y + 250}, 23, 34)) {
                finish(levelIndex + 1);
                return;
            }
        }
        if (type == 16 && controls.down && !downWasHeld && grounded &&
            (levelIndex != 5 || day() > 1) &&
            near(player, {object.position.x, object.position.y + 19}, 38, 26)) {
            pipeExit = objects[i + 1].position;
            sounds.push_back(Sound::Pipe);
            changeScene(Scene::Pipe);
            return;
        }
        if (type == 6 && !collected[i] && near(body, object.position, 26, 35)) {
            collected[i] = true;
            if (power == Power::Full) special = true;
            else power = static_cast<Power>(static_cast<int>(power) + 1);
        }
        if (type == 7 && near(body, object.position, 23, 26)) damage();
    }
}

void Campaign::updateActors() {
    if (theme() == 4) return;
    for (auto& actor : actors) {
        actor.cooldown = std::max(0.0, actor.cooldown - tickSeconds);
        if (actor.health <= 0 || std::abs(actor.position.x - camera) > 345) continue;
        actor.velocityY = std::max(-22.0, actor.velocityY - 1.96);
        move(actor.position, actor.velocityY, false, 10, 23);
        double walk = actor.direction * (actor.type == 24 ? 3.0 : 2.0);
        move(actor.position, walk, true, 10, 23);
        if (walk == 0) actor.direction *= -1;
        if (actor.position.y < upper - 300) {
            actor.health = 0;
            actor.cooldown = 0;
            continue;
        }
        if (!near({player.x, player.y + 10}, {actor.position.x, actor.position.y + 13}, 23, 25)) continue;
        if ((player.y > actor.position.y + 12 && velocity.y < 0) || special) {
            if (actor.cooldown > 0) continue;
            --actor.health;
            actor.cooldown = actor.type == 15 ? 0.5 : 0.22;
            velocity.y = 32;
            grounded = false;
            sounds.push_back(Sound::Stomp);
        } else {
            damage();
            if (scene == Scene::Death) return;
        }
    }
}

void Campaign::updateDragon() {
    if (levelIndex != 5) return;
    if (dragonPhase == 0 && player.x > 9200) {
        dragonPhase = 1;
        dragonHealth = 3;
        dragonTime = 0;
        power = Power::Full;
        sounds.push_back(Sound::Dragon);
    }
    if (!dragonActive()) return;
    camera = 9200;
    player.x = std::clamp(player.x, camera - 222, camera + 222);
    dragonTime += tickSeconds;
    dragonGrace = std::max(0.0, dragonGrace - tickSeconds);
    dragon = {9295 + std::sin(dragonTime * (1.4 + 0.45 * dragonPhase)) * 86,
              upper + 41 + std::sin(dragonTime * (2.3 + 0.45 * dragonPhase)) * 28};
    if (dragonGrace > 0 || !near({player.x, player.y + 15}, dragon, 42, 34)) return;
    const bool stomp = player.y > dragon.y + 3 && velocity.y < 0;
    if (dragonPhase < 3) {
        const bool strong = stomp || special;
        dragonHealth -= dragonPhase == 1 ? (strong ? 0.3 : 0.1) : (strong ? 0.05 : 0.01);
        if (!strong) damage();
    } else if (stomp || special) {
        dragonHealth -= special ? 3 : 1;
        velocity.y = 32;
        grounded = false;
        dragonGrace = 0.25;
        sounds.push_back(Sound::Stomp);
    } else if (invincible == 0) {
        dragonHealth -= 1;
        damage();
        dragonGrace = 0.75;
    }
    if (dragonHealth >= 1) return;
    ++dragonPhase;
    dragonGrace = 1;
    if (dragonPhase == 4) {
        sounds.push_back(Sound::DragonDefeated);
    } else {
        dragonHealth = dragonPhase == 3 ? 5 : 3;
    }
}

void Campaign::tick(Controls controls) {
    if (paused || scene == Scene::Title) return;
    elapsed += tickSeconds;
    switch (scene) {
    case Scene::Title: return;
    case Scene::Intro:
        if (elapsed >= 1.2) changeScene(Scene::Play);
        return;
    case Scene::Pipe:
        if (elapsed < 0.75) return;
        if (pipeExit.x > 1e15) {
            worldTime = 2 * dayLength + dawnLength;
            finish(5);
            return;
        }
        upper = pipeExit.y > 400 ? 1100 : 0;
        player = {pipeExit.x, pipeExit.y + 24};
        velocity = {};
        camera = std::max(0.0, player.x);
        grounded = false;
        jumpTicks = 0;
        resetActors();
        downWasHeld = true;
        changeScene(Scene::Play);
        return;
    case Scene::Death:
        if (elapsed >= 1.15) {
            power = Power::Big;
            special = false;
            spawn();
            changeScene(Scene::Play);
        }
        return;
    case Scene::Clear:
        if (elapsed < 2) return;
        if (levelIndex == 3 && special) worldTime = -6;
        special = false;
        if (clearNext == 6) {
            changeScene(Scene::Ending);
        } else {
            levelIndex = clearNext;
            spawn();
            changeScene(Scene::Play);
        }
        return;
    case Scene::Ending:
        if (elapsed >= 200 * tickSeconds + 1) changeScene(Scene::Title);
        return;
    case Scene::Expired:
        return;
    case Scene::Play:
        break;
    }

    if (!special) worldTime += tickSeconds;
    if (worldTime >= campaignLength) {
        changeScene(Scene::Expired);
        return;
    }
    invincible = std::max(0.0, invincible - tickSeconds);
    if (jumpWasHeld && jumpTicks > 0 && jumpTicks < 9) {
        velocity.y = 14 / 0.77;
        ++jumpTicks;
    }
    if (!controls.jump) jumpTicks = 0;
    jumpWasHeld = controls.jump;
    const bool wantsCrouch = controls.down && power != Power::Small && theme() != 4;
    const bool needsClearance = crouched && blocked({
        player.x - 10, player.x + 10, player.y, player.y + playerHeight(power, false)
    });
    crouched = wantsCrouch || needsClearance;
    if (crouched) controls.move = 0;
    const double height = playerHeight(power, crouched);
    const double width = power == Power::Small ? 5.0 : 10.0;

    velocity.y = std::max(-38.0, velocity.y - (theme() == 3 ? 1.0 : 1.96));
    if (theme() == 4) velocity.y = controls.jump ? 4 : std::max(-4.0, velocity.y);
    grounded = false;
    move(player, velocity.y, false, width, height);
    if (grounded && controls.jump && !crouched && theme() != 4) {
        jumpTicks = 1;
        sounds.push_back(Sound::Jump);
    }
    velocity.x = 0.8 * velocity.x + 4 * controls.move;
    if (controls.move != 0) facing = controls.move < 0 ? -1 : 1;
    move(player, velocity.x, true, width, height);
    const double motion = std::abs(std::round(velocity.x));
    walkFrame = motion == 0 ? 0 : std::fmod(walkFrame + motion / 6, 5.0);
    player.x = std::max(-220.0, player.x);
    if (player.y - upper < -300) {
        die();
        return;
    }

    camera = std::max(0.0, player.x);
    updateDragon();
    if (scene != Scene::Play) return;
    updateActors();
    if (scene != Scene::Play) return;
    interact(controls);
    downWasHeld = controls.down;
}

bool Campaign::importSave(std::string_view code) {
    const auto start = code.find_first_not_of(" \t\r\n");
    if (start == std::string_view::npos) return false;
    code = code.substr(start, code.find_last_not_of(" \t\r\n") - start + 1);
    std::array<std::string_view, 8> fields;
    for (std::size_t i = 0; i < fields.size(); ++i) {
        const auto separator = code.find('|');
        if ((separator == std::string_view::npos) != (i == fields.size() - 1)) return false;
        fields[i] = code.substr(0, separator);
        if (separator != std::string_view::npos) code.remove_prefix(separator + 1);
    }
    if (fields.front() != "[code]" || fields.back() != "[/code]") return false;
    const int progress = readInt(fields[5], -1);
    const auto powerIndex = std::string_view("NBF").find(fields[6]);
    if (progress < 0 || progress > 6 || fields[6].size() != 1 || powerIndex > 2) return false;
    unlocked = progress;
    power = static_cast<Power>(powerIndex);
    return true;
}

std::string Campaign::exportSave() const {
    return "[code]|1|0|0|1|" + std::to_string(unlocked) + "|" +
           "NBF"[static_cast<int>(power)] + "|[/code]";
}

bool checkCampaign(const Levels& levels) {
    const auto simulate = [&](unsigned fps) {
        Campaign sample(levels);
        sample.start();
        sample.scene = Scene::Play;
        sample.collision[29].push_back({-18, 18, -18, 18});
        SimulationClock clock;
        std::uint64_t previous = 0;
        unsigned totalTicks = 0;
        for (unsigned frame = 1; frame <= 2 * fps; ++frame) {
            const auto now = frame * SimulationClock::nanosecondsPerSecond / fps;
            const auto ticks = clock.advance(now - previous);
            previous = now;
            for (unsigned tick = 0; tick < ticks; ++tick) {
                sample.tick({totalTicks < 30 ? 0.0f : 1.0f, totalTicks >= 30 && totalTicks < 42, false});
                ++totalTicks;
            }
        }
        return std::array<double, 5>{double(totalTicks), sample.player.x, sample.player.y,
                                      sample.worldTime, sample.elapsed};
    };
    const auto baseline = simulate(60);
    if (baseline[0] != 120 || baseline[1] <= -100) return false;
    for (const auto fps : {15u, 30u, 75u, 120u, 144u, 240u}) {
        if (simulate(fps) != baseline) return false;
    }

    Campaign game(levels);
    Campaign travel(levels);
    travel.collision[16].push_back({-33, 33, -18, 18});
    int pipes = 0;
    for (int index = 0; index < 6; ++index) {
        const auto& objects = levels[index].objects;
        const auto goal = std::find_if(objects.begin(), objects.end(), [](const Object& object) {
            return object.type == 19;
        });
        if (goal == objects.end()) return false;
        for (std::size_t i = 0; i < objects.size(); ++i) {
            if (objects[i].type == 16) {
                if (i + 1 >= objects.size() || objects[i + 1].type != 18) return false;
                ++pipes;
                travel.start(index);
                travel.scene = Scene::Play;
                const auto entrance = objects[i].position;
                const auto destination = objects[i + 1].position;
                travel.player = {entrance.x, entrance.y + 18};
                travel.upper = entrance.y > 400 ? 1100 : 0;
                travel.dragonPhase = 4;
                if (index == 5) {
                    travel.tick({0, false, true});
                    if (travel.scene != Scene::Play) return false;
                    travel.tick({});
                    travel.worldTime = dayLength;
                }
                travel.tick({0, false, true});
                if (travel.scene != Scene::Pipe) return false;
                for (int frame = 0; frame < 46; ++frame) travel.tick({});
                if (destination.x > 1e15) {
                    if (travel.scene != Scene::Clear || travel.day() != 3) return false;
                    for (int frame = 0; frame < 125; ++frame) travel.tick({});
                    if (travel.levelIndex != 5) return false;
                } else if (travel.scene != Scene::Play || travel.player.x != destination.x ||
                           travel.upper != (destination.y > 400 ? 1100 : 0)) {
                    return false;
                }
            }
        }
        game.start(index);
        game.scene = Scene::Play;
        game.player = {goal->position.x, goal->position.y + 18};
        game.upper = goal->position.y > 400 ? 1100 : 0;
        if (index == 5) game.dragonPhase = 4;
        game.tick({});
        if (game.scene != Scene::Clear || game.unlocked != index + 1) return false;
        for (int frame = 0; frame < 125; ++frame) game.tick({});
        if (index < 5 && game.levelIndex != index + 1) return false;
        if (index == 5 && game.scene != Scene::Ending) return false;
    }
    if (pipes != 20) return false;
    game.start(3);
    game.scene = Scene::Play;
    game.special = true;
    game.worldTime = 70;
    for (const auto& object : game.level().objects) {
        if (object.type == 19) game.player = {object.position.x, object.position.y + 18};
    }
    game.tick({});
    if (game.scene != Scene::Clear || game.unlocked != 6) return false;
    for (int frame = 0; frame < 125; ++frame) game.tick({});
    if (game.levelIndex != 4 || game.special || game.worldTime >= 0) return false;

    game.start(5);
    game.scene = Scene::Play;
    game.player = {9201, 120};
    game.tick({});
    if (!game.dragonActive() || game.dragonPhase != 1 || game.camera != 9200) return false;
    for (int phase = 1; phase <= 3; ++phase) {
        game.dragonGrace = 0;
        game.dragonHealth = 1;
        game.player = {9295, 41};
        game.velocity = {};
        game.special = true;
        game.tick({});
        if (game.dragonPhase != phase + 1) return false;
    }
    const auto code = game.exportSave();
    Campaign loaded(levels);
    if (!loaded.importSave(code) || loaded.unlocked != 6 ||
        loaded.importSave("[code]|1|0|0|1|999|F|[/code]") ||
        loaded.importSave("[code]|1|0|0|1|1||[/code]")) return false;
    loaded.start();
    loaded.scene = Scene::Play;
    loaded.player.y = -301;
    loaded.tick({});
    if (loaded.scene != Scene::Death) return false;
    for (int frame = 0; frame < 72; ++frame) loaded.tick({});
    if (loaded.scene != Scene::Play || loaded.levelIndex != 0 || loaded.power != Power::Big) return false;
    loaded.worldTime = campaignLength;
    loaded.tick({});
    return loaded.scene == Scene::Expired;
}
