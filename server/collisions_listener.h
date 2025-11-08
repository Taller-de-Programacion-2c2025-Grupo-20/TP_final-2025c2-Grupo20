#ifndef COLLISIONS_LISTENER_H
#define COLLISIONS_LISTENER_H

#include <Box2D/Box2D.h>

class CollisionsListener: public b2ContactListener {
private:
public:
    // void BeginContact(b2Contact* contact) override;

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};


#endif
