//
// Interface in the main function.
//

#include "DataTool.h"
#include "MdSpi.h"
#include <iostream>
#include <thread>
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include "rapidjson/document.h"
#include "Proxy.h"

using namespace std;
using namespace rapidjson;
atomic_bool DataTool::bConnected = {false};
atomic_bool DataTool::bLogin = {false};

DataTool::DataTool()
{
    m_szMdFrontAddr = "tcp://180.168.146.187:10110";
    //m_szMdFrontAddr = "tcp://180.168.146.187:10131";
    m_szUserID = "156349";
    m_szPassword = "gengar";
    m_szBrokerID = "9999";
    instrumentsID[0] = "cu2001";
    instrumentsID[1] = "cu2002";
    m_nIns = 2;
}

void DataTool::Init()
{
    m_nReqId = 0;
    m_pMdSpi = make_shared<MdSpi>();
    m_pMdApi = CThostFtdcMdApi::CreateFtdcMdApi();
    cout<<"CTP API version: "<<CThostFtdcMdApi::GetApiVersion()<<endl;
    m_pMdApi->RegisterSpi(m_pMdSpi.get());

    const char* filename = "login.json";
    FILE* fp = fopen("login.json", "r");
    if (fp==nullptr)
    {
        // Default log in info
        cout<<"Fail to read file "<<filename<<endl;
        cout<<"Use default info."<<endl;
	}
    else
    {
        char readBuffer[65536];
        FileReadStream is(fp, readBuffer, sizeof(readBuffer));
        fclose(fp);

        Document d;
        d.ParseStream(is);
        m_szMdFrontAddr = d["FrontAddr2"].GetString();
        m_szUserID = d["UserID"].GetString();
        m_szPassword = d["Password"].GetString();
        m_szBrokerID = d["BrokerID"].GetString();
    }

    const char* ins_filename = "instruments.json";
    FILE* ins_fp = fopen(ins_filename, "r");
    if (ins_fp==nullptr)
    {
        cout<<"Fail to read file "<<filename<<endl;
        cout<<"Use default instrument cu2001 cu2002."<<endl;
        return;
    }
    else
    {
        char readBuffer[65536];
        FileReadStream is(ins_fp, readBuffer, sizeof(readBuffer));
        fclose(ins_fp);
        Document d;
        d.ParseStream(is);
        Value& s = d;
        m_nIns = s.Size();
        if(m_nIns==0 || m_nIns>65536)
        {
            cout<<"Cannot find instruments or Instruments number exceed 65536. "
                  "Use default instrument cu2001 cu2002."<<endl;
            m_nIns = 2;
        }
        vector<string> sss(m_nIns);
        for (int i = 0; i < m_nIns; i++)
        {
            sss[i] = s[i].GetString();
            instrumentsID[i] = sss[i].c_str();
        }
    }
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

void proxy_thread()
{
    Proxy proxy;
    proxy.ListenLoop();
}

void DataTool::Subscribe()
{
    // Create the proxy thread.
    thread threadObj(proxy_thread);
    threadObj.detach();

    // Subscribe here.
    cout<<"Subscribe instruments..."<<endl;

    int ret = m_pMdApi->SubscribeMarketData(const_cast<char**>(instrumentsID), m_nIns);

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

