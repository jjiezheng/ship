#include "stdafx.h"
#include "file_stream.h"

#include "file_stream.h"

namespace ship
{
    FileStream::FileStream(const string& path)
    {
        data = make_shared<FileStream>(path);
    }

    FileStream::~FileStream()
    {
    }

    int FileStream::size() const
    {
        return 0;
    }

    int FileStream::Read(char* buf, int size)
    {
        return 0;
    }

    void FileStream::Reset()
    {
    }
}