#include "car.h"
#include <iostream>

b2Vec2 Car::getLateralVelocity() {
    b2Vec2 rightNormal = car_body->GetWorldVector(b2Vec2(1,0));
    return b2Dot(rightNormal, car_body->GetLinearVelocity()) * rightNormal;
}

b2Vec2 Car::getForwardVelocity() {
    b2Vec2 forwardNormal = car_body->GetWorldVector(b2Vec2(0,1));
    return b2Dot(forwardNormal, car_body->GetLinearVelocity()) * forwardNormal;
}

void Car::limitSpeed() {
    b2Vec2 fwdVel = getForwardVelocity();
    float speed = fwdVel.Length();
    if (speed > max_speed) {
        fwdVel *= max_speed / speed;
        b2Vec2 lateral = getLateralVelocity();
        car_body->SetLinearVelocity(fwdVel + lateral);
    }
}

void Car::updateFriction() {
    float lateralFrictionFactor = 0.95f;
    float angularDamping = 0.05f;

    b2Vec2 lateralVel = getLateralVelocity();
    b2Vec2 lateralImpulse = -(lateralFrictionFactor * car_body->GetMass()) * lateralVel;
    car_body->ApplyLinearImpulse(lateralImpulse, car_body->GetWorldCenter());

    car_body->SetAngularVelocity(car_body->GetAngularVelocity() * (1.0f - angularDamping));
}

void Car::handleInput(InputCmd cmd) {

    if (cmd.key == InputKey::Up){
        if (cmd.action == InputAction::Press){
            accelerating = true;
            std::cout << "Apreto acelerador\n";
        }

        if (cmd.action == InputAction::Release){
            accelerating = false;
            std::cout << "Suelto acelerador\n";
        }
    }

    if (cmd.key == InputKey::Down){
        if (cmd.action == InputAction::Press){
            braking = true;
            std::cout << "Apreto freno\n";
        }

        if (cmd.action == InputAction::Release){
            braking = false;
            std::cout << "Solto freno\n";
        }
    }

    if (cmd.key == InputKey::Left){
        if (cmd.action == InputAction::Press){
            turningLeft = true;
            std::cout << "Dobla a la izquierda\n";
        }

        if (cmd.action == InputAction::Release){
            turningLeft = false;
            std::cout << "Deja de doblar a la izquierda\n";
        }
    }

    if (cmd.key == InputKey::Right){
        if (cmd.action == InputAction::Press){
            turningRight = true;
            std::cout << "Dobla a la derecha\n";
        }

        if (cmd.action == InputAction::Release){
            turningRight = false;
            std::cout << "Deja de doblar a la derecha\n";
        }
    }

}

void Car::updateCarPhysics() {
    b2Vec2 forward = car_body->GetWorldVector(b2Vec2(0,1));
    b2Vec2 force(0,0);
    float torque = 0.f;

    // Aplico fuerza de aceleración o frenado
    if (accelerating) { 
        force = accelaration * forward;
    } 
    
    else if (braking) { 
        force = -accelaration * forward;
    }

    else {
        // Aplico freno si no hay input, por rozamiento con el piso
        b2Vec2 fwdVel = getForwardVelocity();
        float brakingFactor = 0.01f;
        b2Vec2 brakeImpulse = -(brakingFactor * car_body->GetMass()) * fwdVel;
        car_body->ApplyLinearImpulse(brakeImpulse, car_body->GetWorldCenter());
    }

    car_body->ApplyForce(force, car_body->GetWorldCenter());

    float dir = b2Dot(forward, car_body->GetLinearVelocity()) >= 0.f ? 1.f : -1.f;

    // Gira solo si hay velocidad longitudinal
    b2Vec2 fwdVel = getForwardVelocity();
    float forwardSpeed = fwdVel.Length();
    if (forwardSpeed > 1.5f) {
        if (turningLeft)  torque -= rotation_torque * dir;
        if (turningRight) torque += rotation_torque * dir;
        car_body->ApplyTorque(torque);
    }

    // Elimino velocidad lateral y limito velocidad
    updateFriction();
    limitSpeed();
}

const b2Vec2& Car::position() {
    return car_body->GetPosition();
}

float Car::angle() {
    return car_body->GetAngle();
}

void Car::recieveDamage(uint8_t damage) {
    if ( (car_health - damage) <= 0) {
        car_health = 0;
        return;
    }

    car_health -= damage;
}

uint8_t Car::health(){
    return car_health;
}

float Car::getSpeed() const {
    return car_body->GetLinearVelocity().Length(); // m/s
}

Car::Car(b2World& world, const b2Vec2& initial_position) {
    b2BodyDef carDef;
    carDef.type = b2_dynamicBody;
    carDef.position.Set(initial_position.x, initial_position.y);
    carDef.angle = 0.f;
    carDef.angularDamping = 1.0f;
    carDef.linearDamping = 0.1f;
    
    car_body = world.CreateBody(&carDef);
    car_body->SetUserData( this );

    b2PolygonShape car_shape; 
    car_shape.SetAsBox(0.5f, 1.f);
    car_body->CreateFixture(&car_shape,1.f);
}