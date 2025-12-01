#include "collisions_listener.h"

#include <iostream>

#include <Box2D/Box2D.h>

#include "../common/constants.h"
#include "world_entities/entity.h"

void CollisionsListener::handlerCollisionCarAndCar(Car* carA, Car* carB) {

    float delta_speed_A = carA->getBeforeCollisionSpeed() - carA->getSpeed();
    float delta_speed_B = carB->getBeforeCollisionSpeed() - carB->getSpeed();

    if (delta_speed_A < 0.3f && delta_speed_B < 0.3f) //ignoro roces
        return;

    float rawDamageA = 0.0f;
    float rawDamageB = 0.0f;

    if (delta_speed_A > delta_speed_B){
        rawDamageA = std::max(0.0f, delta_speed_A * 1.0f);
        rawDamageB = std::max(0.0f, delta_speed_A * 1.0f * 0.5f);
    }

    if (delta_speed_B > delta_speed_A){
        rawDamageA = std::max(0.0f, delta_speed_B * 1.0f * 0.5f);
        rawDamageB = std::max(0.0f, delta_speed_B * 1.0f);
    }

    uint8_t damageA = (uint8_t) std::clamp(rawDamageA, 0.0f, 255.0f);
    uint8_t damageB = (uint8_t) std::clamp(rawDamageB, 0.0f, 255.0f);

    carA->recieveDamage(damageA);
    carB->recieveDamage(damageB);

    std::cout << "2 Autos colisionaron\n";
    std::cout << "Daño recibido: " << static_cast<int>(damageA) << "\n";
    std::cout << "Vida restante autos: " << static_cast<int>(carA->health()) << "\n";
}

void CollisionsListener::handlerCollisionCarAndWall(Car* car) {


    float last_speed = car->getBeforeCollisionSpeed();
    float current_speed = car->getSpeed();

    float delta_speed = last_speed - current_speed;

    if (delta_speed < 0.3f) { //Para cuando rozo con una pared
        return;
    }

    float raw_damage = delta_speed * 1.0f;

    uint8_t damage = static_cast<uint8_t>(
        std::clamp(raw_damage, 0.0f, 255.0f)
    );

    car->recieveDamage(damage);

    std::cout << "Auto colisiono con una pared\n";
    std::cout << "Daño recibido: " << static_cast<int>(damage) << "\n";
    std::cout << "Vida restante auto: " << static_cast<int>(car->health()) << "\n";
}

void CollisionsListener::handlerCollisionCarAndCheckpoint(Car* car, Checkpoint* checkpoint) {
    if (car->nextCheckpointId() == checkpoint->getId()) {
        car->incrementNextCheckpointId();

        std::cout << "ALERTA: Auto colisiono con el checkpoint de id: " << checkpoint->getId()
                  << "\n";
    }
}

void CollisionsListener::handlerBeforeCollisionCarAndWall(Car* car) {
    car->storeSpeed();
}

void CollisionsListener::handlerBeforeCollisionCarAndCar(Car* carA, Car* carB) {
    carA->storeSpeed();
    carB->storeSpeed();
}


void CollisionsListener::BeginContact(b2Contact* contact) {
    Entity* objectA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* objectB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    if (!objectA || !objectB) {
        return;
    }

    if (objectA->getType() == EntityType::CAR && objectB->getType() == EntityType::CHECKPOINT) {
        Car* car = static_cast<Car*>(objectA);
        Checkpoint* checkpoint = static_cast<Checkpoint*>(objectB);
        handlerCollisionCarAndCheckpoint(car, checkpoint);
    }

    if (objectA->getType() == EntityType::CHECKPOINT && objectB->getType() == EntityType::CAR) {
        Car* car = static_cast<Car*>(objectB);
        Checkpoint* checkpoint = static_cast<Checkpoint*>(objectA);
        handlerCollisionCarAndCheckpoint(car, checkpoint);
    }
}

void CollisionsListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {

    if (impulse) {
        
    }

    Entity* objectA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* objectB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    if (!objectA || !objectB) {
        return;
    }

    if (objectA->getType() == EntityType::CAR && objectB->getType() == EntityType::CAR) {
        Car* carA = static_cast<Car*>(objectA);
        Car* carB = static_cast<Car*>(objectB);
        handlerCollisionCarAndCar(carA, carB);
    }

    if (objectA->getType() == EntityType::CAR && objectB->getType() == EntityType::WALL) {
        Car* car = static_cast<Car*>(objectA);
        handlerCollisionCarAndWall(car);
    }

    if (objectA->getType() == EntityType::WALL && objectB->getType() == EntityType::CAR) {
        Car* car = static_cast<Car*>(objectB);
        handlerCollisionCarAndWall(car);
    }
}

void CollisionsListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {

    if (oldManifold){

    }

    Entity* objectA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* objectB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetBody()->GetUserData());

    if (!objectA || !objectB) {
        return;
    }

    if (objectA->getType() == EntityType::CAR && objectB->getType() == EntityType::CAR) {
        Car* carA = static_cast<Car*>(objectA);
        Car* carB = static_cast<Car*>(objectB);
        handlerBeforeCollisionCarAndCar(carA, carB);
    }

    if (objectA->getType() == EntityType::CAR && objectB->getType() == EntityType::WALL) {
        Car* car = static_cast<Car*>(objectA);
        handlerBeforeCollisionCarAndWall(car);
    }

    if (objectA->getType() == EntityType::WALL && objectB->getType() == EntityType::CAR) {
        Car* car = static_cast<Car*>(objectB);
        handlerBeforeCollisionCarAndWall(car);
    }
}
