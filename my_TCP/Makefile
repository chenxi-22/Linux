all:client server

client:client.c
	gcc -static -o $@ $^
server:server.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -rf client server
