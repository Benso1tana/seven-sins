# Compiler settings
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lm

# Directories
MENU_DIR = menu
STAGE_DIR = stage\ .1
PLAYER_DIR = player

# Source files
SRCS = main.c \
       $(MENU_DIR)/menu_principale.c \
       $(STAGE_DIR)/source.c \
       $(PLAYER_DIR)/p.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = game

# Default target
all: $(TARGET)

# Link all object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilation rules
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(MENU_DIR)/*.o
	rm -f $(STAGE_DIR)/*.o
	rm -f $(PLAYER_DIR)/*.o

# Install dependencies (for Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install -y libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev

# Run target
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean install-deps run