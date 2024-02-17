# MakeFile
# Project Name
PROJECT_NAME = scheduler-sim
TEST_NAME = test-${PROJECT_NAME}

# Compilation Options
CC = gcc
CC_FLAGS =  -c \
	-I $(INCLUDE) \
	-Wall \
	-pedantic \
	-g \
	-o

# Folders
SRC = ./src
APP = ./app
OBJ = ./obj
INCLUDE = ./include
TEST = ./tests
OUTPUT = ./output

# Files
MAIN = ${APP}/main.c
MAIN_OBJ = ${OBJ}/main.o
TEST_MAIN = ${TEST}/main-test.c
TEST_MAIN_OBJ = ${OBJ}/main-test.o
C_HEADERS = $(wildcard ${INCLUDE}/*.h)
C_SOURCE = $(wildcard ${SRC}/*.c)
OBJ_SOURCE = $(subst .c,.o,$(subst $(SRC),$(OBJ), $(C_SOURCE)))

all: ${OBJ} $(OUTPUT) $(PROJECT_NAME)

$(OBJ):
	@ mkdir obj
	@ echo "Object Folder Created!"

$(OUTPUT):
	@ mkdir output
	@ echo "Output File Created!"

$(PROJECT_NAME): $(OBJ_SOURCE) $(MAIN_OBJ)
	@ echo 'Compiling Executable File...'
	@ $(CC) $^ -o $@
	@ echo 'Finished Building Project!'

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	@ echo 'Building and Linking File: $@'
	@ $(CC) $< $(CC_FLAGS) $@
	@ echo ' '

$(MAIN_OBJ): $(MAIN)
	@ echo 'Building and Linking Main File: $@'
	@ $(CC) $< $(CC_FLAGS) $@
	@ echo ' '


# MAKE FOR TEST FILE
test: ${OBJ} ${TEST_NAME}

$(TEST_NAME): ${OBJ_SOURCE} ${TEST_MAIN_OBJ}
	@ echo 'Compiling Test File...'
	@ $(CC) $^ -o $@
	@ echo 'Finished Buildind Test Project!'

$(TEST_MAIN_OBJ): $(TEST_MAIN)
	@ echo 'Building Test Main File: $@'
	@ $(CC) $< $(CC_FLAGS) $@
	@ echo ' '


# CLEAN COMMANDS
clean: clean_obj clean_core clean_output

clean_obj:
	@ echo "Cleaning All Object Files..."
	@ rm -rf obj/

clean_core:
	@ echo "Cleaning All Core Files..."
	@ rm -f ./core*

clean_output:
	@ echo "Cleaning All Output Files..."
	@ rm -rf output/


# RUN PROGRAM
run:
ifdef ARGS
	@ ./${PROJECT_NAME} ${ARGS}
else
	@ ./${PROJECT_NAME}
endif

run_test:
ifdef ARGS
	@ ./${TEST_NAME} ${ARGS}
else
	@ ./${TEST_NAME}
endif
