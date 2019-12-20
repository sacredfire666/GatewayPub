//
// Created by sf666 on 12/10/19.
//

#include "Proxy.h"
#include "zmq.hpp"
#include <iostream>
#include "MarketSnapshot.pb.h"
using namespace std;

Proxy::Proxy()
{
    // Prepare subscribe signal context and socket
    context = zmq::context_t(1);
    frontend = zmq::socket_t(context, ZMQ_SUB);
    backend = zmq::socket_t(context, ZMQ_XPUB);

    frontend.connect("tcp://localhost:5556");
    backend.bind("tcp://*:5557");

    //  Subscribe to every single topic from publisher
    frontend.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    req = zmq::socket_t(context, ZMQ_REQ);
    req.connect("tcp://localhost:5555");

    items[0] = { static_cast<void*>(frontend), 0, ZMQ_POLLIN, 0 };
    items[1] = { static_cast<void*>(backend), 0, ZMQ_POLLIN, 0 };

    cacheLen = 0;

}

void Proxy::ListenLoop()
{
    zmq::message_t request("", 1);

    //cout << "Sending Subscribe signal to the server..." << endl;
    req.send(request, zmq::send_flags::none);
    // Get the reply.
    zmq::message_t reply;
    req.recv(reply);

    while (true)
    {
        if (zmq::poll(items, 2, 0) == -1)
            break; //  Interrupted
        //  Any new topic data we cache and then forward
        if (items[0].revents & ZMQ_POLLIN)
        {
            zmq::message_t Mreply;
            frontend.recv(Mreply);
            MarketSnapshot Mdata;
            Mdata.ParseFromArray(Mreply.data(), Mreply.size());
            cacheLen = Mreply.size();
            if(cacheLen>10)
            {
                const char* instrument = Mdata.instrument_id().c_str();
                for (int i = 0; i < cacheLen; i++) {
                    cache_map[instrument][i] = ((char *) (Mreply.data()))[i];
                }
                std::cout << "Proxy receive data: " << Mdata.trading_day() << " " << Mdata.instrument_id()
                          << " " << Mdata.update_time() << " " << Mdata.update_millisec() << " " << Mdata.action_day()
                          << std::endl;
                backend.send(zmq::message_t(instrument,strlen(instrument)+1),
                        zmq::send_flags::sndmore);
                backend.send(Mreply, zmq::send_flags::none);
            }
        }
        //  When we get a new subscription, we pull data from the cache:
        if (items[1].revents & ZMQ_POLLIN) {
            zmq::message_t msg;
            backend.recv(msg, zmq::recv_flags::none);
            if (msg.empty())
                break;

            //  Event is one byte 0=unsub or 1=sub, followed by topic
            uint8_t *event = (uint8_t *)msg.data();
            if (event[0] == 1) {
                if(cacheLen>10)
                {
                    std::string topic((char *)(event+1), msg.size()-1);
                    auto i = cache_map.find(topic);
                    if (i != cache_map.end())
                    {
                        backend.send(zmq::message_t(topic.c_str(),strlen(topic.c_str())+1),
                                     zmq::send_flags::sndmore);
                        cout<<"lec: "<<cacheLen<<endl;
                        //cout<<"body: "<<i->second<<endl;
                        backend.send(zmq::message_t(&(i->second[0]), cacheLen),
                                zmq::send_flags::none);
                    }

                    //zmq::message_t cacheMsg(cache_map, cacheLen);
                    //backend.send(cacheMsg, zmq::send_flags::none);
                }
            }
        }
    }
}