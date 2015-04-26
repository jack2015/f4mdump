#include <cstdlib>
#include <cstring>
#include <ctime>
#include <memory>
#include <assert.h>

#include "debug.h"
#include "RTMPWrapper.h"
#include "RTMPTypes.h"


namespace rtmp
{

using namespace std;

bool GetStringItem(std::shared_ptr<RTMPItem> valueItem, std::string &value)
{
    bool bRet = false;
    if(valueItem.get() && RTMP_STRING_TYPE == valueItem->getType())
    {
        value = std::static_pointer_cast<RTMPString >(valueItem)->getValue();
        bRet = true;
    }
    return bRet;
}
 
bool GetStringItem(std::shared_ptr<RTMPItem> rtmpItem, const std::string &name, std::string &value)
{
    bool bRet = false;
    value = "";
    if(rtmpItem.get() && RTMP_LIST_TYPE == rtmpItem->getType())
    {
        bRet = GetStringItem((*std::static_pointer_cast<RTMPList >(rtmpItem))[name], value);
    }
    return bRet;
}
 
bool GetNumberItem(std::shared_ptr<RTMPItem> valueItem, double &value)
{
    bool bRet = false;
    if(valueItem.get() && RTMP_NUMBER_TYPE == valueItem->getType())
    {
        value = std::static_pointer_cast<RTMPNumber >(valueItem)->getValue();
        bRet = true;
    }
    return bRet;
}
 
bool GetNumberItem(std::shared_ptr<RTMPItem> rtmpItem, const std::string &name, double &value)
{
    bool bRet = false;
    value = 0.0;
    if(rtmpItem && RTMP_LIST_TYPE == rtmpItem->getType())
    {
        bRet = GetNumberItem((*std::static_pointer_cast<RTMPList >(rtmpItem))[name], value);
    }
    return bRet;
}

bool GetIntegerItem(std::shared_ptr<RTMPItem> valueItem, int32_t &value)
{
    bool bRet = false;
    if(valueItem.get() && RTMP_INTEGER_TYPE == valueItem->getType())
    {
        value = std::static_pointer_cast<RTMPInteger >(valueItem)->getValue();
        bRet = true;
    }
    return bRet;
}
 
bool GetIntegerItem(std::shared_ptr<RTMPItem> rtmpItem, const std::string &name, int32_t &value)
{
    bool bRet = false;
    value = 0.0;
    if(rtmpItem && RTMP_LIST_TYPE == rtmpItem->getType())
    {
        bRet = GetIntegerItem((*std::static_pointer_cast<RTMPList >(rtmpItem))[name], value);
    }
    return bRet;
}
 
bool GetBoolItem(std::shared_ptr<RTMPItem> valueItem, bool &value)
{
    bool bRet = false;
    if(valueItem.get() && RTMP_BOOLEAN_TYPE == valueItem->getType())
    {
        value = std::static_pointer_cast<RTMPBool >(valueItem)->getValue();
        bRet = true;
    }
    return bRet;
}
 
bool GetBoolItem(std::shared_ptr<RTMPItem> rtmpItem, const std::string &name, bool &value)
{
    bool bRet = false;
    value = false;
    if(rtmpItem && RTMP_LIST_TYPE == rtmpItem->getType())
    {
        bRet = GetBoolItem((*std::static_pointer_cast<RTMPList >(rtmpItem))[name], value);
    }
    return bRet;
}
 
bool GetListItem(std::shared_ptr<RTMPItem> valueItem, RTMPItems *&value)
{
    bool bRet = false;
    if(valueItem && RTMP_LIST_TYPE == valueItem->getType())
    {
        value = &(std::static_pointer_cast<RTMPList >(valueItem)->getValue());
        bRet = true;
    }
    return bRet;
}
 
bool GetListItem(std::shared_ptr<RTMPItem> rtmpItem, const std::string &name, RTMPItems *&value)
{
    bool bRet = false;
    // ASSERT(value);
    if(rtmpItem && RTMP_LIST_TYPE == rtmpItem->getType())
    {
        bRet = GetListItem((*std::static_pointer_cast<RTMPList >(rtmpItem))[name], value);
    }
    return bRet;
}

CRTMP::CRTMP(const std::string &rtmpUrl, const RTMPOptionsList_t &rtmpParams)
: m_bTerminate(false)
, m_rtmpUrl(rtmpUrl)
, m_rtmp(0)
, m_rtmpParams(rtmpParams)
{
    RTMP_LogSetOutput(stdout);
    m_rtmp = RTMP_Alloc();
    if(0 == m_rtmp)
    {
        throw "Failed to allocate RTMP handle";
    }
    
    RTMP_Init(m_rtmp);
    if( 1 > RTMP_SetupURL(m_rtmp, const_cast<char*>(m_rtmpUrl.c_str())) )
    {
        throw "Unable to parse URL";
    }
    
    set_options(rtmpParams);
}

CRTMP::~CRTMP()
{
    assert(m_rtmp);
    close();
    RTMP_Free(m_rtmp);
    m_rtmp = 0;
}

void CRTMP::connect()
{
    printDBG("CRTMP::connect\n");
   
    if( 1 > RTMP_Connect(m_rtmp, 0) )
    {
        throw "CRTMP::connect Failed to connect";
    }
}


void CRTMP::close()
{
    printDBG("Closes the connection to the server.\n");
    if(isConnected())
    {
        RTMP_Close(m_rtmp);
    }
}

bool CRTMP::isConnected()
{
    return RTMP_IsConnected(m_rtmp) ? true : false;
}


void CRTMP::set_options(const RTMPOptionsList_t &rtmpOprions)
{
    for(RTMPOptionsList_t::const_iterator it=rtmpOprions.begin(); it != rtmpOprions.end(); ++it)
    {
        set_option(it->first, it->second);
    }
}

void CRTMP::set_option(const RTMPOption_t &option)
{
    set_option(option.first, option.second);
}


void CRTMP::set_option(const std::string &key, const std::string &value)
{
    AVal aKey= {const_cast<char *>(key.c_str()), key.size()};
    AVal aVal= {const_cast<char *>(value.c_str()), value.size()};
    printDBG("key[%s] value[%s]\n", key.c_str(), value.c_str());
    if( 1 > RTMP_SetOpt(m_rtmp, &aKey, &aVal) )
    {
        throw "Unable to set option";
    }
}

void CRTMP::terminate()
{
    m_bTerminate = true;
    close();
}

void CRTMP::startInternalTimeout(const uint32_t timeout)
{
    m_internalTimeout = timeout;
    m_startInternalTimeout = time(0);
    m_bInternalTimeout = false;
}

bool CRTMP::isInternalTimeout()
{
    if((time(0) - m_startInternalTimeout) >= m_internalTimeout)
    {
        m_bInternalTimeout = true;
    }
    return m_bInternalTimeout;
}

bool CRTMP::read_packet(RTMPPacket &rtmpPacket)
{
    bool packetRead = false;
    while(!m_bTerminate && isConnected() && !isInternalTimeout())
    {
        if(0 == RTMP_ReadPacket(m_rtmp, &rtmpPacket))
        {
            printDBG("%s, read_packet error\n", __FUNCTION__);
            break;
        }
        if (RTMPPacket_IsReady(&rtmpPacket))
        {
            packetRead = true;
            break;
        }
    }
    return packetRead;
}

std::shared_ptr<RTMPList> CRTMP::handleServerInvoke(const std::string &strMethod, const uint32_t timeout)
{
    std::shared_ptr<RTMPList> objList;
    
    startInternalTimeout(timeout);
    RTMPPacket rtmpPacket;
    memset(&rtmpPacket, 0, sizeof(RTMPPacket));
    bool cleanUp = false;
    
    
    bool bRet = true;
    while(!m_bTerminate && isConnected() && !isInternalTimeout())
    {
        if(cleanUp)
        {
            RTMP_ClientPacket(m_rtmp, &rtmpPacket);
            RTMPPacket_Free(&rtmpPacket);
            memset(&rtmpPacket, 0, sizeof(RTMPPacket));
        }
        cleanUp = true;
        if(!read_packet(rtmpPacket))
        {
            break;
        }

        // OK we have here rtmpPacket
        if(RTMP_PACKET_TYPE_INVOKE == rtmpPacket.m_packetType)
        {
            // make sure it is a string method name we start with
            if (0 == rtmpPacket.m_nBodySize || rtmpPacket.m_body[0] != 0x02)
            {
                printDBG("%s, Sanity failed. no string method in invoke packet\n", __FUNCTION__);
                continue;
            }
            
            // decode_amf
            CRTMPAMFDecoder decoder;
            
            std::shared_ptr<RTMPList> tmpObjList = decoder.parse(rtmpPacket);
            
            std::shared_ptr<RTMPString> method = std::dynamic_pointer_cast<RTMPString>((*tmpObjList)[0]);
            std::shared_ptr<RTMPNumber> transaction_id = std::dynamic_pointer_cast<RTMPNumber>((*tmpObjList)[1]);
            
            //printDBG("SULGE >>>>>>>>>> size[%u] [%u] [%u]\n", (unsigned)tmpObjList->getValue().size(), (unsigned)GetStringItem((*tmpObjList)[0], method), (unsigned)(*tmpObjList)[1]->getType()); 
            if(tmpObjList && 2 < tmpObjList->getValue().size() && 
               method.get() && transaction_id.get() )
            {
                printDBG("%s, invoking <%s>\n", __FUNCTION__, method->getValue().c_str());
                if(method->getValue() == strMethod && 3 < tmpObjList->getValue().size())
                {
                    objList = std::dynamic_pointer_cast<RTMPList>((*tmpObjList)[3]);
                    break;
                }
            }
            else
            {
                continue;
            }
            if( 1.0 != transaction_id->getValue() )
            {
                RTMP_ClientPacket(m_rtmp, &rtmpPacket);
            }
        }
        else
        {
            RTMP_ClientPacket(m_rtmp, &rtmpPacket);
        }
    }
    
    if(cleanUp)
    {
        RTMPPacket_Free(&rtmpPacket);
        memset(&rtmpPacket, 0, sizeof(RTMPPacket));
    }
    
    return objList;
}

} /* namespace rtmp*/
