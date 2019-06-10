CFLAGS = -ansi -Wall #-g -gstabs+

proj4: character.o game.o ghost.o linked.o main.o node.o zombie.o human.o doctor.o buffy.o vampire.o redshirt.o
	g++ character.o game.o ghost.o linked.o main.o node.o zombie.o human.o doctor.o buffy.o vampire.o redshirt.o -o proj4


game.o: game.cpp game.h
	g++ -c game.cpp -o game.o

linked.o: linked.cpp linked.h
	g++ -c linked.cpp -o linked.o

node.o: node.cpp node.h
	g++ -c node.cpp -o node.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

#:::characters:::
character.o: character.cpp character.h
	g++ -c character.cpp -o character.o

zombie.o: zombie.cpp zombie.h
	g++ -c zombie.cpp -o zombie.o

human.o: human.cpp human.h
	g++ -c human.cpp -o human.o

doctor.o: doctor.cpp doctor.h
	g++ -c doctor.cpp -o doctor.o

buffy.o: buffy.cpp buffy.h
	g++ -c buffy.cpp -o buffy.o

vampire.o: vampire.cpp vampire.h
	g++ -c vampire.cpp -o vampire.o

redshirt.o: redshirt.cpp redshirt.h
	g++ -c redshirt.cpp -o redshirt.o