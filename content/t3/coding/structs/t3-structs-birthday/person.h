#ifndef PERSON_H
#define PERSON_H

struct person {
    char name[32];
    int age;
};

void birthday(struct person *p);

#endif
