.PHONY: build
build:
	rm -rf build && \
    cmake -G Ninja -B build && \
    ninja -C build

.PHONY: test-run
test-run:
	chmod +x ./build/brainfuck-compiler && \
    ./build/brainfuck-compiler ./examples/hello-world.bf
