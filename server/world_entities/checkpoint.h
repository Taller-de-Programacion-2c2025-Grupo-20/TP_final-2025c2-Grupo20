#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#include <Box2D/Box2D.h>

#include "entity.h"

#include "../../common/clientCommand.h"
#include "../../common/constants.h"

#include <string>

class Checkpoint : public Entity {
private:

    int id;

    b2Body* checkpoint_body;

public:

    EntityType getType() const override { return EntityType::CHECKPOINT; }

    int getId();

    Checkpoint(b2World& world, const b2Vec2& position, float width, float height, int id);
};


#endif