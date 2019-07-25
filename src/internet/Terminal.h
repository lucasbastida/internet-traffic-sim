#ifndef TERMINAL_H
#define TERMINAL_H

#include "Page.h"

class Terminal
{
public:
    Page *page;
    int pagesSent;
    int pagesRecieved;
    int id;

    Terminal() : pagesSent(0), pagesRecieved(0){};
    ~Terminal();

    std::string toString() { return "terminal ip:" + std::to_string(id) + " num.Pages sent:" + std::to_string(pagesSent) + " num.Pages Recieved:" + std::to_string(pagesRecieved); }
};

Terminal::~Terminal()
{
    delete page;
}
#endif