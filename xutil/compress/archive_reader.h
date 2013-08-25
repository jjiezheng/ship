#pragma once

namespace xutil
{
    class ArchiveReader : noncopyable
    {
    public:
        ArchiveReader(FILE* file, size_t archive_offset, size_t archive_size);
        virtual ~ArchiveReader();

        void Close();
        bool ExtractFile(const string& archive_path, const string& dst_path);

    private:
        void* zip_obj_;

        FILE* file_;
        size_t archive_offset_;
        size_t archive_size_;
    };
}