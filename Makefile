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

PROTOC=/usr/bin/protoc
PROTOS_PATH=protos
GRPC_OUT_CPP=src/protos/

GRPC_CPP_PLUGIN_PATH=/usr/bin/grpc_cpp_plugin
GRPC_PYTHON_PLUGIN_PATH=/usr/bin/grpc_python_plugin
PROTO_IN=protos/*.proto
PYTHON_PROTOS=/home/heiseish/Projects/DawnPy/protos
PYTHON_PROTO_OUT=/home/heiseish/Projects/DawnPy/src/protobuf
protoc: 
	cp -v $(PROTO_IN) $(PYTHON_PROTOS)
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=$(GRPC_OUT_CPP) \
		--plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) \
			$(PROTO_IN)
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=$(GRPC_OUT_CPP) $(PROTO_IN)

	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=$(PYTHON_PROTO_OUT) \
		--plugin=protoc-gen-grpc=$(GRPC_PYTHON_PLUGIN_PATH) \
			$(PROTO_IN)
	$(PROTOC) -I $(PROTOS_PATH) --python_out=$(PYTHON_PROTO_OUT) $(PROTO_IN)
