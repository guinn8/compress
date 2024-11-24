libcompress.so: compress.c
	gcc -shared -o libcompress.so -fPIC compress.c