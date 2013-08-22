#pragma once

#include "data_stream.h"

namespace ship
{
    class FileStream : public DataStream
    {
    public:
        FileStream(const string& path);
        virtual ~FileStream();

        const string& path() const { return path_; }

        virtual int size() const;
        virtual int Read(char* buf, int size);
        virtual void Reset();
    
    private:
        string path_;
    };
}
