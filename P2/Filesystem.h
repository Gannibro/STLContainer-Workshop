#ifndef SENECA_FILESYSTEM_H
#define SENECA_FILESYSTEM_H

#include <string>
#include "Directory.h"

namespace seneca {
    class Filesystem {
    private:
        Directory* m_root;
        Directory* m_current;

        void load_filesystem(const std::string& file_name);

    public:
        Filesystem(const std::string& file_name, const std::string& root_name = "");
        Filesystem(const Filesystem&) = delete;
        Filesystem& operator=(const Filesystem&) = delete;
        Filesystem(Filesystem&& other) noexcept;
        Filesystem& operator=(Filesystem&& other) noexcept;
        ~Filesystem();

        Filesystem& operator+=(Resource* resource);

        Directory* change_directory(const std::string& dir_name = "");
        Directory* get_current_directory() const;
    };
}

#endif
