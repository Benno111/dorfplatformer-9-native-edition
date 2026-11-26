#pragma once

#include <array>
#include <filesystem>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

struct Point {
    double x = 0;
    double y = 0;
};

struct Object {
    Point position;
    int type = 0;
};

struct Level {
    std::string name;
    int theme = 0;
    int upperTheme = 0;
    std::vector<Object> objects;
};

using Levels = std::array<Level, 6>;
std::istringstream openGameData(const std::filesystem::path& path);
Levels loadLevels(const std::filesystem::path& path);

enum class Power : int { Small, Big, Full };
enum class Scene { Title, Intro, Play, Pipe, Death, Clear, Ending, Expired };
enum class Sound { Jump, Stomp, Pipe, Death, Clear, Dragon, DragonDefeated, Fireball, FireballHit, PowerUp };

struct Controls {
    float move = 0;
    bool jump = false;
    bool down = false;
    bool fire = false;
};

struct Box {
    double left, right, bottom, top;
};

struct Actor {
    Point position;
    int type;
    int direction = -1;
    double velocityY = 0;
    double cooldown = 0;
    int health;
};

struct Fireball {
    Point position;
    Point velocity;
    int ticksLeft;
};

struct Campaign {
    explicit Campaign(const Levels& data) : levels(data) {}

    const Levels& levels;
    Scene scene = Scene::Title;
    int levelIndex = 0;
    int unlocked = 0;
    int upper = 0;
    Power power = Power::Full;
    Point player;
    Point velocity;
    double camera = 0;
    double worldTime = -6;
    double elapsed = 0;
    double invincible = 0;
    double dragonHealth = 0;
    double dragonGrace = 0;
    Point dragon;
    int dragonDirection = 1;
    double dragonTransition = 0;
    int dragonPhase = 0;
    int facing = 1;
    bool grounded = false;
    bool crouched = false;
    bool paused = false;
    double specialRemaining = 0;
    bool special() const { return specialRemaining > 0; }
    double walkFrame = 0;
    std::vector<Actor> actors;
    std::vector<Fireball> fireballs;
    std::vector<Point> meteors;
    std::vector<bool> collected;
    std::vector<Sound> sounds;
    std::array<std::vector<Box>, 31> collision;

    int day() const;
    int theme() const;
    int backdropIndex() const { return dragonPhase == 3 ? 7 : theme(); }
    Point exitPosition() const;
    static constexpr Box exitBounds{-30.261, 30.261, 59.945, 120.467};
    bool night() const;
    float darkness() const;
    float clockProgress() const;
    double finalNightSeconds() const;
    bool dragonActive() const { return dragonPhase >= 1 && dragonPhase <= 3; }
    const Level& level() const { return levels.at(levelIndex); }
    void start(int selected = 0);
    void tick(Controls controls);
    bool importSave(std::string_view code);
    std::string exportSave() const;

private:
    Point pipeExit;
    bool downWasHeld = false;
    bool jumpWasHeld = false;
    int jumpHoldTicks = 0;
    unsigned playerMovementPhase = 0;
    int bounceProtectionTicks = 0;
    int clearNext = 0;
    int fireCooldown = 0;
    unsigned dragonDrops = 0;
    unsigned meteorDrops = 0;
    int meteorCooldown = 0;

    void spawn();
    void resetActors();
    void changeScene(Scene next);
    void move(Point& point, double& speed, bool horizontal, double halfWidth, double height);
    bool blocked(const Box& bounds) const;
    void interact(Controls controls);
    void updatePlayer(Controls controls);
    void updateActors();
    void updateFireballs(bool fire);
    void updateDragon();
    void updateMeteors();
    void bounce(int protectionTicks);
    void damage();
    void die();
    void finish(int next);
};

bool checkCampaign(const Levels& levels);
