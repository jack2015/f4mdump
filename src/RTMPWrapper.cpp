#include <cstdlib>
#include <cstring>
#include <ctime>
#include "debug.h"
#include "RTMPWrapper.h"

#define ASSERT(x)

namespace rtmp
{

using namespace std;

bool GetStringItem(RTMPItem *valueItem, std::string &value)
{
    bool bRet = false;
    if(valueItem && RTMP_STRING_TYPE == valueItem->getType())
    {
        value = static_cast<RTMPString *>(valueItem)->getValue();
        bRet = true;
    }
    return bRet;
}

bool GetStringItem(RTMPItem *rtmpItem, const std::string &name, std::string &value)
{
    bool bRet = false;
    value = "";
    if(rtmpItem && RTMP_LIST_TYPE == rtmpItem->getType())
    {
        bRet = GetStringItem((*static_cast<RTMPList *>(rtmpItem))[name], value);
    }
    return bRet;
}

bool GetNumberItem(RTMPItem *valueItem, double &value)
{
    bool bRet = false;
    if(valueItem && RTMP_NUMBER_TYPE == valueItem->getType())
    {
        value = static_cast<RTMPNumber *>(valueItem)->getValue();
        bRet = true;
    }
    return bRet;
}

bool GetNumberItem(RTMPItem *rtmpItem, const std::string &name, double &value)
{
    bool bRet = false;
    value = 0.0;
    if(rtmpItem && RTMP_LIST_TYPE == rtmpItem->getType())
    {
        bRet = GetNumberItem((*static_cast<RTMPList *>(rtmpItem))[name], value);
    }
    return bRet;
}

bool GetBoolItem(RTMPItem *valueItem, bool &value)
{
    bool bRet = false;
    if(valueItem && RTMP_BOOLEAN_TYPE == valueItem->getType())
    {
        value = static_cast<RTMPNumber *>(valueItem)->getValue();
        bRet = true;
    }
    return bRet;
}

bool GetBoolItem(RTMPItem *rtmpItem, const std::string &name, bool &value)
{
    bool bRet = false;
    value = false;
    if(rtmpItem && RTMP_LIST_TYPE == rtmpItem->getType())
    {
        bRet = GetBoolItem((*static_cast<RTMPList *>(rtmpItem))[name], value);
    }
    return bRet;
}

bool GetListItem(RTMPItem *valueItem, RTMPItems *&value)
{
    bool bRet = false;
    if(valueItem && RTMP_LIST_TYPE == valueItem->getType())
    {
        value = &(static_cast<RTMPList *>(valueItem)->getValue());
        bRet = true;
    }
    return bRet;
}

bool GetListItem(RTMPItem *rtmpItem, const std::string &name, RTMPItems *&value)
{
    bool bRet = false;
    // ASSERT(value);
    if(rtmpItem && RTMP_LIST_TYPE == rtmpItem->getType())
    {
        bRet = GetListItem((*static_cast<RTMPList *>(rtmpItem))[name], value);
    }
    return bRet;
}

CRTMP::CRTMP(const std::string &rtmpUrl, const RTMPOptionsList_t &rtmpParams)
: m_bTerminate(false)
, m_rtmpUrl(rtmpUrl)
, m_rtmp(0)
, m_rtmpParams(rtmpParams)
{
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
    ASSERT(m_rtmp);
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
    if((time(0) - m_startInternalTimeout) >= m_startInternalTimeout)
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

 RTMPList* getAllProperties(AMFObject &obj, const std::string &name)
 {
    RTMPList *objList = 0;
    for(int i=0; i<obj.o_num; ++i)
    {
        RTMPItem *item = 0;
        std::string tmpName(obj.o_props[i].p_name.av_val, obj.o_props[i].p_name.av_len);
        switch(obj.o_props[i].p_type)
        {
        case AMF_OBJECT:
        case AMF_STRICT_ARRAY:
            {
                AMFObject tmpObj;
                memset(&tmpObj, 0, sizeof(tmpObj));
                AMFProp_GetObject(AMF_GetProp(&obj, NULL, i), &tmpObj);
                item = getAllProperties(tmpObj, tmpName);
            }
        break;
        case RTMP_BOOLEAN_TYPE:
        case AMF_NUMBER:
            {
                item = new RTMPNumber(tmpName, obj.o_props[i].p_vu.p_number);
            }
        break;
        case AMF_STRING:
            {
                item = new RTMPString(tmpName, std::string(obj.o_props[i].p_vu.p_aval.av_val, obj.o_props[i].p_vu.p_aval.av_len));
            }
        break;
        default:
            printDBG("------------------------------------>%s, ignored item: p_name[%s] type[%x]\n", __FUNCTION__, obj.o_props[i].p_name.av_val, obj.o_props[i].p_type);
        }
        
        if(0 == objList && 0 != item)
        {
            objList = new RTMPList(name);
        }
        
        if(0 != objList && 0 != item)
        {
            objList->append(item);
        }
    }
    return objList;
 }

RTMPList* CRTMP::handleServerInvoke(const std::string &strMethod, const uint32_t timeout)
{
    RTMPList *objList = 0;
    
    startInternalTimeout(timeout);
    RTMPPacket rtmpPacket;
    memset(&rtmpPacket, 0, sizeof(RTMPPacket));
    bool cleanUp = false;
    
    
    bool bRet = true;
    while(!m_bTerminate && isConnected() && !isInternalTimeout())
    {
        if(cleanUp)
        {
            RTMPPacket_Free(&rtmpPacket);
            memset(&rtmpPacket, 0, sizeof(RTMPPacket));
        }
        if(!read_packet(rtmpPacket))
        {
            break;
        }
        cleanUp = true;

        // OK we have here rtmpPacket
        if(RTMP_PACKET_TYPE_INVOKE == rtmpPacket.m_packetType)
        {
            // make sure it is a string method name we start with
            if (0 == rtmpPacket.m_nBodySize || rtmpPacket.m_body[0] != 0x02)
            {
                printDBG("%s, Sanity failed. no string method in invoke packet\n", __FUNCTION__);
                break;
            }
            
            // decode_amf
            AMFObject obj;
            int nRes = AMF_Decode(&obj, rtmpPacket.m_body, rtmpPacket.m_nBodySize, 0);
            if(nRes < 0)
            {
              printDBG("%s, error decoding invoke packet\n", __FUNCTION__);
              continue;
            }

            AVal method;
            AMFProp_GetString(AMF_GetProp(&obj, NULL, 0), &method);
            
            printDBG("%s, invoking <%s>\n", __FUNCTION__, method.av_val);
            if(string(method.av_val, method.av_len) == strMethod)
            {
                AMFObject cobj;
                AMFProp_GetObject(AMF_GetProp(&obj, NULL, 3), &cobj);
                objList = getAllProperties(cobj, std::string(obj.o_props[3].p_name.av_val, obj.o_props[3].p_name.av_len));
                if(0 != objList)
                {
                    AMF_Reset(&obj);
                    break;
                }
            }
            AMF_Reset(&obj);
            RTMP_ClientPacket(m_rtmp, &rtmpPacket);
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

