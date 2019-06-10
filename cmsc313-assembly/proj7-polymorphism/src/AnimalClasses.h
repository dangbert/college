#define EAT_METHOD     0
#define SPEAK_METHOD   1
#define DISPLAY_METHOD 2

#ifndef ANIMAL_CLASSES_H
#define ANIMAL_CLASSES_H


/* Animal "class" */
typedef struct {
	int (**vtable)(void *, char *);             /* pointer to array of function pointers */

	/* data members for base class: (to be inherited by all derived classes) */
	char *name; 
	int weight;
	char *sound;
} Animal;


/* Lion "subclass" "derived" from Animal */
typedef struct {
	/* parent members */
	int (**vtable)(void *, char *);
	char *name; 
	int weight;
	char *sound;

	/* new members */
	int is_alpha_male;                          /* used as a boolean */
} Lion;


/* Fish subclass */
typedef struct {
	/* parent members */
	int (**vtable)(void *, char *);
	char *name; 
	int weight;
    char *sound;

	/* new members */
	char *fin_type;
} Fish;


int invoke(Animal *this, int method_slot, char *arg);

int Animal_eat(void *this, char *arg);
int Animal_speak(void *this, char *arg);
int Animal_display(void *this, char *arg);
Animal *new_Animal(void);

int Lion_eat(void *this, char *arg);
int Lion_speak(void *this, char *arg);
int Lion_display(void *this, char *arg);
Lion *new_Lion(void);

int Fish_eat(void *this, char *arg);
int Fish_speak(void *this, char *arg);
int Fish_display(void *this, char *arg);
Fish *new_Fish(void);

#endif
