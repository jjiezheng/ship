#pragma once

namespace xutil
{
    class ArchiveWriter : noncopyable
    {
    public:
        ArchiveWriter(ostream& stream);

    private:
        ostream& stream_;
    };
}