#include "stdafx.h"
#include "archive_writer.h"

#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"

namespace xutil
{
    ArchiveWriter::ArchiveWriter(FILE* file, size_t archive_offset)
        : zip_obj_(NULL)
    {
        mz_zip_archive* zip = new mz_zip_archive();
        memset(zip, 0, sizeof(*zip));

        mz_bool rc =  mz_zip_writer_init_file_block(zip, file, archive_offset, 0);
        if (!rc)
        {
            THROW_EXCEPTION("Failed to init archive writer");
        }

        zip_obj_ = zip;
    }

    ArchiveWriter::ArchiveWriter(const string& path)
        : zip_obj_(NULL)
    {
        mz_zip_archive* zip = new mz_zip_archive();
        memset(zip, 0, sizeof(*zip));

        mz_bool rc = mz_zip_writer_init_file(zip, path.c_str(), 0);
        if (!rc)
        {
            THROW_EXCEPTION("Failed to init archive writer");
        }

        zip_obj_ = zip;
    }

    ArchiveWriter::~ArchiveWriter()
    {
        if (zip_obj_)
        {
            Close();
        }
    }

    void ArchiveWriter::Close()
    {
        mz_zip_archive* zip = (mz_zip_archive*)zip_obj_;
        mz_zip_writer_finalize_archive(zip);
        mz_zip_writer_end(zip);
        delete zip;

        zip_obj_ = NULL;
    }

    bool ArchiveWriter::AddFile(const string& archive_name, const string& src_path)
    {
        mz_zip_archive* zip = (mz_zip_archive*)zip_obj_;
        return !!mz_zip_writer_add_file(zip, archive_name.c_str(), src_path.c_str(), NULL, 0, MZ_BEST_COMPRESSION);
    }

}