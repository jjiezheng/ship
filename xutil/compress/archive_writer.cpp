#include "stdafx.h"
#include "archive_writer.h"

namespace xutil
{
    ArchiveWriter::ArchiveWriter(ostream& stream)
        : stream_(stream)
    {
    }
}