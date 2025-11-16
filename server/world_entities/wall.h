#ifndef WALL_H
#define WALL_H

#include <Box2D/Box2D.h>

#include "../../common/constants.h"

#include "entity.h"

class Wall: public Entity {
private:
    b2Body* wall_body;

public:
    EntityType getType() const override { return EntityType::WALL; }

    Wall(b2World& world, const b2Vec2& position, float width, float height);
};


#endif
