CXX=g++
RM=rm -f
CXXFLAGS=-g -Wall -I ..
LDFLAGS=-g -Wall

SRCS=main.cpp Test_Example.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
APP=unittests

all: $(APP)

$(APP): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(APP) $(OBJS)

main.o: main.cpp

Test_Example.o: Test_Example.cpp

clean:
	$(RM) $(OBJS)
	$(RM) $(APP)
