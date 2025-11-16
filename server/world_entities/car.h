#ifndef CAR_H
#define CAR_H

#include <string>

#include <Box2D/Box2D.h>

#include "../../common/clientCommand.h"
#include "../../common/constants.h"

#include "entity.h"

class Car: public Entity {
private:
    float accelaration = 50.f;
    float rotation_torque = 10.f;
    float max_speed = 10.f;

    bool accelerating = false;
    bool braking = false;
    bool turningLeft = false;
    bool turningRight = false;

    b2Body* car_body = nullptr;

    uint8_t car_health = 100;

    int next_checkpoint_id = 0;

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

    int nextCheckpointId();

    void incrementNextCheckpointId();

    float getSpeed() const;

    EntityType getType() const override { return EntityType::CAR; }

    Car(b2World& world, const b2Vec2& initial_position);
};


#endif
