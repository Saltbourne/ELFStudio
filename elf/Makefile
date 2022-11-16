CXX=g++
OBJ=binary

.PHONY: all clean

all: $(OBJ)

Binary.o: Binary.cpp Section.cpp
	$(CXX) -std=c++11 -g -c Binary.cpp -lbfd

binary: Binary.o Main.cpp
	$(CXX) -std=c++11 -g -o binary Main.cpp Symbol.cpp Binary.o -lbfd

clean:
	rm -f $(OBJ) *.o