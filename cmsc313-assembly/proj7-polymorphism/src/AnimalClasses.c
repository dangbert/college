#include <stdio.h>
#include <stdlib.h>
#include "AnimalClasses.h"

int invoke(Animal *this, int method_slot, char *arg) {
    return (*this->vtable[method_slot])(this, arg);
}

int Animal_eat(void *this, char *arg) {
    Animal* a = (Animal*) this;
    printf("Animal:%s eats %s.\n", a->name, arg);
    return 0;
}
int Animal_speak(void *this, char *arg) {
    Animal* a = (Animal*) this;
    printf("Animal:%s says \"%s\".\n", a->name, a->sound);
    return 0;
}
int Animal_display(void *this, char *arg) {
    Animal* a = (Animal*) this;
    printf("Animal:%s:\nweight: %d\nsound: %s\n", a->name, a->weight, a->sound);
    return 0;
}
static int (*Animal_vtable[3])(void *, char *) = {
    &Animal_eat,
    &Animal_speak,
    &Animal_display
};
Animal *new_Animal(void) {
    Animal* a = malloc(sizeof(Animal));
    a->vtable = Animal_vtable;
    return a;
}


int Lion_eat(void *this, char *arg) {
    Lion* a = (Lion*) this;
    printf("Lion:%s chomps %s.\n", a->name, arg);
    return 0;
}
int Lion_speak(void *this, char *arg) {
    Lion* a = (Lion*) this;
    printf("Lion:%s roars \"%s\".\n", a->name, a->sound);
    return 0;
}
int Lion_display(void *this, char *arg) {
    Lion* a = (Lion*) this;
    printf("Lion:%s:\nweight: %d\nsound: %s\nis alpha male: ", a->name, a->weight, a->sound);
    if (a->is_alpha_male)
        printf("true\n");
    else
        printf("false\n");
    return 0;
}
static int (*Lion_vtable[3])(void *, char *) = {
    &Lion_eat,
    &Lion_speak,
    &Lion_display
};
Lion *new_Lion(void) {
    Lion* a = malloc(sizeof(Lion));
    a->vtable = Lion_vtable;
    return a;
}


int Fish_eat(void *this, char *arg) {
    Fish* a = (Fish*) this;
    printf("Fish:%s nibbles %s.\n", a->name, arg);
    return 0;
}
int Fish_speak(void *this, char *arg) {
    Fish* a = (Fish*) this;
    printf("Fish:%s burbles \"%s\".\n", a->name, a->sound);
    return 0;
}
int Fish_display(void *this, char *arg) {
    Fish* a = (Fish*) this;
    printf("Fish:%s:\nweight: %d\nsound: %s\n", a->name, a->weight, a->sound);
    printf("fin type: %s\n", a->fin_type);
    return 0;
}
static int (*Fish_vtable[3])(void *, char *) = {
    &Fish_eat,
    &Fish_speak,
    &Fish_display
};
Fish *new_Fish(void) {
    Fish* a = malloc(sizeof(Fish));
    a->vtable = Fish_vtable;
    return a;
}
