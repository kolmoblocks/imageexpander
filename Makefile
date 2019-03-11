CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD -g
EXEC = rsze
OBJECTS = resize.o lodepng.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

.PHONY: clean
