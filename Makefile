# author: Matej Hornik(xhorni20), Filip Brna(xbrnaf00)
# Projekt: IMS, projekt SHO
# date: 1.12.2021

EXEC = sho
CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDLIBS =  -lsimlib #/usr/local/lib/simlib.a
ZIP_NAME = 10_xbrnaf00_xhorni20.zip

all: $(EXEC)

$(EXEC): sho.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: 
clean:
	rm -f $(EXEC) *.o $(ZIP_NAME)

run: ./sho
	./sho $(E)

zip:
	zip -r $(ZIP_NAME) sho.cpp experiment_data Makefile