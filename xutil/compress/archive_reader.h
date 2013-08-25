#pragma once

namespace xutil
{
    class ArchiveReader : noncopyable
    {
    public:
        ArchiveReader(istream& stream);

    private:
        istream& stream_;
        istream::pos_type start_pos_;
    };
}