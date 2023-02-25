#include <benchmark/benchmark.h>

#include "lib.h"
#include "player_events_generated.h"

namespace benchmark {
using namespace MyGame;

static void BM_PositionBatchCreation(benchmark::State &state) {
  auto numElem = state.range(0);
  std::vector<PlayerMovementTuple> positions(numElem);

  for (int i = 0; i < numElem; i++) {
    auto e = PlayerMovementTuple{MyGame::Events::UUID(12L + i * 2, 23L + i * 3),
                                 MyGame::Events::Point3D(1.0f, 2.0f, 3.0f),
                                 MyGame::Events::Point3D(1.0f, 2.0f, 3.0f)};
    positions.push_back(e);
  }

  for (auto _ : state) {
    auto playerEvents = MyGame::CreateArrayPlayerMovedBuffer(positions);
    benchmark::DoNotOptimize(playerEvents);
  }

  state.SetItemsProcessed(state.iterations());
}

static void BM_PositionBatchCreationReuseBuilder(benchmark::State &state) {
  auto numElem = state.range(0);
  std::vector<PlayerMovementTuple> positions(numElem);

  for (int i = 0; i < numElem; i++) {
    auto e = PlayerMovementTuple{MyGame::Events::UUID(12L + i * 2, 23L + i * 3),
                                 MyGame::Events::Point3D(1.0f, 2.0f, 3.0f),
                                 MyGame::Events::Point3D(1.0f, 2.0f, 3.0f)};
    positions.push_back(e);
  }

  flatbuffers::FlatBufferBuilder builder(1024);
  for (auto _ : state) {
    builder.Clear();
    auto playerEvents =
        MyGame::CreateArrayPlayerMovedBuffer(builder, positions);
    benchmark::DoNotOptimize(playerEvents);
  }

  state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_PositionBatchCreation)->Range(1, 2 << 10)->Complexity();
BENCHMARK(BM_PositionBatchCreation)->Range(1, 2 << 10)->Threads(4);
BENCHMARK(BM_PositionBatchCreationReuseBuilder)
    ->Range(1, 4 << 10)
    ->Complexity();

} // namespace benchmark
