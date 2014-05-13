CFLAGS=-I../stage/include/upnp -I.
LDFLAGS=-lupnp -lixml -lthreadutil -lpthread -L../stage/lib

all : micro_media_renderer


micro_media_renderer : micro_media_renderer.o sample_util.o

clean :
	rm -rf micro_media_renderer *.o -rf
