#include "checkpoint.h"

Checkpoint::Checkpoint(b2World& world, const b2Vec2& position, float width, float height, int id) : id(id) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = position;

    checkpoint_body = world.CreateBody(&bodyDef);
    checkpoint_body->SetUserData( this );

    b2PolygonShape shape;
    shape.SetAsBox(width, height);

    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = true; //con esto hago que no sea solido
    fixtureDef.shape = &shape;

    checkpoint_body->CreateFixture(&fixtureDef);
}

int Checkpoint::getId(){
    return id;
}