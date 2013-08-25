#include "stdafx.h"
#include "archive_reader.h"

#include "miniz.c"

namespace xutil
{
    ArchiveReader::ArchiveReader(istream& stream)
        : stream_(stream)
    {
        start_pos_ = stream_.tellg();
    }
}