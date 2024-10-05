BUILD_TYPE=Debug
PARALEL=-j $(shell nproc)

.PHONY: build
build:
	if [ ! -f build/Makefile ]; then \
		mkdir build; \
		cd build \
			&& cmake .. -DCMAKE_BUILD_TYPE=$(BUILD_TYPE); \
	fi
	cd build && $(MAKE) $(PARALEL)

.PHONY: run
run: build
	build/p2nprobe

.PHONY: fmt
fmt:
	clang-format -i src/**/*.hpp src/**/*.cpp

.PHONY: clean
clean:
	-cd build && $(MAKE) clean
