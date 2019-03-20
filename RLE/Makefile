CXX = g++
CXXFLAGS = -std=c++14  -MMD -g
EXEC = rle
OBJECTS = main.o ../generateDiff/generateDiff.o ../deltaUnit/deltaUnit.o ../blockIterator/blockIterator.o ../rangeDiff/rangeDiff.o ../mapDiff/mapDiff.o ../lodepng/lodepng.o ../bitUtil/bitUtil.o RLE.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

.PHONY: clean
