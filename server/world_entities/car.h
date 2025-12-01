#ifndef CAR_H
#define CAR_H

#include <string>
#include <set>

#include <Box2D/Box2D.h>

#include "../../common/clientCommand.h"
#include "../../common/constants.h"

#include "entity.h"

struct CarAttributes {
    float acceleration;
    float rotation_torque;
    float max_speed;
    uint8_t health;
    float width;
    float height;
    float density;
    float angular_damping;
    float linear_damping;
};

class Car: public Entity {
private:
    float accelaration;
    float rotation_torque;
    float max_speed;
    CarType car_type;
    uint8_t car_health;

    std::set<UpgradeType> applied_upgrades;
    float time_penalization;
    float next_race_time_penalty;
    
    bool accelerating;
    bool braking;
    bool turningLeft;
    bool turningRight;

    int next_checkpoint_id = 0;

    float pre_collision_speed = 0;

    b2Body* car_body;

    CarAttributes getCarTypeAttributes(CarType car_type);

    b2Vec2 getLateralVelocity();

    b2Vec2 getForwardVelocity();

    void limitSpeed();

    void updateFriction();

public:
    void handleInput(InputCmd cmd);

    void updateCarPhysics();

    const b2Vec2& position();

    float angle();

    void recieveDamage(uint8_t damage);

    uint8_t health();

    CarType getCarType();

    int nextCheckpointId();

    void incrementNextCheckpointId();

    float getSpeed() const;
    float getBeforeCollisionSpeed() const ;

    void storeSpeed();

    float timePenalty();
    float nextRaceTimePenalty();

    std::set<UpgradeType> applyedUpgrades() { return applied_upgrades; }

    EntityType getType() const override { return EntityType::CAR; }

    Car(b2World& world, const b2Vec2& initial_position, CarType car_type, float time_penalty);

    ~Car();
};


#endif
