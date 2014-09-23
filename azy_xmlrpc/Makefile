CC=gcc

all: client server

client: client.c azy
	gcc -g -o client client.c Demo_HowAreYou.azy_client.c Demo_Common.c Demo_Common_Azy.c `pkg-config --libs --cflags azy` `pkg-config --libs ecore eina`

server: client.c azy
	gcc -g -o server server.c Demo_HowAreYou.azy_server.c Demo_Common.c Demo_Common_Azy.c `pkg-config --libs --cflags azy` `pkg-config --libs ecore eina`

azy:
	azy_parser services.azy

clean:
	rm -f Demo_Common.{c,h} Demo_Common_Azy.{c,h} Demo_HowAreYou.azy_client.{c,h} Demo_HowAreYou.azy_server.{c,h} client server
