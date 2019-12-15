#include <iostream>
#include <unistd.h>
#include "Publisher.h"
#include "DataTool.h"
#include "Proxy.h"
#include <thread>
#include <chrono>
using namespace std;

void proxy_thread()
{
    Proxy proxy;
    proxy.ListenLoop();
    /*
    zmq::context_t context(1);

    //  First, connect our subscriber socket
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    //  Second, synchronize with publisher
    zmq::socket_t syncclient (context, ZMQ_REQ);
    syncclient.connect("tcp://localhost:5555");

    //  - send a synchronization request
    //s_send (syncclient, "");
    zmq::message_t request("", 1);
    syncclient.send(request, zmq::send_flags::none);

    //  - wait for synchronization reply
    //s_recv(syncclient);
    zmq::message_t reply;
    syncclient.recv(reply);
    //cout<<"Rep Receive"<<endl;

    //cout<<"LoopProxy"<<endl;
    //s_recv(subscriber);
    zmq::message_t Mreply;
    subscriber.recv(Mreply);
    //cout<<(char*)Mreply.data()<<endl;
    cout<<"Loopend"<<endl;
*/
    // Origin
    /*
    zmq::context_t context(1);

    //  First, connect our subscriber socket
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5556");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

    //  Second, synchronize with publisher
    zmq::socket_t syncclient (context, ZMQ_REQ);
    syncclient.connect("tcp://localhost:5555");

    //  - send a synchronization request
    //s_send (syncclient, "");
    zmq::message_t request("s", 2);
    syncclient.send(request, zmq::send_flags::none);
    //  - wait for synchronization reply
    zmq::message_t reply;
    syncclient.recv(reply);
    cout<<"Rep Receive"<<endl;

    cout<<"LoopProxy"<<endl;
    zmq::message_t Mreply;
    subscriber.recv(Mreply);

    cout<<(char*)Mreply.data()<<endl;
    cout<<"Loopend"<<endl;
     */
}

int main()
{
    //return 0;
    printf("the pid of this program is %d\n",(int)getpid());


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

    thread threadObj(proxy_thread);
    threadObj.detach();

    dataTool.Subscribe();
    //sleep(3);
    char flag;
    while(true)
    {
        cin>>flag;
        if(flag=='c') break;
    }
    dataTool.Logout();
    dataTool.Release();
    return 0;


    // Origin
    /*
    printf("the pid of this program is %d\n",(int)getpid());
    zmq::context_t context = zmq::context_t(1);
    zmq::socket_t repSocket = zmq::socket_t(context, ZMQ_REP);
    zmq::socket_t pubSocket = zmq::socket_t(context, ZMQ_PUB);

    int sndhwm = 0;
    pubSocket.setsockopt (ZMQ_SNDHWM, &sndhwm, sizeof (sndhwm));

    repSocket.bind("tcp://*:5555");
    pubSocket.bind("tcp://*:5556");

    int subscribers = 0;
    while (subscribers < 1)
    {
        zmq::message_t request;
        // Wait for next request from client
        repSocket.recv(request);
        cout<<(char*)request.data()<<endl;
        //ReplyToSubscribe();
        zmq::message_t msg("start",6);
        repSocket.send(msg, zmq::send_flags::none);
        subscribers++;
    }
    zmq::message_t msg("miao",5);
    pubSocket.send(msg, zmq::send_flags::none);
    this_thread::sleep_for(chrono::milliseconds(10));
    return 1;
    */
}
