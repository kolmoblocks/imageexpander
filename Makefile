CXX = g++
CXXFLAGS = -std=c++14  -MMD -g
EXEC = diff
OBJECTS = generateDiff.o deltaUnit.o blockIterator.o rangeDiff/rangeDiff.o mapDiff.o lodepng.o bitUtil.o RLE.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

.PHONY: clean
