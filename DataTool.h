//
// Created by sf666 on 12/9/19.
//
#pragma once

#include "ThostFtdcMdApi.h"
#include "MdSpi.h"
#include <memory>
#include <atomic>

class DataTool {
public:
    DataTool();
    ~DataTool() = default;
    static std::atomic_bool bConnected;
    static std::atomic_bool bLogin;
    void Init();
    void ConnectFront();
    void Login();
    void Subscribe();
    void Logout();
    void Release();

private:
    int m_nReqId;
    CThostFtdcMdApi* m_pMdApi;
    std::shared_ptr<MdSpi> m_pMdSpi;
    std::string m_szUserID;
    std::string m_szPassword;
    std::string m_szBrokerID;
    std::string m_szMdFrontAddr;
    const char* instrumentsID[65535];
    int m_nIns;
};

