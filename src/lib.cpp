#include "lib.h"
#include "player_events_generated.h"
#include <math.h>
#include <random>
#include <stdint.h>
#include <chrono>

using namespace MyGame;

uint8_t *MyGame::CreatePlayerMovedBuffer(const Events::UUID &id,
                                         const Events::Point3D &ini,
                                         const Events::Point3D &end) {
  flatbuffers::FlatBufferBuilder builder(1024);
  auto pm = Events::CreatePlayerMoved(builder, &id, &ini, &end);

  auto event = Events::CreatePlayerEvent(builder, get_ts(),
                                         Events::Event_PlayerMoved, pm.Union());
  builder.Finish(event);

  return builder.GetBufferPointer();
}

uint8_t *MyGame::CreateArrayPlayerMovedBuffer(
    std::vector<std::tuple<Events::UUID, Events::Point3D, Events::Point3D>>
        players) {
  flatbuffers::FlatBufferBuilder builder(1024);
  return CreateArrayPlayerMovedBuffer(builder, players);
}

uint8_t *MyGame::CreateArrayPlayerMovedBuffer(
    flatbuffers::FlatBufferBuilder &builder,
    std::vector<std::tuple<Events::UUID, Events::Point3D, Events::Point3D>>
        players) {
  std::vector<uint8_t> eventTypeVector;
  std::vector<flatbuffers::Offset<void>> eventVector;

  for (auto playerPos : players) {
    auto pm = Events::CreatePlayerMoved(builder, &std::get<0>(playerPos),
                                        &std::get<1>(playerPos),
                                        &std::get<2>(playerPos));
    eventVector.push_back(pm.Union());
    eventTypeVector.push_back(Events::Event_PlayerMoved);
  }

  auto eventBatch = builder.CreateVector(eventVector);
  auto eventBatchTypes = builder.CreateVector(eventTypeVector);

  auto playerEventBuilder = Events::PlayerEventBuilder(builder);
  playerEventBuilder.add_ts(get_ts());
  playerEventBuilder.add_event_batch(eventBatch);
  playerEventBuilder.add_event_batch_type(eventBatchTypes);
  builder.Finish(playerEventBuilder.Finish());

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
