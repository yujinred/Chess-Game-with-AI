CXX = g++
CXXFLAGS = -g -Wall -MMD
EXECG = chess -lX11
EXEC = chess
OBJECTS = main.o piece.o grid.o rook.o king.o bishop.o pawn.o knight.o queen.o player.o human.o computer.o window-v2.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXECG}

-include ${DEPENDS}

.PHONY : clean
clean :
	rm ${DEPENDS} ${OBJECTS} ${EXEC}
