# Author: Jakub Antonín Štigler (xstigl00)

BUILD_TYPE=Debug
PARALEL=-j $(shell nproc)
PROBE=src/*.hpp src/*.cpp src/**/*.hpp src/**/*.cpp
CAP=n2tcap/*.hpp n2tcap/*.cpp
ALL=$(PROBE) $(CAP)

.PHONY: all
all: build doc

.PHONY: build
build:
	if [ ! -f build/Makefile ]; then \
		mkdir build; \
		cd build \
			&& cmake .. -DCMAKE_BUILD_TYPE=$(BUILD_TYPE); \
	fi
	cd build && $(MAKE) $(PARALEL)

.PHONY: doc
doc:
	cd doc && $(MAKE)

.PHONY: run
run: build
	build/p2nprobe

.PHONY: fmt
fmt:
	clang-format -i $(ALL)

.PHONY: cppcheck
cppcheck:
	cppcheck --check-level=exhaustive $(ALL)

.PHONY: tidy
tidy: build
	run-clang-tidy $(PARALEL) -use-color -quiet -p build \
		-header-filter=src/ -header-filter=n2tcap/ \
		'src/.*\.cpp' 'src/.*\.hpp' 'n2tcap/.*\.cpp' 'n2tcap/.*\.hpp'

.PHONY: tidy
check: fmt cppcheck tidy

.PHONY: submit
submit:
	./submit.sh

.PHONY: clean
clean:
	-cd build && $(MAKE) clean
	cd doc && $(MAKE) clean
