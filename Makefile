# -- Author's Note ------------------------------------------------------------
#
# Well, it's not the worst Makefile I've ever written...
#
#
.DEFAULT_GOAL := all

C = gcc
CC = g++
CFLAGS = -O3 -static -I. -I./include -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -std=c++11

SRC_DIR  := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
BUILD_DIR = $(SRC_DIR)/obj

# Make build/staging directories. Should be used as a "order-only" prerequisite
# (prefixed with a "|" ) on any build rules.
$(BUILD_DIR)/ :
	mkdir -p $(BUILD_DIR)
	
	

# -- Global build rules -------------------------------------------------------
#
# Rules for all packages of the MSToolkit.
#

.PHONY: all clean realclean
all       : NeoPepXMLParser
clean     : NeoPepXMLParser-clean
realclean : NeoPepXMLParser-realclean

# -- NeoPepXMLParser ----------------------------------------------------------
#
# The core NeoPepXMLParser files
#
#
NEO_SRCDIR = $(SRC_DIR)/
NEO_DSTDIR = $(BUILD_DIR)/
NEO_SRC = $(wildcard $(NEO_SRCDIR)*.cpp)
NEO_DST = $(patsubst ${NEO_SRCDIR}%.cpp, ${NEO_DSTDIR}%.o, $(NEO_SRC))

.PHONY : NeoPepXMLParser

NeoPepXMLParser : $(NEO_DST)
	ar rcs libneopepxmlparser.a $(NEO_DST)

$(NEO_DST) : | $(BUILD_DIR)/
$(NEO_DST) : $(NEO_DSTDIR)%.o : $(NEO_SRCDIR)%.cpp
	$(CC) $(CFLAGS) $< -c -o $@

NeoPepXMLParser-clean :
	rm -rf $(NEO_DST)

NeoPepXMLParser-realclean : mstoolkit-clean
	rm -rf $(NEO_DST) libneopepxmlparser.a
	

# -- cleanup  ----------------------------------------------------------
#
#
.PHONY : rc

rc:
	rm -rf $(BUILD_DIR)
