.PHONY: debug release profile tsan test clean run

debug:
	@cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"
	@cmake --build build/debug
	@ln -sf build/debug/compile_commands.json .

release:
	@cmake -B build/release -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"
	@cmake --build build/release
	@ln -sf build/release/compile_commands.json .

profile:
	@cmake -B build/profile -DCMAKE_BUILD_TYPE=Profile -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"
	@cmake --build build/profile
	@ln -sf build/profile/compile_commands.json .

tsan:
	@cmake -B build/tsan -DCMAKE_BUILD_TYPE=TSan -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake"
	@cmake --build build/tsan
	@ln -sf build/tsan/compile_commands.json .

test:
	@cmake --build build/debug
	@ctest --test-dir build/debug --verbose

clean:
	@rm -rf build/ bin/ lib/

## local test
run:
	./bin/cppdf /Users/masteguin/Documents/Books/C-KR2.pdf
