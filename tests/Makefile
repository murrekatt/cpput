CXX=g++
RM=rm -f
CXXFLAGS=-g -Wall -I ..
LDFLAGS=-g -Wall

SRCS=main.cpp Test_TestHarness.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
APP=unittests

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(APP) $(OBJS)

main.o: main.cpp

Test_TestHarness.o: Test_TestHarness.cpp

clean:
	$(RM) $(OBJS)
	$(RM) $(APP)
