obj = map.o test.o database.o timing.o
CFLAGS = -Wall -g
CC = cc

.PHONY: clean

test_m : $(obj)
	$(CC) -o $@ $(obj)

map.o : src/map.c include/map.h
	$(CC) $(CFLAGS) -c src/map.c -o $@

test.o : tests/map_test.c include/map.h
	$(CC) $(CFLAGS) -c tests/map_test.c -o $@

database.o : src/database.c include/database.h include/map.h
	$(CC) $(CFLAGS) -c  src/database.c -o $(@)

timing.o : src/timing.c include/timing.h
	$(CC) $(CFLAGS) -c src/timing.c -o $(@)

valgrind : test_m
	valgrind --leak-check=full --track-origins=yes ./test_m

run: test_m
	./test_m

clean :
	rm -f test_m $(obj)
