#ifndef RECT_H
#define RECT_H

struct rect {
    int w, h;
};

int area(struct rect r);
int perimeter(struct rect r);
int is_square(struct rect r);

#endif
