bin/compress: main.c compress.c
	@mkdir -p bin
	gcc main.c compress.c -o bin/compress