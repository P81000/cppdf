.PHONY: debug release profile tsan test clean run

debug:
	@cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"
	@cmake --build build/debug

release:
	@cmake -B build/release -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"
	@cmake --build build/release

profile:
	@cmake -B build/profile -DCMAKE_BUILD_TYPE=Profile -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"
	@cmake --build build/profile

tsan:
	@cmake -B build/tsan -DCMAKE_BUILD_TYPE=TSan -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"
	@cmake --build build/tsan

test:
	@cmake --build build/debug
	@ctest --test-dir build/debug --output-on-failure

clean:
	@rm -rf build/ bin/

run:
	./bin/cppdf
