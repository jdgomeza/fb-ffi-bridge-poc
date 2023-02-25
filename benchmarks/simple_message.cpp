#include "bindings.h"
#include "lib.h"
#include "player_events_generated.h"
#include <benchmark/benchmark.h>

using VOffsetT = int8_t;

namespace benchmark {

static void BM_GetTimestamp(benchmark::State &state) {
  for (auto _ : state) {
    auto ts = MyGame::get_ts();
    benchmark::DoNotOptimize(ts);
  }
  state.SetItemsProcessed(state.iterations());
}

static void BM_PositionCreation(benchmark::State &state) {
  for (auto _ : state) {
    auto pos1 = MyGame::Events::Point3D(1.0f, 2.0f, 3.0f);
    auto pos2 = MyGame::Events::Point3D(1.0f, 2.0f, 3.0f);
    auto uuid = MyGame::Events::UUID(12L, 23L);
    benchmark::DoNotOptimize(uuid);
  }
  state.SetItemsProcessed(state.iterations());
}

static void BM_CreatePlayerMovedEvent(benchmark::State &state) {
  auto pos1 = MyGame::Events::Point3D(1.0f, 2.0f, 3.0f);
  auto pos2 = MyGame::Events::Point3D(1.0f, 2.0f, 3.0f);
  auto uuid = MyGame::Events::UUID(12L, 23L);
  for (auto _ : state) {
    auto event = MyGame::CreatePlayerMovedBuffer(uuid, pos1, pos2);
    benchmark::DoNotOptimize(event);
  }
  state.SetItemsProcessed(state.iterations());
}

static void BM_SimpleCalculateDistance(benchmark::State &state) {
  auto pos1 = MyGame::Events::Point3D(1.0f, 2.0f, 3.0f);
  auto pos2 = MyGame::Events::Point3D(1.0f, 2.0f, 3.0f);
  auto uuid = MyGame::Events::UUID(12L, 23L);

  for (auto _ : state) {
    auto distance = MyGame::distance(pos1, pos2);
    benchmark::DoNotOptimize(distance);
  }
  state.SetItemsProcessed(state.iterations());
}

static void BM_SimpleCalculateDistanceInRust(benchmark::State &state) {
  auto pos1 = MyGame::Events::Point3D(1.0f, 2.23f, 3.0f);
  auto pos2 = MyGame::Events::Point3D(1.0f, -0.43f, 3.0f);
  auto uuid = MyGame::Events::UUID(12L, 23L);

  for (auto _ : state) {
    auto distance = distance_points(pos1.x(), pos1.y(), pos1.z(), pos2.x(),
                                    pos2.y(), pos2.z());
    benchmark::DoNotOptimize(distance);
  }
  state.SetItemsProcessed(state.iterations());
}

static void BM_CalculateDistanceBuffer(benchmark::State &state) {
  auto pos1 = MyGame::Events::Point3D(1.0f, 2.0f, 3.0f);
  auto pos2 = MyGame::Events::Point3D(1.0f, 2.0f, 3.0f);
  auto uuid = MyGame::Events::UUID(12L, 23L);
  auto eventPtr = MyGame::CreatePlayerMovedBuffer(uuid, pos1, pos2);

  for (auto _ : state) {
    auto event = MyGame::Events::GetPlayerEvent(eventPtr.data);
    benchmark::DoNotOptimize(event);
    if (event->event_type() == MyGame::Events::Event_PlayerMoved) {
      auto playerMoved = event->event_as_PlayerMoved();
      auto p1 = playerMoved->pos1();
      auto p2 = playerMoved->pos2();
      auto distance = MyGame::distance(*p1, *p2);
      benchmark::DoNotOptimize(distance);
    }
  }
  state.SetItemsProcessed(state.iterations());
}

static void BM_CalculateDistanceBufferInRust(benchmark::State &state) {
  auto pos1 = MyGame::Events::Point3D(1.0f, 2.0f, 3.0f);
  auto pos2 = MyGame::Events::Point3D(1.0f, 2.0f, 3.5f);
  auto uuid = MyGame::Events::UUID(12L, 23L);
  auto eventPtr = MyGame::CreatePlayerMovedBuffer(uuid, pos1, pos2);

  for (auto _ : state) {
    auto dist = calculate_event_distance(eventPtr.data, eventPtr.size);
    if (dist != 0.5) {
      state.SkipWithError(("Wrong distance " + std::to_string(dist)).c_str());
      break;
    }
    benchmark::DoNotOptimize(dist);
  }
  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_GetTimestamp);
BENCHMARK(BM_PositionCreation);
BENCHMARK(BM_CreatePlayerMovedEvent);
BENCHMARK(BM_SimpleCalculateDistance);
BENCHMARK(BM_SimpleCalculateDistanceInRust);
BENCHMARK(BM_CalculateDistanceBuffer);
BENCHMARK(BM_CalculateDistanceBufferInRust);

} // namespace benchmark

BENCHMARK_MAIN();
