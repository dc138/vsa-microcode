CXX      := g++
CXXFLAGS := -pedantic-errors -Wall -Wextra -std=c++20
LDFLAGS  := -L/usr/lib -lstdc++

R_CXXFLAGS := -Ofast -flto -Werror -DNDEBUG
D_CXXFLAGS := -O0 -g -D_DEBUG

GDB      := gdb
GDBFLAGS :=

VALGRIND      := valgrind
VALGRINDFLAGS := --leak-check=full --track-origins=yes -s

TARGET      := microcode
SOURCE_DIR  := src/
INCLUDE_DIR := include/
SRC         := $(shell find $(SOURCE_DIR) $(INCLUDE_DIR) -type f -iname "*.cpp")

D_BUILD_DIR   := ./build
D_OBJECT_DIR  := $(D_BUILD_DIR)
D_BINARY_DIR  := $(D_BUILD_DIR)/bin
D_OBJECTS  := $(SRC:%.cpp=$(D_OBJECT_DIR)/%.o)

R_BUILD_DIR   := ./release
R_OBJECT_DIR  := $(R_BUILD_DIR)
R_BINARY_DIR  := $(R_BUILD_DIR)/bin
R_OBJECTS  := $(SRC:%.cpp=$(R_OBJECT_DIR)/%.o)

.PHONY: all clean debug release debug_run release_run gdb valgrind compile_commands
all: release

$(D_OBJECT_DIR)/%.o: %.cpp
	@if [[ ! -d "$(dir $@)" ]]; then mkdir -p $(dir $@) \
	  && echo -e "[\033[34mMKDIR\033[0m] $(dir $@)"; fi
	@$(CXX) $(CXXFLAGS) $(D_CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ $(LDFLAGS) \
	  && echo -e "[\033[32mCXX\033[0m] \033[1m$^\033[0m -> \033[1m$@\033[0m"

$(R_OBJECT_DIR)/%.o: %.cpp
	@if [[ ! -d "$(dir $@)" ]]; then mkdir -p $(dir $@) \
	  && echo -e "[\033[34mMKDIR\033[0m] $(dir $@)"; fi
	@$(CXX) $(CXXFLAGS) $(R_CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@ $(LDFLAGS) \
	  && echo -e "[\033[32mCXX\033[0m] \033[1m$^\033[0m -> \033[1m$@\033[0m"

$(D_BINARY_DIR)/$(TARGET): $(D_OBJECTS)
	@if [[ ! -d "$(dir $@)" ]]; then mkdir -p $(dir $@) \
	  && echo -e "[\033[34mMKDIR\033[0m] $(dir $@)"; fi
	@$(CXX) $(CXXFLAGS) $(D_CXXFLAGS) -o $(D_BINARY_DIR)/$(TARGET) $^ $(LDFLAGS) \
	  && echo -e "[\033[32mLD\033[0m] \033[1m$^\033[0m -> \033[1m$@\033[0m"

$(R_BINARY_DIR)/$(TARGET): $(R_OBJECTS)
	@if [[ ! -d "$(dir $@)" ]]; then mkdir -p $(dir $@) \
	  && echo -e "[\033[34mMKDIR\033[0m] $(dir $@)"; fi
	@$(CXX) $(CXXFLAGS) $(R_CXXFLAGS) -o $(R_BINARY_DIR)/$(TARGET) $^ $(LDFLAGS) \
	  && echo -e "[\033[32mLD\033[0m] \033[1m$^\033[0m -> \033[1m$@\033[0m"

release: $(R_BINARY_DIR)/$(TARGET)
debug: $(D_BINARY_DIR)/$(TARGET)

debug_run: debug
	@echo -e "[\033[34mRUN\033[0m] $(D_BINARY_DIR)/$(TARGET)"
	@cd $(D_BINARY_DIR); ./$(TARGET)

release_run: release
	@echo -e "[\033[34mRUN\033[0m] $(R_BINARY_DIR)/$(TARGET)"
	@cd $(R_BINARY_DIR); ./$(TARGET)

gdb: debug
	@echo -e "[\033[34mGDB\033[0m] $(D_BINARY_DIR)/$(TARGET)"
	@$(GDB) $(GDBFLAGS) ./$(D_BINARY_DIR)/$(TARGET)

valgrind: debug
	@echo -e "[\033[34mVALGRIND\033[0m] $(D_BINARY_DIR)/$(TARGET)"
	@$(VALGRIND) $(VALGRINDFLAGS) ./$(D_BINARY_DIR)/$(TARGET)

compile_commands:
	-@if [[ -f compile_commands.json ]]; then rm compile_commands.json > /dev/null \
	  && echo -e "[\033[34mRM\033[0m] compile_commands.json"; fi
	@echo -n "[" >> compile_commands.json
	@printf "%s\0" $(SRC) | xargs -0 -I "{}" echo -e -n "\n  {\n    \"directory\": \"$(shell pwd)\",\n    \"file\": \"{}\",\n    \"command\": \"$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c {} $(LDFLAGS)\"\n  }," >> compile_commands.json
	@sed -i "$$ s/.$$//" compile_commands.json
	@echo -e "\n]" >> compile_commands.json
	@echo -e "[\033[34mOUT\033[0m] compile_commands.json"

clean:
	-@if [[ -d "$(D_BUILD_DIR)" ]]; then rm -rfv $(D_BUILD_DIR) > /dev/null \
	  && echo -e "[\033[34mRM\033[0m] $(D_BUILD_DIR)"; fi
	-@if [[ -d "$(R_BUILD_DIR)" ]]; then rm -rfv $(R_BUILD_DIR) > /dev/null \
	  && echo -e "[\033[34mRM\033[0m] $(R_BUILD_DIR)"; fi
