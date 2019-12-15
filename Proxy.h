//
// Created by sf666 on 12/10/19.
//

#pragma once

#include "zmq.hpp"

class Proxy {
public:
    Proxy();
    ~Proxy() {}
    void ListenLoop();
private:
    zmq::context_t context;
    zmq::socket_t req;
    zmq::socket_t frontend;
    zmq::socket_t subscriber;
    zmq::socket_t syncclient;
    zmq::socket_t backend;
    zmq::pollitem_t items[2];
    char cache_map[1024];
    int cacheLen;
};

