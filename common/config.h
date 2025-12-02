#ifndef CONFIG_H
#define CONFIG_H

#include <yaml-cpp/yaml.h>
#include <string>

class Config {
public:
    static const Config& get() { //singleton global para todo el projecto
        static Config instance;
        return instance;
    }

    // gameloop
    int PIXELS_PER_METER() const { return pixels_per_meter; }
    float RACE_DURATION_SECONDS() const { return race_duration_seconds; }
    float MAX_RACES() const { return max_races; }

    // collisions
    float DELTA_SPEED_TOLERANCE() const { return delta_speed_tolerance; }

private:

    Config() {
        YAML::Node config = YAML::LoadFile("../common/config.yaml");

        // gameloop
        pixels_per_meter = config["gameloop"]["pixels_per_meter"].as<int>();
        race_duration_seconds = config["gameloop"]["race_duration_seconds"].as<float>();
        max_races = config["gameloop"]["max_races"].as<int>();

        // collisions
        delta_speed_tolerance = config["collisions"]["delta_speed_tolerance"].as<float>();
    }

    int pixels_per_meter;
    float race_duration_seconds;
    int max_races;

    float delta_speed_tolerance;

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};

#endif