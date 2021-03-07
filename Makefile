test:	links \
		build \
		run_test \
		clean

links:
	sudo sh ./deps/linkGlib.sh

build:
	mpicxx -c util/utilities.c -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lglib-2.0 -o utilities.o
	mpicxx -c src/cipher.c -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lglib-2.0 -o cipher.o
	mpicxx -fopenmp -c dcryptions/decryptionOMP.c -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lglib-2.0 -o decryptionOMP.o
	mpicxx -fopenmp -c src/main.c -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -lglib-2.0 -o main.o
	mpicxx -fopenmp -o main main.o utilities.o cipher.o decryptionOMP.o /usr/local/cuda-9.1/lib64/libcudart_static.a -ldl -lrt -lglib-2.0

clean:
	rm -f *.o ./main

run_test:
	mpiexec -np 2 ./main "4" "./cipherText.e"

run:
	mpiexec -np 2 ./main $(length) $(file) ${words}

change_mod:
	find . -type f -exec chmod 755 {} \;

