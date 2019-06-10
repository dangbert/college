#include "AnimalClasses.h"
#include <stdio.h>

int main(int argc, char **argv) {
  char *animal_class[5];
  Animal *zoo[5];
  char *food[5];
  Animal *animalp;
  Lion *lionp;
  Fish *fishp;
  int i;

  animal_class[0] = "Lion";
  lionp = new_Lion();
  lionp->name = "Nala";
  lionp->weight = 1000;
  lionp->sound = "GRRRR";
  lionp->is_alpha_male = 0; /* true */
  zoo[0] = (Animal *) lionp;
  food[0] = "impala";

  animal_class[1] = "Fish";
  fishp = new_Fish();
  fishp->name = "Nemo";
  fishp->weight = 1;
  fishp->sound = "blub";
  fishp->fin_type = "round";
  zoo[1] = (Animal *) fishp;
  food[1] = "worms";

  animal_class[2] = "Animal";
  animalp = new_Animal();
  animalp->name = "JaneDoe";
  animalp->weight = 10;
  animalp->sound = "mmph";
  zoo[2] = (Animal *) animalp;
  food[2] = "paste";

  animal_class[3] = "Fish";
  fishp = new_Fish();
  fishp->name = "Dory";
  fishp->weight = 5;
  fishp->sound = "blub";
  fishp->fin_type = "pointed";
  zoo[3] = (Animal *) fishp;
  food[3] = "squid";

  animal_class[4] = "Lion";
  lionp = new_Lion();
  lionp->name = "Simba";
  lionp->weight = 1001;
  lionp->sound = "GROWL";
  lionp->is_alpha_male = 1; /* true */
  zoo[4] = (Animal *) lionp;
  food[4] = "mice";

  for (i = 0; i < 5; i++) {
    printf("Invoking methods for object %d (class %s):\n",
	   i, animal_class[i]);
    invoke(zoo[i], SPEAK_METHOD, NULL);
    invoke(zoo[i], EAT_METHOD, food[i]);
    invoke(zoo[i], DISPLAY_METHOD, NULL);
    printf("------------------------\n\n");
  }

  return 0;
}
