#include "Publisher.h"
#include <thread>
using namespace std;

Publisher::Publisher()
{
    context = zmq::context_t(1);
    repSocket = zmq::socket_t(context, ZMQ_REP);
    pubSocket = zmq::socket_t(context, ZMQ_PUB);

    int sndhwm = 0;
    pubSocket.setsockopt (ZMQ_SNDHWM, &sndhwm, sizeof (sndhwm));

    repSocket.bind("tcp://*:5555");
    pubSocket.bind("tcp://*:5556");

    subscribers = 0;
}

void Publisher::GenerateMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    currentMarketData.set_trading_day(pDepthMarketData->TradingDay);
    currentMarketData.set_instrument_id(pDepthMarketData->InstrumentID);
    currentMarketData.set_exchange_id(pDepthMarketData->ExchangeID);
    currentMarketData.set_exchange_inst_id(pDepthMarketData->ExchangeInstID);

    currentMarketData.set_last_price(pDepthMarketData->LastPrice);
    currentMarketData.set_pre_settlement_price(pDepthMarketData->PreSettlementPrice);
    currentMarketData.set_pre_close_price(pDepthMarketData->PreClosePrice);
    currentMarketData.set_pre_open_interest(pDepthMarketData->PreOpenInterest);
    currentMarketData.set_open_price(pDepthMarketData->OpenPrice);
    currentMarketData.set_highest_price(pDepthMarketData->HighestPrice);
    currentMarketData.set_lowest_price(pDepthMarketData->LowestPrice);
    currentMarketData.set_volume(pDepthMarketData->Volume);
    currentMarketData.set_turnover(pDepthMarketData->Turnover);
    currentMarketData.set_open_interest(pDepthMarketData->OpenInterest);
    currentMarketData.set_close_price(pDepthMarketData->ClosePrice);
    currentMarketData.set_settlement_price(pDepthMarketData->SettlementPrice);
    currentMarketData.set_upper_limit_price(pDepthMarketData->UpperLimitPrice);
    currentMarketData.set_lower_limit_price(pDepthMarketData->LowerLimitPrice);
    currentMarketData.set_pre_delta(pDepthMarketData->PreDelta);
    currentMarketData.set_curr_delta(pDepthMarketData->CurrDelta);

    currentMarketData.set_update_time(pDepthMarketData->UpdateTime);
    currentMarketData.set_update_millisec(pDepthMarketData->UpdateMillisec);

    currentMarketData.set_bid_price_1(pDepthMarketData->BidPrice1);
    currentMarketData.set_bid_volume_1(pDepthMarketData->BidVolume1);
    currentMarketData.set_ask_price_1(pDepthMarketData->AskPrice1);
    currentMarketData.set_ask_volume_1(pDepthMarketData->AskVolume2);
    currentMarketData.set_bid_price_2(pDepthMarketData->BidPrice2);
    currentMarketData.set_bid_volume_2(pDepthMarketData->BidVolume2);
    currentMarketData.set_ask_price_2(pDepthMarketData->AskPrice2);
    currentMarketData.set_ask_volume_2(pDepthMarketData->AskVolume3);
    currentMarketData.set_bid_price_3(pDepthMarketData->BidPrice3);
    currentMarketData.set_bid_volume_3(pDepthMarketData->BidVolume3);
    currentMarketData.set_ask_price_3(pDepthMarketData->AskPrice3);
    currentMarketData.set_ask_volume_3(pDepthMarketData->AskVolume4);
    currentMarketData.set_bid_price_4(pDepthMarketData->BidPrice4);
    currentMarketData.set_bid_volume_4(pDepthMarketData->BidVolume4);
    currentMarketData.set_ask_price_4(pDepthMarketData->AskPrice4);
    currentMarketData.set_ask_volume_4(pDepthMarketData->AskVolume4);
    currentMarketData.set_bid_price_5(pDepthMarketData->BidPrice5);
    currentMarketData.set_bid_volume_5(pDepthMarketData->BidVolume5);
    currentMarketData.set_ask_price_5(pDepthMarketData->AskPrice5);
    currentMarketData.set_ask_volume_5(pDepthMarketData->AskVolume5);

    currentMarketData.set_average_price(pDepthMarketData->AveragePrice);
    currentMarketData.set_action_day(pDepthMarketData->ActionDay);
}

void Publisher::PushMarketData()
{
    constexpr int BUFFSIZE = 1024;
    char buff[BUFFSIZE];
    long long len = currentMarketData.ByteSizeLong();
    currentMarketData.SerializeToArray(buff, BUFFSIZE);
    pubSocket.send(zmq::message_t(buff, len), zmq::send_flags::none);
}

void Publisher::ConnectToProxy()
{
    while (subscribers < 1)
    {
        zmq::message_t request;
        // Wait for next request from client
        repSocket.recv(request);
        //cout<<(char*)request.data()<<endl;
        //ReplyToSubscribe();
        zmq::message_t msg("",1);
        repSocket.send(msg, zmq::send_flags::none);
        subscribers++;
    }
    this_thread::sleep_for(chrono::milliseconds(100));
    zmq::message_t msg("",1);
    pubSocket.send(msg, zmq::send_flags::none);
}
