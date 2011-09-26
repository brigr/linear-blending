LIBS = `pkg-config glib-2.0 gdk-pixbuf-2.0 --libs` `pkg-config opencv --libs`
CFLAGS = `pkg-config glib-2.0 gdk-pixbuf-2.0 --cflags` `pkg-config opencv --cflags`

all:
	g++ linearblend.cpp -o linearblend $(LIBS) $(CFLAGS)

debug:
	g++ linearblend.cpp -o linearblend $(LIBS) $(CFLAGS) -g

clean:
	rm -rf linearblend
