EXEC=test

SOURCES=main.cpp Controller.cpp
RESSOURCES=pal.png shader.vert shader.frag

OUT_DIR=bin
OBJ_DIR=obj

OBJECTS=${SOURCES:%.cpp=$(OBJ_DIR)/%.o}
ifeq ($(OS),Windows_NT)

	MINGW_PATH=C://mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64

	IDIR=-I include \
		 -I $(MINGW_PATH)/include \
		 -I $(MINGW_PATH)/glew/include \
		 -I $(MINGW_PATH)/glfw/include \
                 -I $(MINGW_PATH)/SOIL/src \
		 -I $(MINGW_PATH)/glm
	
	LDIR=-L $(MINGW_PATH)/lib \
		 -L $(MINGW_PATH)/glew/lib/Release/x64 \
		 -L $(MINGW_PATH)/glfw/lib-mingw-w64 \
		 -L $(MINGW_PATH)/SOIL/lib
		 
	CFLAGS=-Wall -Wextra -O3 -g3 -std=c++11 -Wno-unused-variable -Wno-unused-parameter
	LFLAGS=-lstdc++ \
	           -lSOIL \
		   -lglfw3 -lopengl32 -lglu32 -lgdi32 -lglew32
else

	IDIR=-I include \
		 -I /usr/include \
		 -I /usr/include/SOIL
	
	LDIR=-L /usr/lib \
		 -L /usr/local/lib \
		 -L /usr/lib64 \
		 -L /usr/lib/x86_64-linux-gnu
		 
	CFLAGS=-Wall -Wextra -O3 -g3 -std=c++11 -Wno-unused-variable -Wno-unused-parameter -fPIC
	LFLAGS=-lstdc++ \
		   -lXi -lGLEW -lm -lGL -lm -lpthread -ldl -ldrm \
		   -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lXxf86vm \
		   -lglfw -lrt -lm -ldl -lXrandr -lXinerama -lXext -lXcursor -lXrender -lXfixes -lX11 \
		   -lpthread -lxcb -lXau -lXdmcp \
		   -lSOIL \
		   -lpcre -lxml2 -lLLVM
endif

CC=g++

all: clean init $(OUT_DIR)/$(EXEC) copy

$(OUT_DIR)/$(EXEC): $(OBJECTS)
	@echo "\n=======LINKING=======\n"
	$(CC) -o $@ $^ $(LDIR) $(LFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	@echo "\n=======COMPILING $@======="
	$(CC) -o $@ -c $< $(IDIR) $(CFLAGS)

init:
	@mkdir -p "$(OUT_DIR)"
	@mkdir -p "$(OBJ_DIR)"

clean:
ifeq ($(OS),Windows_NT)
	@rm -f $(OUT_DIR)/$(EXEC).exe
else
	@rm -f $(OUT_DIR)/$(EXEC)
endif
	@rm -rf "$(OBJ_DIR)"

copy:
	@cp $(RESSOURCES) $(OUT_DIR)
