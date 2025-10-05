BIN := pbs # set executable name

SOURCE_EXT := cpp

UNAME_S := $(shell uname -s)
CXXFLAGS := 
CFLAGS := 
CUSTOM_CFLAGS := -std=c++17 -Wall -Wextra\
				 -Werror -Wpedantic -Wno-unused-parameter\
				 -Wno-unused-variable \
				 -I./inc/ 
DEFAULT_COMPILER := g++

CPP = $(DEFAULT_COMPILER) $(CXXFLAGS) 

SRC_MODULES := $(sort $(dir $(wildcard ./src/*/)))
SRC_MODULES := $(SRC_MODULES:./src/%/=%)

SRC_DIRS := $(addprefix src/, $(SRC_MODULES)) src
OBJ_DIRS := $(addprefix build/, $(SRC_MODULES))
SOURCE := $(foreach sdir,$(SRC_DIRS),$(wildcard $(sdir)/*.$(SOURCE_EXT))) 

HEAD := $(wildcard inc/*.h) 

SHADERS := $(wildcard shaders/*.vert) $(wildcard shaders/*.frag)
SHADERS_SPV := $(SHADERS:shaders/%=shaders/%.spv)

OBJECTS = $(SOURCE:src/%.$(SOURCE_EXT)=build/%.o)

DEBUG_MODE := -DDEBUG
ifdef NDEBUG
DEBUG_MODE := -DNDEBUG
else
DEBUG_MODE := -DDEBUG
endif


.PHONY = all prod clean uninstall reinstall 



ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += `pkg-config --libs glfw3 vulkan` -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi 

	CXXFLAGS += `pkg-config --cflags glfw3 vulkan`  
	CFLAGS = $(CXXFLAGS)
endif


all: $(SRC_DIRS) $(OBJ_DIRS) inc/ $(OBJECTS) $(BIN) $(SHADERS_SPV)

prod : DEBUG_MODE = -DNDEBUG
prod : all

$(BIN): $(SOURCE) $(OBJECTS) $(HEAD) 
	$(CPP) $(CUSTOM_CFLAGS) $(DEBUG_MODE) -o $(BIN) $(OBJECTS) $(LIBS) 

build/%.o: src/%.$(SOURCE_EXT) $(HEAD)
	@mkdir -p build 
	$(CPP) $(CUSTOM_CFLAGS) $(DEBUG_MODE) -c $< -o $@

%.vert.spv: %.vert
	glslangValidator -V -o $@ $<

%.frag.spv: %.frag
	glslangValidator -V -o $@ $< 

uninstall: clean
	@rm -f $(BIN)

clean:
	@rm -drf build 
	@rm -f $(wildcard *.o)

reinstall: uninstall $(BIN)

$(SRC_DIRS) $(OBJ_DIRS) inc/ : 
	@mkdir -p $@
