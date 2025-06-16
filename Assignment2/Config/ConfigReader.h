#pragma once

#include "../GameObjects/Direction.h"

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

class ConfigReader
{
private:
    // === Static Value === //
    static const char comment_sign = '#';
    static const char equal_sign = '=';
    static const char list_delimiter = ',';

    // === Attributes === //
    // speed
    size_t shells_speed = 2;

    // spawn directions
    std::vector<Direction> spawn_directions = {
        Direction::LEFT,
        Direction::RIGHT,
        Direction::LEFT,
        Direction::RIGHT,
        Direction::LEFT,
        Direction::RIGHT,
        Direction::LEFT,
        Direction::RIGHT,
        Direction::LEFT,
    };

    // hp
    size_t wall_hp = 2;
    size_t tank_hp = 1;

    // damage
    size_t shell_damage = 1;
    size_t mine_damage = 1;
    size_t tanks_collision_damage = 1;

    // game settings
    size_t steps_after_shells_end = 40;
    size_t shoot_cooldown_steps = 4;
    size_t backward_wait_steps = 2;

    // === Constructor === //
    explicit ConfigReader(const std::string file_path)
    {
        std::ifstream config_file(file_path, std::ios::in);

        if (!config_file)
            return; // can't open file - stick to default values

        std::string line;
        while (std::getline(config_file, line))
        {
            if (line.empty() || line[0] == ConfigReader::comment_sign)
                continue;

            size_t equal_pos = line.find(ConfigReader::equal_sign);
            std::string key = line.substr(0, equal_pos);
            std::string value = line.substr(equal_pos + 1);

            parseKeyValue(key, value);
        }
    }

    // === Parsing Functions === //
    void parseKeyValue(const std::string &key, const std::string &value)
    {
        if (key == "spawn_directions")
        {
            this->spawn_directions = parseSpawnDirectionList(value);
            return;
        }

        static const std::unordered_map<std::string, size_t ConfigReader::*> key_map = {
            {"shells_speed", &ConfigReader::shells_speed},
            {"wall_hp", &ConfigReader::wall_hp},
            {"tank_hp", &ConfigReader::tank_hp},
            {"shell_damage", &ConfigReader::shell_damage},
            {"mine_damage", &ConfigReader::mine_damage},
            {"tanks_collision_damage", &ConfigReader::tanks_collision_damage},
            {"steps_after_shells_end", &ConfigReader::steps_after_shells_end},
            {"shoot_cooldown_steps", &ConfigReader::shoot_cooldown_steps},
            {"backward_wait_steps", &ConfigReader::backward_wait_steps},
        };

        auto it = key_map.find(key);
        if (it != key_map.end())
        {
            size_t num;
            if (parseNumber(value, num))
                this->*(it->second) = num;
        }
    }

    bool parseNumber(const std::string &value, size_t &num)
    {
        try
        {
            num = std::stoul(value);
            return true;
        }
        catch (const std::exception &)
        {
            return false;
        }
    }

    std::vector<Direction> parseSpawnDirectionList(const std::string &value)
    {
        size_t max_players = 9;
        std::vector<Direction> directions(max_players);

        std::string direction_string;
        size_t start = 0, end = 0;
        size_t i = 0;
        while ((end = value.find(list_delimiter, start)) != std::string::npos &&
               i < max_players - 1)
        {
            direction_string = value.substr(start, end - start);
            directions[i++] = parseDirection(direction_string);
            start = end + 1;
        }

        // last config direction
        direction_string = value.substr(start);
        directions[i++] = parseDirection(direction_string);

        // fill remaining directions
        while (i < max_players)
        {
            directions[i++] = (directions.size() % 2 == 1) ? Direction::LEFT : Direction::RIGHT;
        }

        return directions;
    }

    Direction parseDirection(const std::string &token)
    {
        if (token == "LEFT")
            return Direction::LEFT;
        if (token == "RIGHT")
            return Direction::RIGHT;
        if (token == "UP")
            return Direction::UP;
        if (token == "DOWN")
            return Direction::DOWN;
        if (token == "UPL")
            return Direction::UPL;
        if (token == "UPR")
            return Direction::UPR;
        if (token == "DOWNL")
            return Direction::DOWNL;
        if (token == "DOWNR")
            return Direction::DOWNR;

        return Direction::LEFT; // default
    }

public:
    // === Destructor === //
    ~ConfigReader() = default;

    // === Copy & Move Constructors, Operators (Deleted) === //
    ConfigReader(const ConfigReader &other) = delete;
    ConfigReader(const ConfigReader &&other) = delete;
    ConfigReader operator=(const ConfigReader &other) = delete;
    ConfigReader operator=(const ConfigReader &&other) = delete;

    // === Getters === //
    const std::vector<Direction> &getSpawnDirections() const { return spawn_directions; }
    size_t getShellsSpeed() const { return shells_speed; }
    size_t getWallHp() const { return wall_hp; }
    size_t getTankHp() const { return tank_hp; }
    size_t getShellDamage() const { return shell_damage; }
    size_t getMineDamage() const { return mine_damage; }
    size_t getTankCollisionDamage() const { return tanks_collision_damage; }
    size_t getStepsAfterShellsEnd() const { return steps_after_shells_end; }
    size_t getShootCooldownSteps() const { return shoot_cooldown_steps; }
    size_t getBackwardWaitSteps() const { return backward_wait_steps; }

    // === static instance getter === //
    static ConfigReader &getConfig()
    {
        static ConfigReader instance(std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of("/\\") + 1) + "../config.txt");
        return instance;
    }
};