.PHONY: debug release profile tsan test clean

debug:
	@cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug
	@cmake --build build/debug

release:
	@cmake -B build/release -DCMAKE_BUILD_TYPE=Release
	@cmake --build build/release

profile:
	@cmake -B build/profile -DCMAKE_BUILD_TYPE=Profile
	@cmake --build build/profile

tsan:
	@cmake -B build/tsan -DCMAKE_BUILD_TYPE=TSan
	@cmake --build build/tsan

test:
	@cmake --build build/debug
	@ctest --test-dir build/debug --output-on-failure

clean:
	@rm -rf build/ bin/
