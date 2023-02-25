#include <chrono>
#include <math.h>
#include <random>
#include <stdint.h>

#include "lib.h"
#include "player_events_generated.h"

using namespace MyGame;

ByteBuffer MyGame::CreatePlayerMovedBuffer(const Events::UUID &id,
                                           const Events::Point3D &ini,
                                           const Events::Point3D &end) {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto pm = Events::CreatePlayerMoved(builder, &id, &ini, &end);

  auto event = Events::CreatePlayerEvent(builder, get_ts(),
                                         Events::Event_PlayerMoved, pm.Union());
  builder.Finish(event);

  return ByteBuffer{builder.GetBufferPointer(), builder.GetSize()};
}

uint8_t *
MyGame::CreateArrayPlayerMovedBuffer(std::vector<PlayerMovementTuple> players) {
  flatbuffers::FlatBufferBuilder builder(1024);
  return CreateArrayPlayerMovedBuffer(builder, players);
}

uint8_t *
MyGame::CreateArrayPlayerMovedBuffer(flatbuffers::FlatBufferBuilder &builder,
                                     std::vector<PlayerMovementTuple> players) {
  std::vector<uint8_t> eventTypeVector;
  std::vector<flatbuffers::Offset<Events::PlayerEvent>> eventVector;

  for (auto playerPos : players) {
    auto pm = Events::CreatePlayerMoved(builder, &std::get<0>(playerPos),
                                        &std::get<1>(playerPos),
                                        &std::get<2>(playerPos));

    auto playerEvent = Events::CreatePlayerEvent(
        builder, get_ts(), Events::Event_PlayerMoved, pm.Union());
    eventVector.push_back(playerEvent);
  }

  auto events = builder.CreateVector(eventVector);
  auto eventBatch = Events::CreateEventBatch(builder, events);

  builder.Finish(eventBatch);

  return builder.GetBufferPointer();
}

float MyGame::distance(float x1, float y1, float z1, float x2, float y2,
                       float z2) {
  return std::sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
}

float MyGame::distance(const Events::Point3D &p1, const Events::Point3D &p2) {
  return MyGame::distance(p1.x(), p1.y(), p1.z(), p2.x(), p2.y(), p2.z());
};

uint64_t MyGame::get_ts() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
      .count();
};
