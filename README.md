# Flatbuffer FFI-bridge PoC

This repo implements a simple and dirty FFI bridge PoC using Flatbuffers. The PoC tests the feasibility of creating cross-platform libraries written in rust.

The Flatbuffer FFI-bridge main objectives are:

* Isolate the complexities of FFI bindings in a core library. Ultimately, the core library should be a simple set of functions that pass immutable memory pointers back and forth.
* Enable adding and consuming new library functions with simple and complex data structures without dealing with FFI data conversions, unsafe memory access, or thread safety concerns.
* Target mobile, web, and desktop apps and games

PoC evaluation points:
* Measure Flatbuffer encoding overhead
* Evaluate Flatbuffer code generation tools

## Running the benchmarks

The project uses cmake to build and run the benchmarks. To setup the project just run `make`.

```bash
$ make
```

This command should download and build all the dependencies including the rust library.

Once the build is ready, run the benchmarks

```
# make bench
./build/Release/Linux/benchmarks/flatbuffers-test_bench --benchmark_out_format=json --benchmark_out=results.json
2023-02-26T00:54:00+00:00
Running ./build/Release/Linux/benchmarks/flatbuffers-test_bench
Run on (2 X 48 MHz CPU s)
Load Average: 0.39, 1.54, 1.34
----------------------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------------------------
BM_GetTimestamp                                 20.9 ns         20.9 ns     33069187 items_per_second=47.949M/s
BM_PositionCreation                            0.312 ns        0.311 ns   1000000000 items_per_second=3.21298G/s
BM_CreatePlayerMovedEvent                       96.0 ns         96.0 ns      7205226 items_per_second=10.4172M/s
BM_SimpleCalculateDistance                      1.24 ns         1.24 ns    558033864 items_per_second=804.037M/s
BM_SimpleCalculateDistanceInRust                1.25 ns         1.25 ns    561018815 items_per_second=801.261M/s
BM_CalculateDistanceBuffer                      3.55 ns         3.45 ns    204070627 items_per_second=290.215M/s
BM_CalculateDistanceBufferInRust                59.2 ns         59.2 ns      9184137 items_per_second=16.8857M/s
BM_PositionBatchCreation/1                       286 ns          286 ns      2447179 items_per_second=3.4949M/s
BM_PositionBatchCreation/8                      1736 ns         1737 ns       401634 items_per_second=575.87k/s
BM_PositionBatchCreation/64                    12794 ns        12794 ns        54977 items_per_second=78.1641k/s
BM_PositionBatchCreation/512                  121105 ns       121091 ns         5756 items_per_second=8.25823k/s
BM_PositionBatchCreation/2048                 499912 ns       494418 ns         1439 items_per_second=2.02258k/s
BM_PositionBatchCreation_BigO              127166.77 (1)   126065.17 (1)  
BM_PositionBatchCreation_RMS                     151 %           150 %    
BM_PositionBatchCreation/1/threads:4             138 ns          294 ns      2372476 items_per_second=3.39861M/s
BM_PositionBatchCreation/8/threads:4             875 ns         1799 ns       383696 items_per_second=555.848k/s
BM_PositionBatchCreation/64/threads:4           6477 ns        13122 ns        49548 items_per_second=76.2075k/s
BM_PositionBatchCreation/512/threads:4         51403 ns       102012 ns         6872 items_per_second=9.80277k/s
BM_PositionBatchCreation/2048/threads:4       343531 ns       533964 ns         1352 items_per_second=1.87279k/s
BM_PositionBatchCreationReuseBuilder/1           244 ns          244 ns      2870268 items_per_second=4.10171M/s
BM_PositionBatchCreationReuseBuilder/8          1633 ns         1633 ns       428934 items_per_second=612.313k/s
BM_PositionBatchCreationReuseBuilder/64        12077 ns        12075 ns        58755 items_per_second=82.8141k/s
BM_PositionBatchCreationReuseBuilder/512       93640 ns        93639 ns         7443 items_per_second=10.6793k/s
BM_PositionBatchCreationReuseBuilder/4096     747202 ns       747083 ns          919 items_per_second=1.33854k/s
BM_PositionBatchCreationReuseBuilder_BigO  170959.48 (1)   170934.80 (1)  
BM_PositionBatchCreationReuseBuilder_RMS         170 %           170 %    
BM_CreateJsonMovedEventStr                       581 ns          581 ns      1187805 items_per_second=1.72135M/s
BM_CreateJsonMovedEventBinary                    504 ns          504 ns      1335818 items_per_second=1.98363M/s
cargo -C rust-fb/ bench
    Finished bench [optimized] target(s) in 0.89s
     Running unittests src/lib.rs (target/release/deps/rust_fb-e45cde6fe5d969df)

running 5 tests
test tests::distance ... ignored
test tests::movement_distance ... ignored
test tests::bench_create_movement_event       ... bench:         214 ns/iter (+/- 131)
test tests::bench_create_movement_event_reuse ... bench:          24 ns/iter (+/- 1)
test tests::bench_slice_from_raw              ... bench:          55 ns/iter (+/- 1)

test result: ok. 0 passed; 0 failed; 2 ignored; 3 measured; 0 filtered out; finished in 1.19s

```

## VSCode Container

You can use the VSCode containers to run the project without installing the required tools. All the environment setup configuration is included in the `.devcontainer` folder.

Just execute CMD + P and `Dev Containers: Reopen in Container`

More info in [Developing inside a Container](https://code.visualstudio.com/docs/devcontainers/containers)