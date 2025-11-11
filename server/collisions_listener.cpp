#include "collisions_listener.h"

#include <iostream>

#include <Box2D/Box2D.h>
#include "world_entities/entity.h"

#include "../common/constants.h"

void CollisionsListener::handlerCollisionCarAndCar(Car* carA, Car* carB, const b2ContactImpulse* impulse){
    //std::cout << "2 Autos colisionaron\n";

    float total_impulse = 0;
    total_impulse += impulse->normalImpulses[0];
    total_impulse += impulse->normalImpulses[1];

    //std::cout << "TotalImpulse: " << total_impulse << "\n";

    if (total_impulse < damage_minimum)
        return;

    uint8_t damage = static_cast<uint8_t>(std::clamp(total_impulse * 0.5f, 0.0f, 255.0f));
    carA->recieveDamage(damage);
    carB->recieveDamage(damage);

    std::cout << "2 Autos colisionaron\n";
    std::cout << "Daño recibido: " << (total_impulse * 0.5) << "\n";
    std::cout << "Vida restante autos: " << static_cast<int>(carA->health()) << "\n";

}

void CollisionsListener::handlerCollisionCarAndWall(Car* car, const b2ContactImpulse* impulse){
    //std::cout << "Auto colisiono con una pared\n";

    float total_impulse = 0;
    total_impulse += impulse->normalImpulses[0];
    total_impulse += impulse->normalImpulses[1];

    //std::cout << "TotalImpulse: " << total_impulse << "\n";

    if (total_impulse < damage_minimum)
        return;

    uint8_t damage = static_cast<uint8_t>(std::clamp(total_impulse * 0.5f, 0.0f, 255.0f));
    car->recieveDamage(damage);

    std::cout << "Auto colisiono con una pared\n";
    std::cout << "Daño recibido: " << (total_impulse * 0.5) << "\n";
    std::cout << "Vida restante auto: " << static_cast<int>(car->health()) << "\n";

}

void CollisionsListener::handlerCollisionCarAndCheckpoint(Car* car, Checkpoint* checkpoint){
    std::cout << "Auto colisiono con un checkpoint\n";
    if (car){

    }

    if(checkpoint){

    }
}

void CollisionsListener::BeginContact(b2Contact* contact){
    Entity* objectA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* objectB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    if (!objectA || !objectB){
        return;
    }

    if (objectA->getType() == EntityType::CAR && objectB->getType() == EntityType::CHECKPOINT){
        Car* car = static_cast<Car*>(objectA);
        Checkpoint* checkpoint = static_cast<Checkpoint*>(objectB);
        handlerCollisionCarAndCheckpoint(car, checkpoint);
    }

    if (objectA->getType() == EntityType::CHECKPOINT && objectB->getType() == EntityType::CAR){
        Car* car = static_cast<Car*>(objectB);
        Checkpoint* checkpoint = static_cast<Checkpoint*>(objectA);
        handlerCollisionCarAndCheckpoint(car, checkpoint);
    }
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

    if (objectA->getType() == EntityType::CAR && objectB->getType() == EntityType::WALL){
        Car* car = static_cast<Car*>(objectA);
        handlerCollisionCarAndWall(car, impulse);
    }

    if (objectA->getType() == EntityType::WALL && objectB->getType() == EntityType::CAR){
        Car* car = static_cast<Car*>(objectB);
        handlerCollisionCarAndWall(car, impulse);
    }
}