# COMPILER DEFINITIONS
CXX	= g++
CXXOPT	=
CXXFLAGS= -Wall -Wextra -std=c++17 -Wpedantic -Werror -fstack-protector-strong
LDFLAGS	=

# DIR DEFINITIONS
SRC	= src
OBJ	= build
INC	= include
TARGET	= bmtui

# SRC AND TARGET

SRCS	= $(wildcard $(SRC)/*.cpp)
OBJS	= $(SRCS:$(SRC)/%.cpp=$(OBJ)/%.o)
DEPS	= $(OBJS:.o=.d)

# HEADER SEARCH PATH

CPPFLAGS= -I$(INC)

# DEFAULT TARGETS
.PHONY: all clean run debug release

all: release

# RELEASE

release: CXXOPT := -O2
release: LDFLAGS :=
release: $(TARGET)

# DEBUG

debug: CXXOPT := -DDEBUG -g -O0 -fsanitize=address,undefined
debug: LDFLAGS := -fsanitize=address,undefined
debug: $(TARGET)

# LINK

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CXXOPT) $(LDFLAGS) -o $@ $^

# COMPILE

$(OBJ)/%.o: $(SRC)/%.cpp | $(OBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(CXXOPT) -MMD -MP -c $< -o $@

# MAKE DIR

$(OBJ):
	mkdir -p $(OBJ)

# DEPENDENCIES

-include $(DEPS)

# CLEAN

clean:
	rm -rf $(OBJ) $(TARGET)

# RUN

run: $(TARGET)
	./$(TARGET)
