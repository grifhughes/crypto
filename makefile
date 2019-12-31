CC = gcc 
CCFLAGS = -m64 -std=gnu99 -O3 -mavx2 -s

tests_avx: tests.o dream128_avx.o dream256_avx.o
		$(CC) -o tests_avx tests.o avx_ref/dream128_avx.o avx_ref/dream256_avx.o

tests: tests.o dream128.o dream256.o
		$(CC) -o tests tests.o ref/dream128.o ref/dream256.o

tests.o: tests.c common/dream.h
		$(CC) $(CCFLAGS) -c tests.c

dream256_avx.o: avx_ref/dream256_avx.c common/dream.h common/dream_impl.h
		$(CC) $(CCFLAGS) -c avx_ref/dream256_avx.c -o avx_ref/dream256_avx.o

dream128_avx.o: avx_ref/dream128_avx.c common/dream.h common/dream_impl.h
		$(CC) $(CCFLAGS) -c avx_ref/dream128_avx.c -o avx_ref/dream128_avx.o

dream256.o: ref/dream256.c common/dream.h common/dream_impl.h
		$(CC) $(CCFLAGS) -c ref/dream256.c -o ref/dream256.o

dream128.o: ref/dream128.c common/dream.h common/dream_impl.h
		$(CC) $(CCFLAGS) -c ref/dream128.c -o ref/dream128.o

clean_avx: 
		rm avx_tests *.o avx_ref/*.o
	
clean:
		rm tests *.o ref/*.o
