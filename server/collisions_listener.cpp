#include "collisions_listener.h"

#include <iostream>

#include <Box2D/Box2D.h>
#include "entity.h"

#include "../common/constants.h"

void CollisionsListener::handlerCollisionCarAndCar(Car* carA, Car* carB, const b2ContactImpulse* impulse){
    std::cout << "2 Autos colisionaron\n";

    float total_impulse = 0;
    total_impulse += impulse->normalImpulses[0];
    total_impulse += impulse->normalImpulses[1];

    std::cout << "TotalImpulse: " << total_impulse << "\n";

    if (total_impulse < damage_minimum)
        return;

    uint8_t damage = static_cast<uint8_t>(std::clamp(total_impulse * 0.5f, 0.0f, 255.0f));
    carA->recieveDamage(damage);
    carB->recieveDamage(damage);

    std::cout << "Daño recibido: " << (total_impulse * 0.5) << "\n";
    std::cout << "Vida restante autos: " << static_cast<int>(carA->health()) << "\n";

}

void CollisionsListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    
    Entity* objectA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* objectB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    if (!objectA || !objectB){
        return;
    }

    if (objectA->getType() == EntityType::CAR && objectB->getType() == EntityType::CAR){
        Car* carA = static_cast<Car*>(objectA);
        Car* carB = static_cast<Car*>(objectB);
        handlerCollisionCarAndCar(carA, carB, impulse);
    }



    if (impulse)
        std::cout << "";
}