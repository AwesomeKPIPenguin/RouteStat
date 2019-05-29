
NAME = routestat

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))

CXX = g++
CXXFLAGS = -std=c++14
INCLUDES = -I inc -I json_lib/single_include/nlohmann
LIBS = -lpthread -lzmq -lpqxx -lpq

.PHONY: all clean fclean re
.NOTPARALLEL: all clean fclean re

all: dir $(NAME)

dir:
	@mkdir -p obj

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)

clean:
	@/bin/rm -f $(OBJS)

fclean: clean
	@/bin/rm -rf obj
	@/bin/rm -f $(NAME)

re: fclean all
