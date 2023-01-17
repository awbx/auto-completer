
NAME = completer

SRC = completer.cpp utils.cpp

HEADER = completer.hpp

OBJ = $(SRC:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	c++  -Wall -Wextra -std=c++14 $^ -o $@ -fsanitize=address -lreadline
%.o: %.cpp $(HEADER)
	c++ -c -std=c++14 $< -o $@ -ggdb


clean:
	rm -vf $(OBJ)