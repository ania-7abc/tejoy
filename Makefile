# =======
# Main paths and settings
# =======

SRC_DIR ?= src
INC_DIR ?= include
RES_DIR ?= resources
LIB_DIR ?= libs
BIN_DIR ?= bin
OBJ_DIR ?= build

TARGET ?= debug
OUT ?= tejoy

CXX ?= g++
CXXFLAGS += -Wall -Wextra -std=c++20 -g -fdiagnostics-color -Werror
LDFLAGS +=

PKG_CONFIG ?= pkg-config
PKGS += libsodium asio

EXTRA_CXXFLAGS += -I$(shell brew --prefix nlohmann-json)/include
EXTRA_LDFLAGS +=

# =======
# Target configurations
# =======

ifneq ($(TARGET),release)
	CXXFLAGS += -g3 -O0 -DDEBUG
	LDFLAGS += -fsanitize=undefined,address,leak
else
	TARGET := release
	CXXFLAGS += -O2 -DNDEBUG -s -ffast-math
	LDFLAGS += -flto
endif

OBJ_DIR_NO_TARGET := $(OBJ_DIR)
BIN_DIR_NO_TARGET := $(BIN_DIR)

OBJ_DIR := $(OBJ_DIR)/$(TARGET)
BIN_DIR := $(BIN_DIR)/$(TARGET)

# =======
# File search
# =======

LIB_SRC_DIRS := $(shell find $(LIB_DIR) -type d -name "$(SRC_DIR)" 2>/dev/null)
LIB_INCLUDE_DIRS := $(shell find $(LIB_DIR) -type d -name "$(INC_DIR)" 2>/dev/null)
LIB_RES_DIRS := $(shell find $(LIB_DIR) -type d -name "$(RES_DIR)" 2>/dev/null)

CXXFLAGS += $(foreach dir,$(LIB_INCLUDE_DIRS),-I$(dir))
CXXFLAGS += -I$(INC_DIR)

# Search for .cpp files
SRC_FILES := $(shell find $(SRC_DIR) $(LIB_SRC_DIRS) -name "*.cpp" -type f)
OBJ_FILES := $(foreach src,$(SRC_FILES),$(OBJ_DIR)/$(src:.cpp=.o))
RES_FILES := $(shell find $(RES_DIR) $(LIB_RES_DIR) -type f)

RES_TARGETS := $(patsubst $(RES_DIR)/%,$(BIN_DIR)/%,$(RES_FILES))

# =======
# Pkg-config dependencies
# =======

ifeq ($(PKGS),)
    PKG_CXXFLAGS :=
    PKG_LDFLAGS :=
else
    PKG_CXXFLAGS := $(shell $(PKG_CONFIG) --cflags $(PKGS))
    PKG_LDFLAGS := $(shell $(PKG_CONFIG) --libs $(PKGS))
endif

CXXFLAGS += $(PKG_CXXFLAGS) $(EXTRA_CXXFLAGS)
LDFLAGS += $(PKG_LDFLAGS) $(EXTRA_LDFLAGS)

# =======
# Build rules
# =======

all: $(BIN_DIR)/$(OUT) resources

$(BIN_DIR)/$(OUT): $(OBJ_FILES) | $(BIN_DIR) $(OBJ_DIR)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Rule for compiling .cpp to .o
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Automatic generation of dependencies for .cpp files
$(OBJ_DIR)/%.d: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM -MT $(@:.d=.o) $< > $@

# Including dependency files if they exist
-include $(OBJ_FILES:.o=.d)

$(OBJ_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@mkdir -p $@

# =======
# Copying resources
# =======

resources: $(RES_TARGETS)

$(BIN_DIR)/%: $(RES_DIR)/% | $(BIN_DIR)
	cp $< $@

# =======
# Cleanup
# =======

clean:
	rm -rf $(OBJ_DIR_NO_TARGET) $(BIN_DIR_NO_TARGET)

# =======
# Helper targets
# =======

.PHONY: all clean resources
