CC = gcc
CFLAGS = -Iinclude -Iinclude/core -Iinclude/core/common -Iinclude/core/masm -Iinclude/core/memulator -g

BUILD_DIR = build
NAME = masm
BIN = $(BUILD_DIR)/$(NAME)
SOURCE = $(shell find . -name "*.c")
OBJECTS = $(SOURCE:%.c=$(BUILD_DIR)/%.o)
TERMINALRC=.bashrc
$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(BIN)

install: all
	mv $(BIN) /usr/bin/$(NAME)

install-no-sudo:
	mkdir ~/.masm
	mv $(BIN) ~/.masm/$(NAME)
	echo "\n alias masm='~/.masm/masm'" >> .bashrc

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(BIN)