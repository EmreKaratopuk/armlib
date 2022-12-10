#include "arm/armTcp.h"

using namespace arm;

ArmTcp::ArmTcp(const string& sHost, int port)
{
    auto& cntx = client_.cntx_;
    cntx.sHost = sHost;
    cntx.port = port;
}

ArmTcp::ArmTcp(const string& sUri)
{
    auto ss = tokens(sUri, ':');
    if(ss.size()<2)
    {
        log_e("sUri incorrect:'"+sUri+
        "', expect 'ip/hostname:port'");
        return;
    }
    //----
    client_.cntx_.sHost = ss[0];
    s2d(ss[1], client_.cntx_.port);
}

//-----
bool ArmTcp::init()
{
    auto& cntx = client_.cntx_;
    client_.setRcv([&](const char* buf, int len){
        onRecv(buf, len);
    });
    bool ok = client_.connect(cntx.sHost, cntx.port); 
    return ok;
}

//----
void ArmTcp::onRecv(const char* buf, int len)
{
    string s(buf, len);
    log_d("socket recv:"+s);

}
//----
bool ArmTcp::release()
{
    return client_.send("release");
}

//---
bool ArmTcp::reset()
{
    return client_.send("reset");

}

//-----
bool ArmTcp::moveTo(const TipSt& ts, float spd) 
{
    string s = "moveto ";

    s += "xyz=" + remove(str(ts.T.t), ' ') + " ";
    s += "rvec=" + remove(ts.T.e.str(), ' ') + " ";
    s += "grip=" + str(ts.gripper) +" ";
    s += "spd=" + str(std::max(spd, cfg_.maxSpeed));

    bool ok = client_.send(s);
    return ok;

}

ArmSt ArmTcp::getSt()const 
{
    ArmSt st;
    return st;

}

bool ArmTcp::test()
{
    return true;

}