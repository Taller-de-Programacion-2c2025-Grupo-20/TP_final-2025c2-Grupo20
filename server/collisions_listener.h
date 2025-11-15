#ifndef COLLISIONS_LISTENER_H
#define COLLISIONS_LISTENER_H

#include <Box2D/Box2D.h>

#include "world_entities/car.h"
#include "world_entities/wall.h"
#include "world_entities/checkpoint.h"

class CollisionsListener: public b2ContactListener {
private:

    const float damage_minimum = 0.4f;

    void handlerCollisionCarAndCar(Car* carA, Car* carB, const b2ContactImpulse* impulse, b2Contact* contact);

    void handlerCollisionCarAndWall(Car* car, const b2ContactImpulse* impulse, b2Contact* contact);

    void handlerCollisionCarAndCheckpoint(Car* car, Checkpoint* checkpoint);

public:
    void BeginContact(b2Contact* contact) override;

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};


#endif
