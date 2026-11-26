#include "Campaign.h"
#include "SimulationClock.h"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <SDL3/SDL.h>

#include <memory>
#include <stdexcept>

namespace {
constexpr double invincibilityDuration = 12 * 165879.0 / 48000; // Twelve Jungle loops.
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
            level.name != "1-" + std::to_string(i + 1) || count == 0 || count > 100000 ||
            level.theme < 1 || level.theme > 6 || level.upperTheme < 1 || level.upperTheme > 6) {
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

Point Campaign::exitPosition() const {
    const auto& objects = level().objects;
    const auto exit = std::find_if(objects.begin(), objects.end(), [](const Object& object) {
        return object.type == 19;
    });
    return exit == objects.end() ? Point{1500, double(upper)} : exit->position;
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
    const double start = (day() - 1) * (dayLength + dawnLength);
    const double localTime = worldTime - start;
    return (day() > 1 && localTime < 0) || localTime >= daylightLength;
}

float Campaign::clockProgress() const {
    const double localTime = worldTime - (day() - 1) * (dayLength + dawnLength);
    const double progress = night() ? (localTime - daylightLength - sunset) / nightLength
                                    : localTime / daylightLength;
    return static_cast<float>(std::clamp(progress, 0.0, 1.0));
}

double Campaign::finalNightSeconds() const {
    const double remaining = campaignLength - worldTime;
    if (remaining > nightLength) return -1;
    return std::max(0.0, remaining) / (clockStep * 60);
}

void Campaign::changeScene(Scene next) {
    scene = next;
    elapsed = 0;
    if (next != Scene::Play) {
        fireballs.clear();
        fireCooldown = 0;
    }
}

void Campaign::start(int selected) {
    levelIndex = std::clamp(selected, 0, 5);
    worldTime = -6;
    specialRemaining = 0;
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
        if ((object.type == 15 || object.type == 24 || object.type == 25) &&
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
    jumpHoldTicks = 0;
    playerMovementPhase = 0;
    bounceProtectionTicks = 0;
    dragonPhase = 0;
    dragonHealth = 0;
    dragonGrace = 0;
    dragonTransition = 0;
    dragonDrops = 0;
    meteorDrops = 0;
    meteorCooldown = 0;
    meteors.clear();
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

void Campaign::bounce(int protectionTicks) {
    velocity.y = 32;
    grounded = false;
    jumpHoldTicks = 0;
    bounceProtectionTicks = protectionTicks;
    sounds.push_back(Sound::Stomp);
}

void Campaign::damage() {
    if (invincible > 0 || bounceProtectionTicks > 0 || special()) return;
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
    const auto exit = exitPosition();
    const double halfWidth = power == Power::Small ? 5 : 10;
    const Box bodyBounds{player.x - halfWidth, player.x + halfWidth, player.y,
                         player.y + playerHeight(power, crouched)};
    if (!dragonActive() && (exit.y > 400) == (upper != 0) &&
        overlaps(bodyBounds, exitBounds, exit)) {
        finish(levelIndex + 1);
        return;
    }
    for (std::size_t i = 0; i < objects.size(); ++i) {
        const auto& object = objects[i];
        const int type = object.type;
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
            if (power == Power::Full) {
                if (special()) continue;
                specialRemaining = invincibilityDuration;
            } else {
                power = static_cast<Power>(static_cast<int>(power) + 1);
            }
            sounds.push_back(Sound::PowerUp);
        }
        if (type == 7 && near(body, object.position, 23, 26)) damage();
    }
}

void Campaign::updateActors() {
    if (theme() == 4) return;
    for (auto& actor : actors) {
        actor.cooldown = std::max(0.0, actor.cooldown - tickSeconds);
        if (actor.health <= 0 || std::abs(actor.position.x - camera) > 425) continue;
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
        if ((player.y > actor.position.y + 12 && velocity.y < 0) || bounceProtectionTicks > 0 || special()) {
            if (actor.cooldown > 0) continue;
            --actor.health;
            actor.cooldown = actor.type == 15 ? 0.5 : 0.22;
            bounce(actor.type == 24 ? 14 : 15);
        } else {
            damage();
            if (scene == Scene::Death) return;
        }
    }
}

void Campaign::updateFireballs(bool fire) {
    if (fireCooldown > 0) --fireCooldown;
    if (fire && power == Power::Full && fireCooldown == 0) {
        fireCooldown = 12;
        fireballs.push_back({{player.x, player.y + 22}, {14.0 * facing, 9}, 120});
        sounds.push_back(Sound::Fireball);
    }

    for (auto& ball : fireballs) {
        if (ball.ticksLeft <= 0) continue;
        --ball.ticksLeft;
        move(ball.position, ball.velocity.x, true, 4, 8);
        if (ball.velocity.x == 0 || std::abs(ball.position.x - camera) > 320 ||
            ball.position.y < upper - 180 || ball.position.y > upper + 180) {
            ball.ticksLeft = 0;
            continue;
        }
        ball.velocity.y -= 2;
        const bool falling = ball.velocity.y < 0;
        move(ball.position, ball.velocity.y, false, 4, 8);
        if (falling && ball.velocity.y == 0) ball.velocity.y = 13;

        for (auto& actor : actors) {
            if (actor.health <= 0 || !near(ball.position, {actor.position.x, actor.position.y + 12}, 18, 22)) continue;
            actor.health = 0;
            actor.cooldown = 0.5;
            ball.ticksLeft = 0;
            sounds.push_back(Sound::Stomp);
            break;
        }
    }
    fireballs.erase(std::remove_if(fireballs.begin(), fireballs.end(),
                                   [](const Fireball& ball) { return ball.ticksLeft <= 0; }),
                    fireballs.end());
}

void Campaign::updateDragon() {
    if (levelIndex != 5) return;
    if (dragonPhase == 0 && player.x > 9200) {
        dragonPhase = 1;
        dragonHealth = 3;
        dragonDirection = 1;
        dragon = {9295, upper + 41.0};
        power = Power::Full;
        sounds.push_back(Sound::Dragon);
    }
    if (!dragonActive()) return;
    camera = 9200;
    player.x = std::clamp(player.x, camera - 222, camera + 222);
    if (dragonTransition > 0) {
        dragonTransition = std::max(0.0, dragonTransition - tickSeconds);
        return;
    }

    dragonGrace = std::max(0.0, dragonGrace - tickSeconds);
    if (dragonPhase < 3) {
        dragon.x += dragonDirection;
        if (dragon.x >= 9403 || dragon.x <= 9245) dragonDirection *= -1;
        for (auto& ball : fireballs) {
            if (ball.ticksLeft <= 0 || !near(ball.position, dragon, 88, 70)) continue;
            const bool movingRight = dragonDirection > 0;
            dragonHealth -= dragonPhase == 1 ? (movingRight ? 0.10 : 0.30)
                                             : (movingRight ? 0.01 : 0.05);
            ball.ticksLeft = 0;
            sounds.push_back(Sound::FireballHit);
        }
        if (near({player.x, player.y + 15}, dragon, 76, 63)) {
            if (special()) dragonHealth = 0;
            else damage();
        }
    } else {
        const Box next{dragon.x - 15, dragon.x + 15, dragon.y - 17, dragon.y + 13};
        if (!blocked(next)) dragon.y -= 3;
        const bool touch = near({player.x, player.y + 15}, dragon, 31, 34);
        const bool stomp = player.y > dragon.y + 3 && velocity.y < 0;
        if (touch && dragonGrace == 0 && (stomp || special())) {
            dragonHealth -= special() ? 3 : 1;
            bounce(15);
            dragonGrace = 0.25;
        } else if (touch && dragonGrace == 0 && invincible == 0 && bounceProtectionTicks == 0) {
            dragonHealth -= 1;
            damage();
            dragonGrace = 0.75;
        }
        if (dragon.y < upper - 180 || touch || blocked(next)) {
            // Cycle across the arena so falls are reproducible across render rates.
            constexpr int dropOffsets[] = {95, -90, 155, -150, 25};
            dragon = {camera + dropOffsets[dragonDrops++ % 5], upper + 250.0};
        }
    }
    if (dragonHealth >= 1 || scene != Scene::Play) return;
    ++dragonPhase;
    if (dragonPhase == 4) {
        sounds.push_back(Sound::DragonDefeated);
    } else if (dragonPhase == 2) {
        dragonHealth = 3;
        dragonTransition = 1;
        dragonDirection = 1;
    } else {
        dragonHealth = 5;
        dragon = {9295, upper + 250.0};
    }
}

void Campaign::updateMeteors() {
    if (dragonPhase != 3) {
        meteors.clear();
        return;
    }
    if (meteorCooldown > 0) --meteorCooldown;
    if (meteorCooldown == 0) {
        constexpr int offsets[] = {-158, 40, 174, -47, 111, -110};
        meteors.push_back({9200.0 + offsets[meteorDrops++ % 6], upper + 220.0});
        meteorCooldown = 42;
    }
    constexpr double radius = 9;
    const Point body{player.x, player.y + playerHeight(power, crouched) / 2};
    meteors.erase(std::remove_if(meteors.begin(), meteors.end(), [&](Point& meteor) {
        meteor.y -= 4;
        if (near(body, meteor, radius + 9, radius + playerHeight(power, crouched) / 2)) {
            damage();
            return true;
        }
        return meteor.y < upper - 180 ||
               blocked({meteor.x - radius, meteor.x + radius, meteor.y - radius, meteor.y + radius});
    }), meteors.end());
}

void Campaign::updatePlayer(Controls controls) {
    // The original applies the held jump from the previous tick before reading
    // movement for this tick. A press starts its hold on downward contact.
    if (jumpHoldTicks > 0 && jumpWasHeld && jumpHoldTicks < 8) {
        velocity.y = 14 / 0.77;
        ++jumpHoldTicks;
    }
    if (!jumpWasHeld) jumpHoldTicks = 0;
    const int currentTheme = theme();
    const bool wantsCrouch = controls.down && power != Power::Small && currentTheme != 4;
    const bool needsClearance = crouched && blocked({
        player.x - 10, player.x + 10, player.y, player.y + playerHeight(power, false)
    });
    crouched = wantsCrouch || needsClearance;
    if (crouched) {
        controls.move = 0;
        controls.jump = false;
    }
    jumpWasHeld = controls.jump;
    const double height = playerHeight(power, crouched);
    const double width = power == Power::Small ? 5.0 : 10.0;

    if (player.x < -240) {
        player.x = -220;
        velocity.x = 18;
    }
    velocity.y -= currentTheme == 3 ? 1.0 : 1.96;
    if (currentTheme == 4) velocity.y = controls.jump ? 4 : std::max(-4.0, velocity.y);
    const bool rising = velocity.y > 0;
    grounded = false;
    move(player, velocity.y, false, width, height);
    if (grounded) {
        jumpHoldTicks = controls.jump && currentTheme != 4 ? 1 : 0;
        if (jumpHoldTicks != 0) sounds.push_back(Sound::Jump);
    } else if (rising && velocity.y == 0) {
        jumpHoldTicks = 0;
    }
    velocity.x = (velocity.x / 2.5 + 2 * controls.move) * 2;
    if (controls.move != 0) facing = controls.move < 0 ? -1 : 1;
    move(player, velocity.x, true, width, height);
    const double motion = std::abs(std::round(velocity.x));
    walkFrame = motion == 0 ? 0 : std::fmod(walkFrame + motion / 6, 5.0);
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
        jumpHoldTicks = 0;
        playerMovementPhase = 0;
        bounceProtectionTicks = 0;
        resetActors();
        downWasHeld = true;
        changeScene(Scene::Play);
        return;
    case Scene::Death:
        if (elapsed >= 1.15) {
            power = Power::Big;
            specialRemaining = 0;
            spawn();
            changeScene(Scene::Play);
        }
        return;
    case Scene::Clear:
        if (elapsed < 2) return;
        if (levelIndex == 3 && special()) worldTime = -6;
        specialRemaining = 0;
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

    if (special()) specialRemaining = std::max(0.0, specialRemaining - tickSeconds);
    else worldTime += tickSeconds;
    if (worldTime >= campaignLength) {
        changeScene(Scene::Expired);
        return;
    }
    invincible = std::max(0.0, invincible - tickSeconds);
    if (bounceProtectionTicks > 0) --bounceProtectionTicks;
    // Run movement at 30 Hz to preserve the per-step jump arc at a slower pace.
    // Contact protection and campaign timers still use the 60 Hz simulation clock.
    if (playerMovementPhase == 0) updatePlayer(controls);
    playerMovementPhase = (playerMovementPhase + 1) % (SimulationClock::ticksPerSecond / 30);
    if (player.y - upper < -300) {
        die();
        return;
    }

    camera = std::max(0.0, player.x);
    updateFireballs(controls.fire);
    updateDragon();
    if (scene != Scene::Play) return;
    updateMeteors();
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

    Levels movementLevels = levels;
    movementLevels[0].objects = {{{40, 0}, 15}, {{0, 12}, 7}};
    Campaign movement(movementLevels);
    movement.start();
    movement.scene = Scene::Play;
    movement.player = {40, 20};
    movement.velocity.y = -2;
    movement.tick({});
    if (movement.velocity.y != 32 || movement.actors.front().health != 0) return false;
    for (int tick = 1; tick <= 15; ++tick) {
        movement.player = {0, 0};
        movement.velocity = {};
        movement.tick({});
        const auto expectedPower = tick < 15 ? Power::Full : Power::Big;
        if (movement.power != expectedPower) return false;
    }

    movementLevels[0].objects = {{{0, -18}, 29}};
    const auto jumpHeight = [&](bool holdJump) {
        Campaign jumper(movementLevels);
        jumper.collision[29].push_back({-18, 18, -18, 18});
        jumper.start();
        jumper.scene = Scene::Play;
        jumper.player = {0, 0};
        double apex = 0;
        for (int tick = 0; tick < 60; ++tick) {
            jumper.tick({0, holdJump || tick == 0, false});
            apex = std::max(apex, jumper.player.y);
        }
        return apex;
    };
    movementLevels[0].objects.clear();
    Campaign walking(movementLevels);
    walking.start();
    walking.scene = Scene::Play;
    walking.tick({1, false, false});
    const double firstX = walking.player.x;
    walking.tick({1, false, false});
    if (firstX != -96 || walking.player.x != firstX) return false;
    walking.tick({1, false, false});
    if (walking.player.x != -89) return false;
    movementLevels[0].objects = {{{0, -18}, 29}};

    if (jumpHeight(false) != 74 || jumpHeight(true) != 170) return false;

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
        game.player = {goal->position.x, goal->position.y + 250};
        game.upper = goal->position.y > 400 ? 1100 : 0;
        if (index == 5) game.dragonPhase = 4;
        game.tick({});
        if (game.scene != Scene::Play) return false;
        game.upper = game.upper == 0 ? 1100 : 0;
        if (game.upper == 0) {
            game.player = {goal->position.x, goal->position.y + 65};
            game.tick({});
            if (game.scene != Scene::Play) return false;
        }
        game.player = {goal->position.x, goal->position.y + 65};
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
    game.specialRemaining = invincibilityDuration;
    game.worldTime = 70;
    for (const auto& object : game.level().objects) {
        if (object.type == 19) game.player = {object.position.x, object.position.y + 65};
    }
    game.tick({});
    if (game.scene != Scene::Clear || game.unlocked != 6) return false;
    for (int frame = 0; frame < 125; ++frame) game.tick({});
    if (game.levelIndex != 4 || game.special() || game.worldTime >= 0) return false;

    Levels poweredLevels = levels;
    poweredLevels[0].objects = {{{0, 12}, 6}, {{0, 12}, 7}};
    Campaign powered(poweredLevels);
    powered.start();
    powered.scene = Scene::Play;
    powered.player = {0, 0};
    powered.tick({});
    if (!powered.special() || powered.specialRemaining != invincibilityDuration ||
        powered.power != Power::Full) return false;
    const double frozenTime = powered.worldTime;
    powered.specialRemaining = tickSeconds;
    powered.player = {0, 0};
    powered.velocity = {};
    powered.tick({});
    if (powered.special() || powered.worldTime != frozenTime || powered.power != Power::Big) return false;
    powered.tick({});
    if (powered.worldTime <= frozenTime) return false;

    Levels projectileLevels = levels;
    projectileLevels[0].objects = {{{58, 16}, 15}};
    Campaign shooting(projectileLevels);
    shooting.start();
    shooting.scene = Scene::Play;
    shooting.player = {0, 0};
    shooting.power = Power::Big;
    shooting.tick({0, false, false, true});
    if (!shooting.fireballs.empty()) return false;
    shooting.power = Power::Full;
    shooting.tick({0, false, false, true});
    if (shooting.fireballs.size() != 1 || shooting.fireballs.front().position.x != 14) return false;
    for (int i = 0; i < 4; ++i) shooting.tick({});
    if (shooting.actors.front().health != 0 || !shooting.fireballs.empty()) return false;

    game.start(5);
    game.scene = Scene::Play;
    game.player = {9201, 120};
    game.tick({});
    if (!game.dragonActive() || game.dragonPhase != 1 || game.camera != 9200 ||
        game.dragon.x != 9296 || game.dragon.y != 41) return false;
    for (int phase = 1; phase <= 2; ++phase) {
        game.dragonTransition = 0;
        game.dragonHealth = 1;
        game.player = {9200, 5};
        game.fireballs.push_back({game.dragon, {14, 0}, 12});
        game.tick({});
        if (game.dragonPhase != phase + 1) return false;
    }
    if (game.dragon.y != 250 || game.dragonHealth != 5 || game.meteors.size() != 1) return false;
    game.meteors.push_back({9200, 39});
    game.player = {9200, 12};
    game.velocity = {};
    game.tick({});
    if (game.power != Power::Big || game.meteors.size() != 1) return false;
    game.dragonHealth = 1;
    game.dragon = {9295, 41};
    game.player = {9295, 41};
    game.velocity = {};
    game.specialRemaining = invincibilityDuration;
    game.tick({});
    if (game.dragonPhase != 4 || !game.meteors.empty()) return false;
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
