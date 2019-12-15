//
// Created by sf666 on 12/15/19.
//

#include "Subscriber.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include "zmq.hpp"
#include "MarketSnapshot.pb.h"

using namespace std;

Subscriber::Subscriber()
{
    // Prepare subscribe data context and socket
    context = zmq::context_t(1);
    subscriber = zmq::socket_t(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5557");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
}

void Subscriber::GetDataLoop()
{
    for (int request_nbr = 0; request_nbr != 10; request_nbr++)
    {
        zmq::message_t reply;
        subscriber.recv(reply);
        //cout<<"size: "<<reply.size()<<endl;
        MarketSnapshot data;
        //data.ParseFromArray((char*)reply.data(), strlen((char*)reply.data()));
        data.ParseFromArray(reply.data(), reply.size());
        std::cout<<"Received data: "<<data.trading_day()<<" "<<data.instrument_id()
        <<" "<<data.update_time()<<" "<<data.update_millisec()<<" "<<data.action_day()<< std::endl;
    }
}