

all:
	@c++ -std=c++14 completer.cpp -o completer -fsanitize=address -ggdb 
	@./main
