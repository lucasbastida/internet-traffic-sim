#include <iostream>
#include <fstream>

#include "Admin.h"
#include <stack>
#include <chrono>
#include <thread>

using namespace std;

using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;      // nanoseconds, system_clock, seconds

// sleep_for(nanoseconds(10));
// sleep_until(system_clock::now() + seconds(1));

int main()
{

    Admin *admin = new Admin("tinyEWD.txt");
    cout << admin->G->toString() << endl;

    cout << "Testing cycle" << endl;

    for (size_t i = 0; i < 10; i++)
    {
        if (i % 2 == 0)
        {
            admin->updateRouteTable();
        }

        admin->cycle();

        for (int i = 0; i < admin->G->getV(); i++)
        {
            cout << admin->G->nodes[i].toString() << endl;
        }

        cout << admin->G->toString() << endl;
        // sleep_for(seconds(3));
    }
    return 0;
}