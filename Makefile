CXX = g++
CXXFLAGS = -Weffc++ -Wconversion -std=c++17


all: algebra tests

algebra: build demo.o function.o
	$(CXX) $(CXXFLAGS) build/obj/demo.o build/obj/function.o -o build/algebra

tests: build tests.o function.o
	$(CXX) $(CXXFLAGS) build/obj/tests.o build/obj/function.o -o build/tests

demo.o:
	$(CXX) $(CXXFLAGS) -c src/demo.cpp -Isrc -o build/obj/demo.o

function.o:
	$(CXX) $(CXXFLAGS) -c src/function.cpp -Isrc -o build/obj/function.o

tests.o:
	$(CXX) $(CXXFLAGS) -c tests/demo.cpp -Isrc -o build/obj/tests.o

build:
	mkdir build
	mkdir build/obj

clean:
	rm -rf build
	rm config.log config.status
