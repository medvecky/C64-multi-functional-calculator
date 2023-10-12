multicalc: build build/multicalc.o build/system_helper.o build/FP.o
	cl65 build/multicalc.o build/system_helper.o build/FP.o -o bin/multicalc.prg

build/multicalc.o: src/multicalc.c 
	cl65 -c src/multicalc.c 
	@mv src/multicalc.o build/multicalc.o

build/system_helper.o: src/modules/system_helper.c
	cl65 -c src/modules/system_helper.c 
	@mv src/modules/system_helper.o build/system_helper.o

build/FP.o: src/modules/FP.c
	cl65 -c src/modules/FP.c 
	@mv src/modules/FP.o build/FP.o

build:
	@mkdir -p build
	@mkdir -p bin

clean:
	@rm -rf build
	@rm -rf bin