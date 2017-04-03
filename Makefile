COMPILER = g++
FLAGS = -W -Wall
CXXFLAGS = -std=c++11
tp2: inf3105.o DocumentXML.o Histoire.o
	$(COMPILER) $(CXXFLAGS) $(FLAGS) -o tp2 inf3105.o DocumentXML.o Histoire.o
inf3105.o: inf3105.cpp Histoire.h DocumentXML.h
	$(COMPILER) $(CXXFLAGS) $(FLAGS) -c -o inf3105.o inf3105.cpp
DocumentXML.o: DocumentXML.cpp DocumentXML.h
	$(COMPILER) $(CXXFLAGS) -c -o DocumentXML.o DocumentXML.cpp
Histoire.o: Histoire.cpp Histoire.h
	$(COMPILER) $(CXXFLAGS) $(FLAGS) -c -o Histoire.o Histoire.cpp
clean:
	rm *.o
	rm tp2
	clear
