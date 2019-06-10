CPPFLAGS = -ansi -Wall

lab8: Residence.o House.o Apartment.o Tent.o residence_driver.o
	g++ Residence.o House.o Apartment.o Tent.o residence_driver.o -o lab8

Residence.o: Residence.cpp Residence.h
	g++ -c Residence.cpp -o Residence.o

House.o: House.cpp
	g++ -c House.cpp -o House.o

Apartment.o: Apartment.cpp Apartment.h
	g++ -c Apartment.cpp -o Apartment.o

Tent.o: Tent.cpp Tent.h
	g++ -c Tent.cpp -o Tent.o

residence_driver.o: residence_driver.cpp
	g++ -c residence_driver.cpp -o residence_driver.o