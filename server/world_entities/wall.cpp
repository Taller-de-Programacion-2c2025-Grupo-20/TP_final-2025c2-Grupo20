#include "wall.h"

Wall::Wall(b2World& world, const b2Vec2& position, float width, float height) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position = position;

    wall_body = world.CreateBody(&bodyDef);
    wall_body->SetUserData( this );

    b2PolygonShape shape;
    shape.SetAsBox(width, height);

    wall_body->CreateFixture(&shape, 0.0f);
}