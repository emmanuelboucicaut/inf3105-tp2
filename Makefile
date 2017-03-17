#Lab6

all : test_arbreavl lab6-tache4

lab6-tache4: lab6-tache4.cpp arbreavl.h
	g++ -o lab6-tache4 lab6-tache4.cpp

test_arbreavl: test_arbreavl.cpp arbreavl.h
	g++ -o test_arbreavl test_arbreavl.cpp

clean :
	rm -f test_arbreavl
	rm -f lab6-tache4
	rm -f a.out
