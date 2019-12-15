//
// Created by sf666 on 12/9/19.
//

#include "DataTool.h"
#include "MdSpi.h"
#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

atomic_bool DataTool::bConnected = {false};
atomic_bool DataTool::bLogin = {false};

void DataTool::Init()
{
    m_nReqId = 0;
    m_pMdSpi = make_shared<MdSpi>();
    m_pMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
    cout<<"CTP API version: "<<CThostFtdcMdApi::GetApiVersion()<<endl;
    m_pMdApi->RegisterSpi(m_pMdSpi.get());
    //m_szMdFrontAddr = "tcp://180.168.146.187:10110";
    m_szMdFrontAddr = "tcp://180.168.146.187:10131";
    m_szUserID = "156349";
    m_szPassword = "gengar";
    m_szBrokerID = "9999";
}

void DataTool::ConnectFront()
{
    m_pMdApi->RegisterFront(const_cast<char*>(m_szMdFrontAddr.data()));
    m_pMdApi->Init();

    for(int i=0; i<10; i++)
    {
        cout<<"Connecting the market front... "<<i+1<<" try..."<<endl;
        this_thread::sleep_for(chrono::seconds(1));
        if(bConnected) break;
    }

}

void DataTool::Login()
{
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, m_szBrokerID.data());
    strcpy(req.UserID, m_szUserID.data());
    strcpy(req.Password, m_szPassword.data());
    for(int i=0; i<10; i++)
    {
        int ret = m_pMdApi->ReqUserLogin(&req, ++m_nReqId);
        cout<<"Logging... "<<i+1<<" try... RequestID: "<<m_nReqId<<endl;
        this_thread::sleep_for(chrono::seconds(1));
        if(bLogin) break;
    }
}

void DataTool::Subscribe()
{
    cout<<"Subscribe instruments..."<<endl;
    // Subscribe here.
    const char* instrumentsID[2] = {"cu1912", "cu2001"};
    int ret = m_pMdApi->SubscribeMarketData(const_cast<char**>(instrumentsID), 2);
}

void DataTool::Release()
{
    cout<<"Release"<<endl;
    m_pMdApi->Release();
}

void DataTool::Logout()
{
    CThostFtdcUserLogoutField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, "9999");
    strcpy(req.UserID, "156349");

    m_pMdApi->ReqUserLogout(&req, m_nReqId);
    cout<<"Logout."<<endl;
}

