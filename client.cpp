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

int main ()  
{
    Subscriber sub;
    sub.GetDataLoop();

    return 0;
}  
