CPP_FILES	:= $(wildcard src/*.cpp)
OBJ_FILES	:= $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

CC_FLAGS	:= -std=c++14 -Wall -Wextra -pedantic-errors
CC_FLAGS	+= -fopenmp
LD_LIBS		:= -fopenmp -lfftw3 -lnessh -lcurl -lcurlpp -lpthread

EXECUTABLE	:= Server

$(EXECUTABLE): $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^ $(LD_LIBS)

obj/%.o: src/%.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

clean:
	rm -f obj/* $(EXECUTABLE)
	rm -f results/*

CC_FLAGS += -MMD
-include $(OBJFILES:.o=.d)