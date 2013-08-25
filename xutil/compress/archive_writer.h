#pragma once

namespace xutil
{
    class ArchiveWriter : noncopyable
    {
    public:
        ArchiveWriter(const string& path);
        virtual ~ArchiveWriter();

        void Close();
        bool AddFile(const string& archive_name, const string& src_path);

    private:
        void* zip_obj_;
        FILE* file_;
    };
}