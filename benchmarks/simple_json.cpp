#include <benchmark/benchmark.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace benchmark {

    static void BM_CreateJsonMovedEventStr(benchmark::State& state) {
        json j;
        j["pos1"] = { {"x",1.0f},{"y" , 2.0f},{"z" , 3.0f} };
        j["pos2"] = { {"x",1.0f},{"y" , 2.0f},{"z" , 3.0f} };
        j["uuid"] = json::array({ 12L,23L });

        for (auto _ : state) {
            auto event = j.dump();
            benchmark::DoNotOptimize(event);
        }
        state.SetItemsProcessed(state.iterations());
    }

    static void BM_CreateJsonMovedEventBinary(benchmark::State& state) {
        json j;
        j["pos1"] = { {"x",1.0f},{"y" , 2.0f},{"z" , 3.0f} };
        j["pos2"] = { {"x",1.0f},{"y" , 2.0f},{"z" , 3.0f} };
        j["uuid"] = json::array({ 12L,23L });

        for (auto _ : state) {
            auto buf = json::to_ubjson(j);
            benchmark::DoNotOptimize(buf);
        }
        state.SetItemsProcessed(state.iterations());
    }

    BENCHMARK(BM_CreateJsonMovedEventStr);
    BENCHMARK(BM_CreateJsonMovedEventBinary);

}
