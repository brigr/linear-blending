all:
	g++ linearblend.cpp -o linearblend `pkg-config glib-2.0 gdk-pixbuf-2.0 --libs --cflags` `pkg-config opencv --libs --cflags` -g

clean:
	rm -rf linearblend
