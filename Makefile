.PHONY: debug release profile tsan test clean run

VCPKG_ROOT ?= $(HOME)/.vcpkg
VCPKG_TOOLCHAIN := $(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake

$(VCPKG_TOOLCHAIN):
	@echo "[vcpkg] Toolchain not found. Bootstrapping vcpkg into $(VCPKG_ROOT)..."
	@git clone https://github.com/microsoft/vcpkg.git $(VCPKG_ROOT)
	@$(VCPKG_ROOT)/bootstrap-vcpkg.sh

debug: $(VCPKG_TOOLCHAIN)
	@cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_TOOLCHAIN)"
	@cmake --build build/debug
	@ln -sf build/debug/compile_commands.json .

release: $(VCPKG_TOOLCHAIN)
	@cmake -B build/release -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_TOOLCHAIN)"
	@cmake --build build/release
	@ln -sf build/release/compile_commands.json .

profile: $(VCPKG_TOOLCHAIN)
	@cmake -B build/profile -DCMAKE_BUILD_TYPE=Profile -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_TOOLCHAIN)"
	@cmake --build build/profile
	@ln -sf build/profile/compile_commands.json .

tsan: $(VCPKG_TOOLCHAIN)
	@cmake -B build/tsan -DCMAKE_BUILD_TYPE=TSan -DCMAKE_TOOLCHAIN_FILE="$(VCPKG_TOOLCHAIN)"
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

