multicalc: build build/multicalc.o build/system_helper.o build/math_helper.o build/misc_helper.o build/adt_stack.o
	cl65 build/multicalc.o build/system_helper.o build/math_helper.o build/misc_helper.o build/adt_stack.o -o bin/multicalc.prg

build/multicalc.o: src/multicalc.c 
	cl65 -c src/multicalc.c 
	@mv src/multicalc.o build/multicalc.o

build/system_helper.o: src/modules/system_helper.c
	cl65 -c src/modules/system_helper.c 
	@mv src/modules/system_helper.o build/system_helper.o

build/math_helper.o: src/modules/math_helper.c
	cl65 -c src/modules/math_helper.c 
	@mv src/modules/math_helper.o build/math_helper.o

build/misc_helper.o: src/modules/misc_helper.c
	cl65 -c src/modules/misc_helper.c 
	@mv src/modules/misc_helper.o build/misc_helper.o

build/adt_stack.o: src/modules/adt_stack.c
	cl65 -c src/modules/adt_stack.c 
	@mv src/modules/adt_stack.o build/adt_stack.o

build:
	@mkdir -p build
	@mkdir -p bin

clean:
	@rm -rf build
	@rm -rf bin