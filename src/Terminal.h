#ifndef TERMINAL_H
#define TERMINAL_H

#include "Page.h"

class Terminal
{
public:
    Page * page;
    int numberSent;
    int numberRecieved;
    int id;

    Terminal(int id, Page * page) : id(id), page(page), numberSent(0), numberRecieved(0) {};
};
#endif