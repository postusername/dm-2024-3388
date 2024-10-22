all: algebra tests

algebra: build demo.o function.o
	g++ build/obj/demo.o build/obj/function.o -o build/algebra

tests: build tests.o function.o
	g++ build/obj/tests.o build/obj/function.o -o build/tests

demo.o:
	g++ -c src/demo.cpp -Isrc -o build/obj/demo.o

function.o:
	g++ -c src/function.cpp -Isrc -o build/obj/function.o

tests.o:
	g++ -c tests/demo.cpp -Isrc -o build/obj/tests.o

build:
	mkdir build
	mkdir build/obj

clean:
	rm -rf build
