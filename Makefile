
GLFW_DIR := /home/daw/dev/glfw
GLFW_INCLUDE := $(GLFW_DIR)/include
GLFW_LIB_DIR := $(GLFW_DIR)/lib

GLM_DIR := /home/daw/dev/glm-0.9.4.3

ifndef verbose
  SILENT = @
endif

export GLFW_DIR GLM_DIR GLFW_INCLUDE GLFW_LIB_DIR SILENT

PROJECTS := ingredients chapter01 chapter02 chapter03 chapter04 chapter05 chapter06 chapter07 chapter08 chapter09

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

ingredients: 
	@echo "==== Building ingredients ($(config)) ===="
	@${MAKE} --no-print-directory -C ingredients -f Makefile

chapter01: ingredients
	@echo "==== Building chapter01 ($(config)) ===="
	@${MAKE} --no-print-directory -C chapter01 -f Makefile

chapter02: ingredients
	@echo "==== Building chapter02 ($(config)) ===="
	@${MAKE} --no-print-directory -C chapter02 -f Makefile

chapter03: ingredients
	@echo "==== Building chapter03 ($(config)) ===="
	@${MAKE} --no-print-directory -C chapter03 -f Makefile

chapter04: ingredients
	@echo "==== Building chapter04 ($(config)) ===="
	@${MAKE} --no-print-directory -C chapter04 -f Makefile

chapter05: ingredients
	@echo "==== Building chapter05 ($(config)) ===="
	@${MAKE} --no-print-directory -C chapter05 -f Makefile

chapter06: ingredients
	@echo "==== Building chapter06 ($(config)) ===="
	@${MAKE} --no-print-directory -C chapter06 -f Makefile

chapter07: ingredients
	@echo "==== Building chapter07 ($(config)) ===="
	@${MAKE} --no-print-directory -C chapter07 -f Makefile

chapter08: ingredients
	@echo "==== Building chapter08 ($(config)) ===="
	@${MAKE} --no-print-directory -C chapter08 -f Makefile

chapter09: ingredients
	@echo "==== Building chapter09 ($(config)) ===="
	@${MAKE} --no-print-directory -C chapter09 -f Makefile

clean:
	@${MAKE} --no-print-directory -C ingredients -f Makefile clean
	@${MAKE} --no-print-directory -C chapter01 -f Makefile clean
	@${MAKE} --no-print-directory -C chapter02 -f Makefile clean
	@${MAKE} --no-print-directory -C chapter03 -f Makefile clean
	@${MAKE} --no-print-directory -C chapter04 -f Makefile clean
	@${MAKE} --no-print-directory -C chapter05 -f Makefile clean
	@${MAKE} --no-print-directory -C chapter06 -f Makefile clean
	@${MAKE} --no-print-directory -C chapter07 -f Makefile clean
	@${MAKE} --no-print-directory -C chapter08 -f Makefile clean
	@${MAKE} --no-print-directory -C chapter09 -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   verbose"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   ingredients"
	@echo "   chapter01"
	@echo "   chapter02"
	@echo "   chapter03"
	@echo "   chapter04"
	@echo "   chapter05"
	@echo "   chapter06"
	@echo "   chapter07"
	@echo "   chapter08"
	@echo "   chapter09"
	@echo ""
