SRC=src
BUILD=build
DOC=./

TARGET=icp_fsm_interpreter

all:
	$(MAKE) -C ./$(SRC)

run: all
	./$(TARGET)

doxygen: 
	doxygen ./$(DOC)doxyfile

pack:

clean:


.PHONY: all run pack clean doxygen
