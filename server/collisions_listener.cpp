#include "collisions_listener.h"

#include <iostream>

#include <Box2D/Box2D.h>
#include "entity.h"

#include "../common/constants.h"

void CollisionsListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    
    Entity* objectA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* objectB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    if (!objectA || !objectB){
        return;
    }

    if (objectA->getType() == EntityType::CAR && objectB->getType() == EntityType::CAR){
        std::cout << "Son 2 Autos que chocaron\n";
    }



    if (impulse)
        std::cout << "";
}