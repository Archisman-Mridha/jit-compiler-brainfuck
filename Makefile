.PHONY: build
build:
	rm -rf build && \
    cmake -G Ninja -B build && \
    ninja -C build

.PHONY: test
test: build
	chmod +x ./build/brainfuck-compiler && \
    ./build/brainfuck-compiler ./examples/hello-world.bf
