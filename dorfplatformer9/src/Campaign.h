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
enum class Sound { Jump, Stomp, Pipe, Death, Clear, Dragon, DragonDefeated };

struct Controls {
    float move = 0;
    bool jump = false;
    bool down = false;
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
    double dragonTime = 0;
    double dragonHealth = 0;
    double dragonGrace = 0;
    Point dragon;
    int dragonPhase = 0;
    int facing = 1;
    bool grounded = false;
    bool crouched = false;
    bool paused = false;
    bool special = false;
    double walkFrame = 0;
    std::vector<Actor> actors;
    std::vector<bool> collected;
    std::vector<Sound> sounds;
    std::array<std::vector<Box>, 31> collision;

    int day() const;
    int theme() const;
    bool night() const;
    float darkness() const;
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
    double jumpTicks = 0;
    int clearNext = 0;

    void spawn();
    void resetActors();
    void changeScene(Scene next);
    void move(Point& point, double& speed, bool horizontal, double halfWidth, double height);
    bool blocked(const Box& bounds) const;
    void interact(Controls controls);
    void updateActors();
    void updateDragon();
    void damage();
    void die();
    void finish(int next);
};

bool checkCampaign(const Levels& levels);
