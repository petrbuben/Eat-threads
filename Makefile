
mon: main.o monitor.o monitor.hpp
	g++ main.o monitor.o -o mon 
main.o: main.cpp monitor.hpp
	g++ -c main.cpp
monitor.o: monitor.cpp
	g++ -c monitor.cpp 
clean:
	rm -f *.o mon 
