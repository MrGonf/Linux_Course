CUR_DIR := .

INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib

LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

# Chuyen file thu vien sang file .o
create_obj:
#	gcc -c -fPIC src/tq84/add.c    -o bin/shared/add.o
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)
	gcc -c -fPIC $(SRC_DIR)/myMath.c   -o $(OBJ_DIR)/myMath.o -I $(INC_DIR)

# Tu file .o chuyen sang file thu vien
create_shared:
	gcc -shared $(OBJ_DIR)/myMath.o -o $(LIB_SHARED_DIR)/libmyMath.so

# Link cac file 
linking:
	gcc $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -lmyMath -o $(BIN_DIR)/use-shared-lib

all: create_obj create_shared linking
	@echo "Create shared Lib sucessfully"

clean:
	rm -rf $(BIN_DIR)/use-shared-lib $(OBJ_DIR)/*.o $(LIB_SHARED_DIR)/*.so

# LD_LIBRARY_PATH=./lib/shared/ ./bin/use-shared-lib