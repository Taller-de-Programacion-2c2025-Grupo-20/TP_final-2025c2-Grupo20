#ifndef CAR_H
#define CAR_H

#include <string>

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

    bool accelerating;
    bool braking;
    bool turningLeft;
    bool turningRight;

    int next_checkpoint_id = 0;

    b2Body* car_body;

    uint8_t car_health;

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

    EntityType getType() const override { return EntityType::CAR; }

    Car(b2World& world, const b2Vec2& initial_position);
    Car(b2World& world, const b2Vec2& initial_position, CarType car_type);
};


#endif
