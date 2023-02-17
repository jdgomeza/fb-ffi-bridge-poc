#pragma once

#include "flatbuffers/flatbuffers.h"
#include "player_events_generated.h"
#include <stdint.h>

namespace MyGame {
uint8_t *CreatePlayerMovedBuffer(const Events::UUID &id,
                                 const Events::Point3D &ini,
                                 const Events::Point3D &end);

using PlayerMovementTuple =
    std::tuple<Events::UUID, Events::Point3D, Events::Point3D>;

uint8_t *CreateArrayPlayerMovedBuffer(std::vector<PlayerMovementTuple> players);

uint8_t *CreateArrayPlayerMovedBuffer(flatbuffers::FlatBufferBuilder &builder,
                                      std::vector<PlayerMovementTuple> players);

float distance(float x1, float y1, float z1, float x2, float y2, float z2);
float distance(const Events::Point3D &p1, const Events::Point3D &p2);

uint64_t get_ts();
}; // namespace MyGame
