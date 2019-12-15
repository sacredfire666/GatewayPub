#include "MdSpi.h"
#include <unistd.h>
#include <string.h>
#include "DataTool.h"
#include <iostream>
#include <thread>
using namespace std;

void MdSpi::OnFrontConnected()
{
    DataTool::bConnected = true;
    cout<<"Front connected."<<endl;
}

void MdSpi::OnFrontDisconnected(int nReason)
{
    DataTool::bConnected = false;
    cout<<"Front disconnected."<<endl;
}

void MdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
    DataTool::bLogin = true;
    cout<<"Login."<<endl;
}

void MdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
    cout<<"Subscribed "<<pSpecificInstrument->InstrumentID<<". "<<pRspInfo->ErrorMsg<<endl;
    if(!m_pCtpPub)
        m_pCtpPub = make_shared<Publisher>();
    m_pCtpPub->ConnectToProxy();
};


void MdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) 
{
    cout<<"Data "<<pDepthMarketData->TradingDay<<" "<<pDepthMarketData->InstrumentID<<
    " "<<pDepthMarketData->UpdateTime<<" "<<pDepthMarketData->UpdateMillisec<<endl;
    m_pCtpPub->GenerateMarketData(pDepthMarketData);
    m_pCtpPub->PushMarketData();
}

