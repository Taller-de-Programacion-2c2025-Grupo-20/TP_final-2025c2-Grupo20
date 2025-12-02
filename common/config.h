#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

#include <yaml-cpp/yaml.h>

#include "constants.h"

class Config {
public:
    static const Config& get() {  // singleton global para todo el projecto
        static Config instance;
        return instance;
    }

    // gameloop
    int PIXELS_PER_METER() const { return pixels_per_meter; }
    float RACE_DURATION_SECONDS() const { return race_duration_seconds; }
    int MAX_RACES() const { return max_races; }

    // collisions
    float DELTA_SPEED_TOLERANCE() const { return delta_speed_tolerance; }
    float DAMAGE_FACTOR() const { return damage_factor; }

    // upgrades
    float SPEED_UPGRADE_VALUE() const { return speed_upgrade; }
    float SPEED_PENALTY() const { return speed_penalty; }

    float ACCELERATION_UPGRADE_VALUE() const { return acceleration_upgrade; }
    float ACCELERATION_PENALTY() const { return acceleration_penalty; }

    float HEALTH_UPGRADE_VALUE() const { return health_upgrade; }
    float HEALTH_PENALTY() const { return health_penalty; }

    // cars
    const CarAttributes& getCarAttributes(const CarType& type) const {
        return cars_attributes.at(type);
    }

private:
    static CarType carTypeFromString(const std::string& string) {
        if (string == "VERDE") {
            return CarType::VERDE;
        }
        if (string == "ROJO") {
            return CarType::ROJO;
        }
        if (string == "DESCAPOTABLE") {
            return CarType::DESCAPOTABLE;
        }
        if (string == "CELESTE") {
            return CarType::CELESTE;
        }
        if (string == "JEEP") {
            return CarType::JEEP;
        }
        if (string == "CAMIONETA") {
            return CarType::CAMIONETA;
        }
        if (string == "CAMION") {
            return CarType::CAMION;
        }
        throw std::runtime_error("CarType invalido: " + string);
    }

    Config() {
        YAML::Node config = YAML::LoadFile(std::string(CONFIG_DATA_PATH) + "config.yaml");

        // gameloop
        pixels_per_meter = config["gameloop"]["pixels_per_meter"].as<int>();
        race_duration_seconds = config["gameloop"]["race_duration_seconds"].as<float>();
        max_races = config["gameloop"]["max_races"].as<int>();

        // collisions
        delta_speed_tolerance = config["collisions"]["delta_speed_tolerance"].as<float>();
        damage_factor = config["collisions"]["damage_factor"].as<float>();

        // upgrades
        speed_upgrade = config["upgrades"]["speed_upgrade"].as<float>();
        speed_penalty = config["upgrades"]["speed_penalty"].as<float>();

        acceleration_upgrade = config["upgrades"]["acceleration_upgrade"].as<float>();
        acceleration_penalty = config["upgrades"]["acceleration_penalty"].as<float>();

        health_upgrade = config["upgrades"]["health_upgrade"].as<int>();
        health_penalty = config["upgrades"]["health_penalty"].as<float>();

        // cars
        for (const auto& car: config["cars"]) {
            std::string name = car.first.as<std::string>();
            CarType car_type = carTypeFromString(name);

            YAML::Node car_attributes = car.second;
            CarAttributes attr;

            attr.acceleration = car_attributes["acceleration"].as<float>();
            attr.rotation_torque = car_attributes["rotation_torque"].as<float>();
            attr.max_speed = car_attributes["max_speed"].as<float>();
            attr.health = car_attributes["health"].as<uint8_t>();
            attr.width = car_attributes["width"].as<float>();
            attr.height = car_attributes["height"].as<float>();
            attr.density = car_attributes["density"].as<float>();
            attr.angular_damping = car_attributes["angular_damping"].as<float>();
            attr.linear_damping = car_attributes["linear_damping"].as<float>();

            cars_attributes[car_type] = attr;
        }
    }

    // gameloop
    int pixels_per_meter;
    float race_duration_seconds;
    int max_races;

    // collisions
    float delta_speed_tolerance;
    float damage_factor;

    // upgrades
    float speed_upgrade;
    float speed_penalty;

    float acceleration_upgrade;
    float acceleration_penalty;

    int health_upgrade;
    float health_penalty;

    // cars
    std::unordered_map<CarType, CarAttributes> cars_attributes;

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};

#endif