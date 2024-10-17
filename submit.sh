#!/usr/bin/sh

set -e

make doc

LOGIN=xstigl00
mkdir $LOGIN

cp -r src n2tcap CMakeLists.txt README $LOGIN/
cp doc/manual.pdf $LOGIN/manual.pdf

printf '# Author: Jakub Antonín Štigler (xstigl00)

BUILD_TYPE=Release
CXX=g++-14.2
CMAKE_FLAGS=-DCMAKE_CXX_COMPILER=$(CXX) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
PARALEL=$(shell nproc)

.PHONY: all
all: build

.PHONY: build
build:
\tif [ ! -f build/Makefile ]; then \\
\t\tmkdir build; \\
\t\tcd build \\
\t\t\t&& cmake .. $(CMAKE_FLAGS); \\
\tfi
\tcd build && $(MAKE) -j $(PARALEL)
\tmv build/p2nprobe p2nprobe

clean:
\t-cd build && $(MAKE) clean
\t-rm p2nprobe
' > $LOGIN/Makefile

printf '
Files:
' >> $LOGIN/README

cd $LOGIN

for f in `find . -name '*'`; do
    if [ -f "$f" ]; then
        echo "  ${f#./}" >> README
    fi
done

tar cf $LOGIN.tar -- *

cd ..

mv $LOGIN/$LOGIN.tar $LOGIN.tar
rm -r $LOGIN
