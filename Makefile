
NAME = routestat

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

CXX = g++
CXXFLAGS = -std=c++14
INCLUDES = -I inc -I json_lib/single_include/nlohmann
LIBS = -lpthread -lzmq -lpqxx -lpq

.PHONY: all clean fclean re
.NOTPARALLEL: all clean fclean re

all: $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ $(LIBS)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJS) $(LIBS)

clean:
	@/bin/rm -f $(OBJS)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all
