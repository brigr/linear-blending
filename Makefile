all:
	g++ linearblend.cpp -o linearblend `pkg-config opencv --libs --cflags`

clean:
	rm -rf linearblend
