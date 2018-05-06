default:
	gcc -pthread -std=c99 sstf_test.c -o sstf_test
more_files:
	gcc -pthread -std=c99 sstf_test.c -D NUM_THREADS=200 -o sstf_test_many
clean:
	rm -f sstf_test sstf_test_many testfile*
