#include <iostream>
#include "DataTool.h"
#include "Proxy.h"
#include <thread>
#include <chrono>
using namespace std;

int main()
{
    DataTool dataTool;
    dataTool.Init();

    dataTool.ConnectFront();
    if(!DataTool::bConnected)
    {
        cout<<"Cannot connect to the market front."<<endl;
        dataTool.Release();
        return 0;
    }

    dataTool.Login();
    if(!DataTool::bLogin)
    {
        cout<<"Cannot login."<<endl;
        dataTool.Release();
        return 0;
    }

    dataTool.Subscribe();
    char flag;
    while(true)
    {
        cin>>flag;
        if(flag=='c') break;
    }
    dataTool.Logout();
    dataTool.Release();
    return 0;
}
