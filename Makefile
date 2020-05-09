.PHONY: build test

format:
	echo "Formatting the code"
	find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i
	find test/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i

clean:
	echo "Cleaning builds"
	rm -rf build
  
run:
	echo "Running binary"
	./build.sh -rb

test:
	echo "Running test"
	set -a # automatically export all variables
	./build.sh -rt
	set +a

build:
	echo "Building the project"
	mkdir -p build
	cd build && cmake -GNinja -DCMAKE_BUILD_TYPE=Release .. && ninja -j`nproc`

docker:
	echo "Building docker"
	docker build -t dawncpp:1.0.0 -f Dockerfile .
