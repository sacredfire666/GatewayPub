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

Subscriber::Subscriber(const char* instrument)
{
    // Prepare subscribe data context and socket
    context = zmq::context_t(1);
    subscriber = zmq::socket_t(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5557");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, instrument, strlen(instrument));
}

void Subscriber::GetDataLoop()
{
    for (int request_nbr = 0; request_nbr != 10; request_nbr++)
    {
        zmq::message_t topic;
        zmq::message_t reply;
        subscriber.recv(topic);
        subscriber.recv(reply);
        //cout<<"header size: "<<topic.size()<<endl;
        //cout<<"Instrument "<<(char*)topic.data()<<endl;
        //cout<<"body size: "<<reply.size()<<endl;
        MarketSnapshot data;
        //data.ParseFromArray((char*)reply.data(), strlen((char*)reply.data()));
        data.ParseFromArray(reply.data(), reply.size());
        cout<<"Received data: "<<data.trading_day()<<" "<<data.instrument_id()
        <<" "<<data.update_time()<<" "<<data.update_millisec()<<endl;
    }
}