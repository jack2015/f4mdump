#ifndef _STREAM_READER_H_
#define _STREAM_READER_H_

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "SimpleDataTypes.h"

enum EOffsetwhence
{
    READER_SEEK_SET=0,
    READER_SEEK_CUR=1,
    READER_SEEK_END=2,
};

class IStreamReader
{
public:
    virtual int64_t read(void *buf, uint32_t count) = 0;
    virtual int64_t seek(int32_t offset, EOffsetwhence whence) = 0;
    virtual int64_t size() = 0;
    virtual int64_t offset() = 0;
    
    virtual uint8_t  readUInt8();
    virtual uint32_t readUInt24();
    virtual uint32_t readUInt32();
    virtual uint64_t readUInt64();
    virtual std::string readString();
};

class CBufferReader : public IStreamReader
{
public:
    CBufferReader(const ByteBuffer_t &buffer);
    virtual ~CBufferReader();
    
    virtual int64_t read(void *buf, uint32_t count);
    virtual int64_t seek(int32_t offset, EOffsetwhence whence);
    virtual int64_t size();
    virtual int64_t offset();
private:
    const ByteBuffer_t &m_buffer;
    int64_t     m_size;
    int64_t     m_offset;
    
};


#endif //_STREAM_READER_H_
