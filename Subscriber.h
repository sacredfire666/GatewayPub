//
// Created by sf666 on 12/15/19.
//

#pragma once

#include "zmq.hpp"

class Subscriber {
public:
    Subscriber();
    void GetDataLoop();

private:
    zmq::context_t context;
    zmq::socket_t subscriber;
};

