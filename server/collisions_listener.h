#ifndef COLLISIONS_LISTENER_H
#define COLLISIONS_LISTENER_H

#include <Box2D/Box2D.h>
#include "car.h"

class CollisionsListener: public b2ContactListener {
private:

    const float damage_minimum = 0.4f;

    void handlerCollisionCarAndCar(Car* carA, Car* carB, const b2ContactImpulse* impulse);

public:
    // void BeginContact(b2Contact* contact) override;

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};


#endif
