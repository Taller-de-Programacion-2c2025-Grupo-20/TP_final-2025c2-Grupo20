#ifndef COLLISIONS_LISTENER_H
#define COLLISIONS_LISTENER_H

#include <Box2D/Box2D.h>

#include "world_entities/car.h"
#include "world_entities/checkpoint.h"
#include "world_entities/wall.h"

class CollisionsListener: public b2ContactListener {
private:
    const float damage_minimum = 0.4f;

    void handlerBeforeCollisionCarAndWall(Car* car);
    void handlerBeforeCollisionCarAndCar(Car* carA, Car* carB);

    void handlerCollisionCarAndCar(Car* carA, Car* carB);

    void handlerCollisionCarAndWall(Car* car);

    void handlerCollisionCarAndCheckpoint(Car* car, Checkpoint* checkpoint);

public:
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

    void BeginContact(b2Contact* contact) override;

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};


#endif
