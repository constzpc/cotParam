CURR_DIR_PATH=$(shell pwd)

CURR_DIR_PATH="C:\Users\const\Desktop\param\param\test"

GCC:=gcc
INC+=-I${CURR_DIR_PATH}/../Param/inc
INC+=-I${CURR_DIR_PATH}/unity
INC+=-I${CURR_DIR_PATH}/

SRC+=${CURR_DIR_PATH}/param_test.c
SRC+=${CURR_DIR_PATH}/unity/unity.c
SRC+=${CURR_DIR_PATH}/../Param/src/param.c

AIM_NAME:=param_test

.PHONY:all
all:
	${GCC} ${INC} ${SRC} -o ${CURR_DIR_PATH}/${AIM_NAME} -std=c99

.PHONY:clean
clean:
	rm -rf ${CURR_DIR_PATH}/${AIM_NAME}