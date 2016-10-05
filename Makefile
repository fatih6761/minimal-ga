all:
	g++ -Wall -O3 -o bin/mga src/main.cpp -std=c++11

clean:
	rm bin/*
