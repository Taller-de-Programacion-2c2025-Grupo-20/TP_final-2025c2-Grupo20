#ifndef ENTITY_H
#define ENTITY_H

#include "../common/constants.h"

class Entity {
public:
    virtual EntityType getType() const = 0;
    virtual ~Entity() = default;
};

#endif
