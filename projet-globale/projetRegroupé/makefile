# Compiler settings
CC = gcc
CFLAGS = -Wall -g -I./src
LDFLAGS = -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lm

# Directories
SRC_DIR = src
STAGES_DIR = $(SRC_DIR)/stages
MENU_DIR = $(SRC_DIR)/menu
PLAYER_DIR = $(SRC_DIR)/player
STORY_DIR = $(SRC_DIR)/story

# Source files
MAIN_SRC = $(SRC_DIR)/main.c
STAGE1_SRC = $(STAGES_DIR)/stage1/source.c
STAGE2_SRC = $(STAGES_DIR)/stage2/source.c
STAGE_FINALE_SRC = $(STAGES_DIR)/stage_finale/source.c
MENU_SRC = $(MENU_DIR)/menu_principale.c
PLAYER_SRC = $(PLAYER_DIR)/player.c
STORY_SRC = $(STORY_DIR)/histoire.c

# Object files
MAIN_OBJ = $(MAIN_SRC:.c=.o)
STAGE1_OBJ = $(STAGE1_SRC:.c=.o)
STAGE2_OBJ = $(STAGE2_SRC:.c=.o)
STAGE_FINALE_OBJ = $(STAGE_FINALE_SRC:.c=.o)
MENU_OBJ = $(MENU_SRC:.c=.o)
PLAYER_OBJ = $(PLAYER_SRC:.c=.o)
STORY_OBJ = $(STORY_SRC:.c=.o)

# Executable
TARGET = game

# Default target
all: $(TARGET)

# Link
$(TARGET): $(MAIN_OBJ) $(STAGE1_OBJ) $(STAGE2_OBJ) $(STAGE_FINALE_OBJ) $(MENU_OBJ) $(PLAYER_OBJ) $(STORY_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(TARGET)
	rm -f $(SRC_DIR)/*.o
	rm -f $(STAGES_DIR)/*/*.o
	rm -f $(MENU_DIR)/*.o
	rm -f $(PLAYER_DIR)/*.o
	rm -f $(STORY_DIR)/*.o

# Dependencies