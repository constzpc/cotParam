CURR_DIR_PATH=$(shell pwd)

CURR_DIR_PATH="C:\Users\const\Desktop\param\param\Demo\cmd_shell"

GCC:=gcc
INC+=-I${CURR_DIR_PATH}/../../Param/inc
INC+=-I${CURR_DIR_PATH}/

SRC+=${CURR_DIR_PATH}/../../Param/src/param.c
SRC+=${CURR_DIR_PATH}/../../Param/src/stream.c
SRC+=${CURR_DIR_PATH}/demo.c


AIM_NAME:=demo

.PHONY:all
all:
	${GCC} ${INC} ${SRC} -o ${CURR_DIR_PATH}/${AIM_NAME} -std=c99

.PHONY:clean
clean:
	rm -rf ${CURR_DIR_PATH}/${AIM_NAME}