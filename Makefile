

SRC = completer.cpp utils.cpp

OBJ = $(SRC:%.cpp=%.o)

all: $(OBJ)
	c++  -Wall -Wextra -std=c++14 $^ -o completer -lreadline
%.o: %.cpp $(HEADER)
	c++ -c -std=c++14 $< -o $@ -ggdb


clean:
	rm -vf $(OBJ)