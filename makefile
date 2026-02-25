# cpp files 
MAIN=main.cpp

# hpp files
TREE=tree.hpp


# CXXflags

STD=-std=c++17 # i like c++17 :)
WARNINGS=-Wpedantic
SYMBOLS=-g 
SANITIZE=-fsanitize=undefined
CC=g++
CXXFLAGS=$(SYMBOLS) $(WARNINGS) $(STD) $(SANITIZE)

# Out file 
OUT = a.out

OUT: $(MAIN) $(TREE)
	$(CC) $(CXXFLAGS) $(MAIN) -o $(OUT)
