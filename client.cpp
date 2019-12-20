//  
// Hello World client in C++  
// Connects REQ socket to tcp://localhost:5555  
// Sends "Hello" to server, expects "World" back  
//  
#include <cstdio>
#include <unistd.h>
#include <string>  
#include <iostream>  
#include "Subscriber.h"
using namespace std;

int main(int argc, char* argv[])
{
    const char* instrument;
    if(argc==2)
        instrument = argv[1];
    else
        instrument = "cu2001";
    Subscriber sub(instrument);
    sub.GetDataLoop();

    return 0;
}  
