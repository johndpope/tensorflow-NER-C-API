### Makefile for building tensorflow application
# link pre-build libtensorflow_cc.so to your personal build project
# param:
# INCLUDES: -I, remember to include eigen3 and tf libs
# LDLIBS : -L path of folder where libtensorflow_cc.so exist
# ACTUAL_LIBS: -l   name of actual lib(name).so


SOURCE_DIR =./src
BIN_DIR = ./bin

CPP     = g++ -std=c++11
LDFLAGS = -g -Wall -D_DEBUG -Wshadow -Wno-sign-compare -w 
LDLIBS  = -L ./include/tf
INCLUDES = -I ./include -I./include/tf -I./include/tf/bazel-genfiles -I ./include/tf/bazel-tensorflow-1.4.1/external/nsync/public -I/usr/local/include/eigen3 
ACTUAL_LIBS = -ltensorflow_cc -ltensorflow_framework

INPUT_FILE = $(SOURCE_DIR)/main2.cpp $(SOURCE_DIR)/ner_model.cpp $(SOURCE_DIR)/tf_model.cpp $(SOURCE_DIR)/sentence_breaker.cpp
OBJET_FILE = $(BIN_DIR)/ner_demo

tfcpp_demo:
	$(CPP) -o $(OBJET_FILE) $(INCLUDES) $(LDFLAGS) $(LDLIBS) $(ACTUAL_LIBS) $(INPUT_FILE)
