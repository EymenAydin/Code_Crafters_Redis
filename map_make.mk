obj = map.o test.o

.PHONY: clean

test_m : $(obj)
	cc -o $@ $(obj)

map.o : src/map.c include/map.h
	cc -c src/map.c -o $@

test.o : tests/map_test.c include/map.h
	cc -c tests/map_test.c -o $@

clean :
	rm -f test_m $(obj)
