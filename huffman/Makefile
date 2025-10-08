BUILD_DIR=build
SRC_DIR=src
INCLUDE_DIR=include
OBJECTS=create_tree.o read_file.o
MAIN_OBJECT=$(BUILD_DIR)/huffman.o
MAIN_SOURCE=$(SRC_DIR)/huffman.c

main: build_dir $(MAIN_OBJECT) $(OBJECTS)
	gcc -o $(BUILD_DIR)/main $(MAIN_OBJECT) $(addprefix $(BUILD_DIR)/, $(OBJECTS))

build_dir:
	mkdir -p $(BUILD_DIR)

$(MAIN_OBJECT): $(MAIN_SOURCE)
	gcc -c $^ -o $@

$(OBJECTS): %.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h
	gcc -c $< -o $(BUILD_DIR)/$@

clean:
	rm -rf $(BUILD_DIR)

