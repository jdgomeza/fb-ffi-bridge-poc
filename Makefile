PLATFORM:= $(shell uname)

# Adding the platform to the build path avoids cmake conflicts when using vscode containers
BUILD_PATH:= ./build/Release/$(PLATFORM)

all: config flatc codegen build
	
config:
	cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -S . -B ./build/Release/${PLATFORM} -G "Unix Makefiles"

build:
	cmake --build $(BUILD_PATH) --config Release --target flatbuffers-test_bench -j 12 -- 

flatc:
	cmake --build $(BUILD_PATH) --config Release --target flatc -j 12 -- 

clean:
	rm -rf rust-fb/target
	rm -rf build

bench:
	$(BUILD_PATH)/benchmarks/flatbuffers-test_bench --benchmark_out_format=json --benchmark_out=results.json
	cargo -C rust-fb/ bench

codegen:
	$(info *** Generating flatbuffer code ***)
	$(BUILD_PATH)/_deps/flatbuffers-build/flatc --cpp --gen-name-strings --gen-object-api -o ./src fbs/player_events.fbs
	$(BUILD_PATH)/_deps/flatbuffers-build/flatc --rust -o ./rust-fb/src fbs/player_events.fbs
