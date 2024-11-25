libcompress.so: compress.c
	gcc -shared -Wall -Werror -o libcompress.so -fPIC compress.c