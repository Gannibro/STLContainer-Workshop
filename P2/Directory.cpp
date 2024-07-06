#include "Directory.h"
#include "File.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace seneca {

    Directory::Directory(const std::string& name) : Resource(name) {}

    Directory::~Directory() {
        for (auto& resource : m_contents) {
            delete resource;
        }
    }

    int Directory::count() const {
        return m_contents.size();
    }

    size_t Directory::size() const {
        size_t total_size = 0;
        for (const auto& resource : m_contents) {
            total_size += resource->size();
        }
        return total_size;
    }

    Directory* Directory::find_directory(const std::string& name) {
        for (auto& resource : m_contents) {
            if (resource->name() == name) {
                return dynamic_cast<Directory*>(resource);
            }
        }
        return nullptr;
    }

    Resource* Directory::find(const std::string& name, const std::vector<OpFlags>& flags) const {
        for (const auto& resource : m_contents) {
            if (resource->name() == name) {
                return resource;
            }

            if (std::find(flags.begin(), flags.end(), OpFlags::RECURSIVE) != flags.end()) {
                Directory* dir = dynamic_cast<Directory*>(resource);
                if (dir) {
                    Resource* found = dir->find(name, flags);
                    if (found) {
                        return found;
                    }
                }
            }
        }
        return nullptr;
    }

    void Directory::operator+=(Resource* resource) {
        for (auto& res : m_contents) {
            if (res->name() == resource->name()) {
                throw std::runtime_error(resource->name() + " already exists in the directory.");
            }
        }
        m_contents.push_back(resource);
        resource->update_parent_path(this->path());
    }

    void Directory::remove(const std::string& name, const std::vector<OpFlags>& flags) {
        auto it = std::find_if(m_contents.begin(), m_contents.end(),
            [&name](Resource* resource) { return resource->name() == name; });

        if (it != m_contents.end()) {
            Directory* dir = dynamic_cast<Directory*>(*it);
            if (dir && std::find(flags.begin(), flags.end(), OpFlags::RECURSIVE) == flags.end()) {
                throw std::invalid_argument(name + " is a directory. Pass the recursive flag to delete directories.");
            }

            delete *it;
            m_contents.erase(it);
        } else {
            throw std::string(name + " does not exist in " + this->name());
        }
    }

    void Directory::display(std::ostream& out, const std::vector<FormatFlags>& flags) const {
        out << "Total size: " << this->size() << " bytes\n";
        for (const auto& resource : m_contents) {
            if (dynamic_cast<const Directory*>(resource)) {
                out << "D | " << std::left << std::setw(15) << resource->name();
            } else {
                out << "F | " << std::left << std::setw(15) << resource->name();
            }

            if (std::find(flags.begin(), flags.end(), FormatFlags::LONG) != flags.end()) {
                if (dynamic_cast<const Directory*>(resource)) {
                    out << " | " << std::right << std::setw(2) << resource->count() << " | "
                        << std::right << std::setw(10) << resource->size() << " bytes |";
                } else {
                    out << " | " << std::right << std::setw(10) << resource->size() << " bytes |";
                }
            }
            out << '\n';
        }
    }
}
