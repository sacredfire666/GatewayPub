#pragma once

#include "ThostFtdcMdApi.h"
#include "Publisher.h"
#include <memory>

class MdSpi : public CThostFtdcMdSpi
{
public:

    void ResponseClient();

    void OnFrontConnected() override;
    void OnFrontDisconnected(int nReason) override;;
    void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;;

    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) override;;
    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
            CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) override;;

private:
    std::shared_ptr<Publisher> m_pCtpPub;
};

