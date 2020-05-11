

#================================================================
#   Copyright (C) 2020 Sangfor Ltd. All rights reserved.
#   
#   File Name    ：makefile
#   Author       ：spider
#   Date Time    ：2020年04月15日
#   Describe     ：
#
#================================================================

.SUFFIXES:


CXX := g++
CXXFLAGS := -Wall -g -std=c++11
INCLUDES := -Iinclude
LIBS :=  -lpthread
TARGET := bin/client

SRCDIR := src
SRCOBJS := $(patsubst %.cpp, %.o, $(wildcard $(SRCDIR)/*.cpp))
OBJS := $(SRCOBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LIBS) 

$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -c -o $@
clean:
	-@rm -f $(TARGET)
	-@rm -f $(OBJS)
