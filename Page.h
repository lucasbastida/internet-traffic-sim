#ifndef PAGE_H
#define PAGE_H

class Page
{
public:
    int id;
    int size;

    Page(int id, int size) :id(id), size(size){};
};
#endif