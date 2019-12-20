//
// Created by sf666 on 12/10/19.
//

#pragma once

#include "zmq.hpp"
#include <map>

class Proxy {
public:
    Proxy();
    ~Proxy() = default;
    void ListenLoop();
private:
    zmq::context_t context;
    zmq::socket_t req;
    zmq::socket_t frontend;
    zmq::socket_t backend;
    zmq::pollitem_t items[2];
    //char cache_map[1024];
    std::map<std::string, std::array<char, 1024>> cache_map;
    int cacheLen;
};

