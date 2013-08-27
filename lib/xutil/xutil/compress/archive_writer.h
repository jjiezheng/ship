#pragma once

namespace xutil
{
    class ArchiveWriter : noncopyable
    {
    public:
        ArchiveWriter(const string& path);
        ArchiveWriter(FILE* file, size_t archive_offset);
        virtual ~ArchiveWriter();

        void Close();
        bool AddFile(const string& archive_name, const string& src_path);

    private:
        void* zip_obj_;
    };
}