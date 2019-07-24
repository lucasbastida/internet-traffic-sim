#include <iostream>
#include <fstream>

#include "Admin.h"
#include <stack>

using namespace std;

int main()
{

    Admin *admin = new Admin("tinyEWD.txt");
    cout << admin->G->toString() << endl;

    cout << "Testing cycle" << endl;

    admin->cycle();

    for (size_t i = 0; i < 10; i++)
    {
        if (i%2 == 0)
        {
            admin->updateRouteTable();
        }
        
        admin->cycleNoPage();

        for (int i = 0; i < admin->G->getV(); i++)
        {
            cout << admin->G->nodes[i].toString() << endl;
        }
        
        cout << admin->G->toString() << endl;
    }
    return 0;
}