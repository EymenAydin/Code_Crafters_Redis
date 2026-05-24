obj=methods.o protocols.o client.o

.PHONY: clean

test_cli : $(obj)
	cc -o $(@) $(obj)

methods.o : src/methods.c include/methods.h
	cc -c src/methods.c -o $@

protocols.o : src/redis_protocols.c include/redis_protocols.h
	cc -c src/redis_protocols.c -o $@

client.o : tests/client.c include/redis_protocols.h
	cc -c tests/client.c -o $@

clean:
	rm -f test_cli $(obj)
