#include "stdafx.h"
#include "archive_reader.h"

#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"

namespace xutil
{
    ArchiveReader::ArchiveReader(FILE* file, size_t archive_offset, size_t archive_size)
        : file_(file),
          archive_offset_(archive_offset),
          archive_size_(archive_size),
          zip_obj_(NULL)
    {
        mz_zip_archive* zip = new mz_zip_archive();
        memset(zip, 0, sizeof(*zip));
        
        mz_bool rc = mz_zip_reader_init_file_block(zip, file, archive_offset_, archive_size_, 0);
        if (!rc)
        {
            throw exception("Failed to init archive reader");
        }

        zip_obj_ = zip;
    }

    ArchiveReader::~ArchiveReader()
    {
        if (zip_obj_)
        {
            Close();
        }
    }

    void ArchiveReader::Close()
    {
        mz_zip_archive* zip = (mz_zip_archive*)zip_obj_;
        mz_zip_reader_end(zip);
        delete zip;

        zip_obj_ = NULL;
    }

    bool ArchiveReader::ExtractFile(const string& archive_path, const string& dst_path)
    {
        mz_zip_archive* zip = (mz_zip_archive*)zip_obj_;
        return !!mz_zip_reader_extract_file_to_file(zip, archive_path.c_str(), dst_path.c_str(), 0);
    }
}