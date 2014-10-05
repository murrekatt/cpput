CXX=g++
RM=rm -f
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
CXXFLAGS=-Wall -W -Werror -pedantic -O0 -fno-inline $(GCOV_FLAGS)
LDFLAGS=--coverage

SRCS=main.cpp Test_TestHarness.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
APP=unittests

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(APP) $(OBJS)

main.o: main.cpp

Test_TestHarness.o: Test_TestHarness.cpp

test:
	./$(APP)

clean:
	$(RM) $(OBJS)
	$(RM) $(APP)
	$(RM) *.gcda *.gcno *.gcov
