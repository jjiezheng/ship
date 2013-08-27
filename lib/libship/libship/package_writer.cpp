#include "stdafx.h"
#include "package_writer.h"

#include "xutil/compress/archive_writer.h"
#include "package/file_item.h"
#include "package/file_stream.h"

namespace ship
{
    namespace 
    {
        int FindStringEnd(FILE* f, const char* s)
        {
            int pos = 0;
            fseek(f, 0, SEEK_SET);

            const char* t = s;
            int c;
            while((c=fgetc(f)) != EOF)
            {
                ++pos;
                if (c == *t)
                {
                    ++t;
                    if (*t == '\0')
                    {
                        fseek(f, pos, SEEK_SET);
                        return pos;
                    }
                }
                else
                {
                    t = s;
                }
            }

            return 0;
        }

        void WriteIntAsString(FILE* f, int value)
        {
            char buf[10];
            const char* str_value = _itoa(value, buf, 10);
            fputs(str_value, f);
            fputc('\0', f);
            fflush(f);
        }

        bool WritePackageSfxOffset(FILE* f, int value)
        {
            int pos = FindStringEnd(f, "!PKG_SFX_OFFSET!");
            if (!pos)
            {
                return false;
            }

            WriteIntAsString(f, value);
            return true;
        }

        bool WritePackageSfxSize(FILE* f, int value)
        {
            int pos = FindStringEnd(f, "!PKG_SFX_SIZE!");
            if (!pos)
            {
                return false;
            }
            WriteIntAsString(f, value);
            return true;
        }
        
        bool WritePackageSfxExeName(FILE* f, const string& value)
        {
            int pos = FindStringEnd(f, "!PKG_SFX_EXE_NAME!");
            if (!pos)
            {
                return false;
            }
            fputs(value.c_str(), f);
            fputc('\0', f);
            fflush(f);
            return true;
        }

        bool WritePackageDataOffset(FILE* f, int value)
        {
            int pos = FindStringEnd(f, "!PKG_DATA_OFFSET!");
            if (!pos)
            {
                return false;
            }
            WriteIntAsString(f, value);
            return true;
        }

        bool WritePackageDataSize(FILE* f, int value)
        {
            int pos = FindStringEnd(f, "!PKG_DATA_SIZE!");
            if (!pos)
            {
                return false;
            }
            WriteIntAsString(f, value);
            return true;
        }
    }

    PackageWriter::PackageWriter(const string& stub_path)
        : stub_path_(stub_path)
    {
        if (!is_regular_file(stub_path))
        {
            THROW_EXCEPTION("Stub does not exist");
        }
    }

    void PackageWriter::Compose(const string& path, const PackagePtr& package)
    {
        copy_file(stub_path_, path, copy_option::overwrite_if_exists);
        FILE* f = fopen(path.c_str(), "r+b");
        if (!f)
        {
            THROW_EXCEPTION("asda");
        }
        fseek(f, 0, SEEK_END);
        int sfx_start = ftell(f);

        if (!WritePackageSfxOffset(f, sfx_start))
        {
            THROW_EXCEPTION("Failed to write SFX start pos");
        }

        ArchiveWriter writer(f, sfx_start);
        for (auto i = package->content.begin(); i != package->content.end(); ++i)
        {
            const PackageItemPtr& item = *i;
            FileItemPtr file_item = dynamic_pointer_cast<FileItem>(item);
            if (file_item)
            {
                const DataStreamPtr& data = file_item->data;
                FileStreamPtr file_data = dynamic_pointer_cast<FileStream>(data);
                if (file_data)
                {
                    cout << file_item->name << " - " <<  file_data->path() << endl;
                    writer.AddFile(file_item->name, file_data->path());
                }
            }
        }
        writer.Close();

        fseek(f, 0, SEEK_END);
        int sfx_size = ftell(f) - sfx_start;
        if (!WritePackageSfxSize(f, sfx_size))
        {
            THROW_EXCEPTION("Failed to write SFX size");
        }

        fclose(f);
    }
}