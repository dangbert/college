FLAGS = -Wall
DRIVER = Driver

all: Driver.o Commodity.o Game.o CommodityStore.o Objective.o Player.o Card.o
	g++ $(FLAGS) Driver.o Commodity.o Game.o CommodityStore.o Objective.o Player.o Card.o -o Driver.out

Driver.o: $(DRIVER).cpp
	g++ $(FLAGS) -c $(DRIVER).cpp -o Driver.o

Commodity.o: Commodity.cpp
	g++ $(FLAGS) -c Commodity.cpp

Game.o: Game.cpp
	g++ $(FLAGS) -c Game.cpp

CommodityStore.o: CommodityStore.cpp
	g++ $(FLAGS) -c CommodityStore.cpp

Objective.o: Objective.cpp
	g++ $(FLAGS) -c Objective.cpp

Player.o: Player.cpp
	g++ $(FLAGS) -c Player.cpp

Card.o: Card.cpp
	g++ $(FLAGS) -c Card.cpp


# run with make run CARDS=”<card_filename>” COMMODITIES=”<commodities_filename>”
# PLAYERS=<num_players> STRATEGY=<strategy_id>
# e.g.     make run CARDS=cards.txt COMMODITIES=commodities.txt PLAYERS=4 STRATEGY=3
#run:
run:
	./Driver.out $(CARDS) $(COMMODITIES) $(PLAYERS) $(STRATEGY)

test:
	clear
	make
	valgrind --leak-check=full ./Driver.out cards.txt commodities.txt 4 3

save:
	scp -r . end1@gl.umbc.edu:proj2/src

#diff:
#	make clean
#	clear
#	make
#	./a.out > temp.txt 2>&1
#	vim -d temp.txt ../expected_outputs/$(DRIVER).txt

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
	rm -f results.txt
