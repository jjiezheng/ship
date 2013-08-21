#pragma once

namespace ship
{
    class DataStream
    {
    public:
        virtual ~DataStream() = 0;

        virtual int size() const = 0;
        virtual int Read(char* buf, int size) = 0;
        virtual void Reset() = 0;
    };

    typedef shared_ptr<DataStream> DataStreamPtr;
}