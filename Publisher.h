#pragma once
#include <memory>
#include "zmq.hpp"
#include <set>
#include "MarketSnapshot.pb.h"
#include "ThostFtdcUserApiStruct.h"

class Publisher
{
public:
    Publisher();
    void ConnectToProxy();
    void GenerateMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    void PushMarketData();

protected:

private:
    MarketSnapshot currentMarketData;
    zmq::context_t context;
    zmq::socket_t repSocket;
    zmq::socket_t pubSocket;    
    int subscribers;
};
