LIBS=-lopencv_highgui -lopencv_core -lopencv_imgcodecs
all: main

main: GrayImageOperations.o main.o
	g++ GrayImageOperations.o main.o ${LIBS} -o main

main.o: main.cpp
	g++ -c main.cpp

GrayImageOperations.o: GrayImageOperations.cpp
	g++ -c GrayImageOperations.cpp

.PHONY: clean

clean:
	rm *.o *.bmp main output.txt
