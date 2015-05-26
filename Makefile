# USAGE:
#
# // to compile:
# make
#
# // to compile tests and run tests:
# make testLangModel
#
# // remove compilation output files:
# make clean
#

CC = g++
CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -O # for final build
# CXXFLAGS = -std=c++11 -pedantic -Wall -Wextra -O0 -g   # for debugging

bin: LM_Driver

testLangModel: test_LangModel
		@echo "Running tests..."
		./test_LangModel
		@echo "All Tests Passed."

test_LangModel: testLangModel.cpp langModel.cpp
		$(CC) $(CXXFLAGS) -o test_LangModel testLangModel.cpp langModel.cpp

langModel.o: langModel.cpp langModel.h
		$(CC) $(CXXFLAGS) -c langModel.cpp

langModel: langModel.o
		$(CC) $(CXXFLAGS) -o langModel langModel.o

LM_Driver.o: LM_Driver.cpp langModel.h
		$(CC) $(CXXFLAGS) -c LM_Driver.cpp

LM_Driver: LM_Driver.o langModel.o
		$(CC) $(CXXFLAGS) -o LM_Driver LM_Driver.o langModel.o

clean:
		rm -f *.o LM_Driver testLangModel

