.PHONY: all debug release clean

all: debug

debug:
	@echo "--- building mode: DEBUG ---"
	@sh ./scripts/build.sh Debug

release:
	@echo "-- building mode: RELEASE ---"
	@sh ./scripts/build.sh Release

clean:
	@rm -rf build
	@rm -rf bin
