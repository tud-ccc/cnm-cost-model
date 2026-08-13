DPU_DIR := dpu
HOST_DIR := host
GENERATED_DIR ?= generated_headers
BUILDDIR ?= bin
NR_TASKLETS ?= 2
BL ?= 10
NR_DPUS ?= 1 

define conf_filename
	${BUILDDIR}/.NR_DPUS_$(1)_NR_TASKLETS_$(2)_BL_$(3).conf
endef
CONF := $(call conf_filename,${NR_DPUS},${NR_TASKLETS},${BL})

HOST_TARGET := ${BUILDDIR}/host
DPU_TARGET := ${BUILDDIR}/dpu

COMMON_INCLUDES := support
HOST_SOURCES := $(wildcard ${HOST_DIR}/*.c ) $(wildcard ${GENERATED_DIR}/*.c )
DPU_SOURCES := $(wildcard ${DPU_DIR}/*.c)

.PHONY: all clean test

__dirs := $(shell mkdir -p ${BUILDDIR})

COMMON_FLAGS := -Wall -Wextra -g -I${COMMON_INCLUDES} -I${GENERATED_DIR} -Wno-sign-compare -Wno-pointer-sign -Wno-unused-variable
HOST_FLAGS := ${COMMON_FLAGS} -std=c11 -O3 `dpu-pkg-config --cflags --libs dpu` -DNR_TASKLETS=${NR_TASKLETS} -DNR_DPUS=${NR_DPUS} -DBL=${BL} -DDPU_BINARY='"./${BUILDDIR}/dpu"'
DPU_FLAGS := ${COMMON_FLAGS} -O3 -DNR_TASKLETS=${NR_TASKLETS} -DBL=${BL} -DDPU_BINARY='"./${BUILDDIR}/dpu"'

DPU_ASSEMBLY := ${DPU_DIR}/dpu.s

all: ${HOST_TARGET} ${DPU_TARGET}

${CONF}:
	$(RM) $(call conf_filename,*,*)
	touch ${CONF}

${HOST_TARGET}: ${HOST_SOURCES} ${COMMON_INCLUDES} ${CONF}
	$(CC) -o $@ ${HOST_SOURCES} ${HOST_FLAGS} -lm

${DPU_TARGET}: ${DPU_SOURCES} ${COMMON_INCLUDES} ${CONF}
	dpu-upmem-dpurte-clang ${DPU_FLAGS} -o $@ ${DPU_SOURCES}

assembly: ${DPU_ASSEMBLY}

${DPU_ASSEMBLY}: ${DPU_SOURCES} ${COMMON_INCLUDES}
	dpu-upmem-dpurte-clang ${DPU_FLAGS} -S -o $@ ${DPU_SOURCES}

clean:
	$(RM) -r $(BUILDDIR)

test: all
	./${HOST_TARGET} -m 1024 -n 1024
