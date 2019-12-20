//
// Created by sf666 on 12/15/19.
//

#pragma once

#include "zmq.hpp"

class Subscriber {
public:
    explicit Subscriber(const char* instrument);
    void GetDataLoop();

private:
    const char* instrument;
    zmq::context_t context;
    zmq::socket_t subscriber;
};

