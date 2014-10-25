#ifndef _RTMP_WRAPPER_H_
#define _RTMP_WRAPPER_H_

#include <map>
#include <list>
#include <string>
#include "RTMPWrapper.h"

extern "C"
{
#include <librtmp/rtmp.h>
#include <librtmp/amf.h>
}

namespace rtmp
{

enum RTMPObjectType
{
    RTMP_NUMBER_TYPE = 0,
    RTMP_BOOLEAN_TYPE,
    RTMP_STRING_TYPE,
    RTMP_LIST_TYPE
};

class RTMPItem;
typedef std::list<RTMPItem*> RTMPItems;

class RTMPItem
{
public:
    RTMPItem(const std::string &name)
    : m_name(name)
    {
    }
    virtual ~RTMPItem()
    {
    }
    virtual RTMPObjectType getType() = 0;
    virtual std::string getName()
    {
        return m_name;
    }
protected:
    std::string m_name;
    
};

class RTMPBool : public RTMPItem
{
public:
    RTMPBool(const std::string &name, const double value)
    : RTMPItem(name)
    , m_value(value)
    {
    }
    
    virtual ~RTMPBool()
    { 
        //printDBG("->| destroy[%d] [%s]\n", getType(), getName().c_str());
    }
    
    bool getValue()
    {
        return m_value;
    }
    
    virtual RTMPObjectType getType()
    {
        return RTMP_BOOLEAN_TYPE;
    }
    
private:
    bool m_value;
};

class RTMPNumber : public RTMPItem
{
public:
    RTMPNumber(const std::string &name, const double value)
    : RTMPItem(name)
    , m_value(value)
    {  }
    
    virtual ~RTMPNumber()
    {
        //printDBG("->| destroy[%d] [%s]\n", getType(), getName().c_str());
    }
    
    double getValue()
    {
        return m_value;
    }
    
    virtual RTMPObjectType getType()
    {
        return RTMP_NUMBER_TYPE;
    }
    
private:
    double m_value;
};

class RTMPString : public RTMPItem
{
public:
    RTMPString(const std::string &name, const std::string &value)
    : RTMPItem(name)
    , m_value(value)
    { }
    
    virtual ~RTMPString()
    {
        //printDBG("->| destroy[%d] [%s]\n", getType(), getName().c_str());
    }
    
    std::string getValue()
    {
        return m_value;
    }
    
    virtual RTMPObjectType getType()
    {
        return RTMP_STRING_TYPE;
    }
    
private:
    std::string m_value;
};


class RTMPList : public RTMPItem
{
public:
    RTMPList(const std::string &name)
    : RTMPItem(name)
    { }
    
    virtual ~RTMPList()
    {
        for(RTMPItems::iterator it = m_items.begin(); it != m_items.end(); ++it)
        {
            delete (*it);
        }
        //printDBG("->| destroy[%d] [%s]\n", getType(), getName().c_str());
    }
    
    virtual RTMPObjectType getType()
    {
        return RTMP_LIST_TYPE;
    }
    
    RTMPItems& getValue()
    {
        return m_items;
    }

    virtual void append(RTMPItem *item)
    {
        if(0 != item)
        {
            m_items.push_back(item);
        }
    }
    
    virtual RTMPItem* operator[](const std::string &name)
    {
        RTMPItem *obj = 0;
        for(RTMPItems::iterator it = m_items.begin(); it != m_items.end(); ++it)
        {
            if((*it)->getName() == name)
            {
                obj = (*it);
                break;
            }
        }
        return obj;
    }
    
private:
    RTMPItems m_items;
};

bool GetStringItem(RTMPItem *rtmpItem, std::string &value);
bool GetNumberItem(RTMPItem *rtmpItem, double &value);
bool GetBoolItem(RTMPItem *rtmpItem, bool &value);
bool GetListItem(RTMPItem *rtmpItem, RTMPItems *&value);
bool GetStringItem(RTMPItem *rtmpItem, const std::string &name, std::string &value);
bool GetNumberItem(RTMPItem *rtmpItem, const std::string &name, double &value);
bool GetBoolItem(RTMPItem *rtmpItem, const std::string &name, bool &value);
bool GetListItem(RTMPItem *rtmpItem, const std::string &name, RTMPItems *&value);


typedef std::pair< std::string, std::string>    RTMPOption_t;       //first key, second value
typedef std::list< RTMPOption_t >               RTMPOptionsList_t;   //first key, second value

typedef std::map< double, uint8_t >  InvokeResults_t;

class CRTMP
{
public:
    explicit CRTMP(const std::string &rtmpUrl, const RTMPOptionsList_t &rtmpParams = RTMPOptionsList_t() );
    
    virtual ~CRTMP();

    virtual void connect();
    
    virtual void close();
    
    virtual void terminate();

    virtual void set_options(const RTMPOptionsList_t &rtmpOprions);
    
    virtual void set_option(const RTMPOption_t &option);
    
    virtual RTMPList* handleServerInvoke(const std::string &strMethod, const uint32_t timeout=static_cast<uint32_t>(-1));
    
private:
    void set_option(const std::string &key, const std::string &value);
    bool isConnected();
    bool read_packet(RTMPPacket &rtmpPacket);

    void startInternalTimeout(const uint32_t timeout);
    bool isInternalTimeout();

    uint32_t m_internalTimeout;
    uint32_t m_startInternalTimeout;
    bool m_bInternalTimeout;
    
    bool m_bTerminate;
    RTMP *m_rtmp;
    std::string m_rtmpUrl;
    RTMPOptionsList_t m_rtmpParams;
    InvokeResults_t m_invoke_results;
    
    
};

}

#endif // _RTMP_WRAPPER_H_