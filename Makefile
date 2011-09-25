all:
	g++ linear_blend.cpp -o linear_blend `pkg-config opencv --libs --cflags`

clean:
	rm -rf linear_blend
