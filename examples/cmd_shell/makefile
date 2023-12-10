ifeq ($(OS),Windows_NT)
    CURR_DIR_PATH = $(CURDIR)
else
    CURR_DIR_PATH = $(shell pwd)
endif


GCC:=gcc
INC+=-I${CURR_DIR_PATH}/../../
INC+=-I${CURR_DIR_PATH}/

SRC+=${CURR_DIR_PATH}/../../cot_param.c
SRC+=${CURR_DIR_PATH}/demo.c
SRC+=${CURR_DIR_PATH}/param_demo.c


AIM_NAME:=demo

.PHONY:all
all:
	${GCC} ${INC} ${SRC} -o ${CURR_DIR_PATH}/${AIM_NAME} -std=c99

.PHONY:clean
clean:
	rm -rf ${CURR_DIR_PATH}/${AIM_NAME}