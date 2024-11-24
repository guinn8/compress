all: bin/compress bin/libcompress.so

bin/compress: main.c compress.c
	@mkdir -p bin
	gcc main.c compress.c -o bin/compress

bin/libcompress.so: compress.c
	gcc -shared -o bin/libcompress.so -fPIC compress.c