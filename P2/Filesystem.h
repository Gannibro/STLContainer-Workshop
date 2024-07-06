#ifndef SENECA_FILESYSTEM_H
#define SENECA_FILESYSTEM_H

#include <string>
#include "Directory.h"

namespace seneca {

class Filesystem {
private:
    Directory* m_root;
    Directory* m_current;

public:
    Filesystem(const std::string& filename, const std::string& root_name = "");
    ~Filesystem();

    // Disable copy operations
    Filesystem(const Filesystem&) = delete;
    Filesystem& operator=(const Filesystem&) = delete;

    // Enable move operations
    Filesystem(Filesystem&&) noexcept;
    Filesystem& operator=(Filesystem&&) noexcept;

    Filesystem& operator+=(Resource* resource);
    Directory* change_directory(const std::string& dir_name = "");
    Directory* get_current_directory() const;
};

}

#endif // SENECA_FILESYSTEM_H