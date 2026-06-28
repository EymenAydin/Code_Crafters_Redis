obj=methods.o protocols.o client.o
CC= cc
CFLAGS= -Wall -g

.PHONY: clean

test_cli : $(obj)
	cc -o $(@) $(obj)

methods.o : src/methods.c include/methods.h
	$(CC) $(CFLAGS) -c src/methods.c -o $@

protocols.o : src/redis_protocols.c include/redis_protocols.h
	$(CC) $(CFLAGS) -c src/redis_protocols.c -o $@

client.o : tests/client.c include/redis_protocols.h
	$(CC) $(CFLAGS) -c tests/client.c -o $@

run : test_cli
	./test_cli

valgrind : test_cli
	valgrind --leak-check=full --track-origins=yes ./test_cli


clean:
	rm -f test_cli $(obj)
