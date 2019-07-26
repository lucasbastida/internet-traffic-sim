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

    cout << "-------------------------------LOADING-------------------------------" << std::endl;
    Admin *admin = new Admin("tinyEWD.txt");

    cout << endl;
    cout << "STARING CYCLES:" << endl;
    cout << endl;

    for (int loop = 0; loop < 10; loop++)
    {
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@CYCLE " << loop << " START@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;

        if (loop % 2 == 0 && loop != 0)
        {
            admin->updateRouteTable();
        }

        admin->cycle();
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CYCLE " << loop << " END@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;        
        cout << "END OF CYCLE INFO:" << std::endl;

        for (int i = 0; i < admin->G->getV(); i++)
        {
            cout << admin->G->nodes[i].toString() << endl;
            for (int j = 0; j < admin->terminalAmount; j++)
            {
                cout << admin->G->nodes[i].terminals[j].toString() << endl;
            }
        }
        // sleep_for(seconds(3));
            cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << std::endl;
        cout << endl
             << endl
             << endl;
    }

    delete admin;
    return 0;
}