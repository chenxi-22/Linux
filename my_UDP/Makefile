all:service client
client:client.c
	gcc -o $@ $^
service:service.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -rf service client
	
