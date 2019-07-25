#ifndef PAGE_H
#define PAGE_H

class Page
{
public:
    int size;

    Page(int size) : size(size){};
    std::string toString() { return "page size:" + std::to_string(size); }
};
#endif