# author: Matej Hornik(xhorni20), Filip Brna(xbrnaf00)
# Projekt: IMS, projekt SHO
# date: 1.12.2021

EXEC = sho
CXX = g++
CXXFLAGS = -std=c++17 -Wall 
LDLIBS = /usr/local/lib/simlib.a -lm

all: $(EXEC)

$(EXEC): sho.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $^ $(LDLIBS)

.PHONY: 
clean:
	rm -f $(EXEC) *.o