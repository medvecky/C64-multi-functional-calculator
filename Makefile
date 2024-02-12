multicalc: build src/main.fs
	acmeforth src/main.fs
	acme -o bin/multicalc.prg -f cbm main.asm	

build:
	@mkdir -p bin

clean:
	@rm -rf *.asm
	@rm -rf bin