#include "Filesystem.h"
#include "File.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace seneca {

    void Filesystem::load_filesystem(const std::string& file_name) {
        std::ifstream file(file_name);
        if (!file) {
            throw std::runtime_error("Cannot open file: " + file_name);
        }

        std::string line;
        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            std::string path, contents;
            size_t pos = line.find('|');
            if (pos != std::string::npos) {
                path = line.substr(0, pos);
                contents = line.substr(pos + 1);
                contents.erase(0, contents.find_first_not_of(" \t"));
                contents.erase(contents.find_last_not_of(" \t") + 1);
            } else {
                path = line;
            }

            Directory* current = m_root;
            std::stringstream ss(path);
            std::string component;
            while (std::getline(ss, component, '/')) {
                if (component.empty()) continue;

                Resource* found = current->find(component);
                if (!found) {
                    if (ss.peek() == EOF) {
                        current->operator+=(new File(component, contents));
                    } else {
                        Directory* new_dir = new Directory(component + "/");
                        current->operator+=(new_dir);
                        current = new_dir;
                    }
                } else {
                    current = dynamic_cast<Directory*>(found);
                    if (!current) {
                        throw std::runtime_error("Invalid directory structure in " + path);
                    }
                }
            }
        }
    }

    Filesystem::Filesystem(const std::string& file_name, const std::string& root_name) {
        m_root = new Directory(root_name.empty() ? "" : root_name + "/");
        m_current = m_root;
        load_filesystem(file_name);
    }

    Filesystem::Filesystem(Filesystem&& other) noexcept : m_root(other.m_root), m_current(other.m_current) {
        other.m_root = nullptr;
        other.m_current = nullptr;
    }

    Filesystem& Filesystem::operator=(Filesystem&& other) noexcept {
        if (this != &other) {
            delete m_root;
            m_root = other.m_root;
            m_current = other.m_current;
            other.m_root = nullptr;
            other.m_current = nullptr;
        }
        return *this;
    }

    Filesystem::~Filesystem() {
        delete m_root;
    }

    Filesystem& Filesystem::operator+=(Resource* resource) {
        m_current->operator+=(resource);
        return *this;
    }

    Directory* Filesystem::change_directory(const std::string& dir_name) {
        if (dir_name.empty()) {
            m_current = m_root;
            return m_current;
        }

        Resource* found = m_current->find(dir_name);
        Directory* dir = dynamic_cast<Directory*>(found);
        if (!dir) {
            throw std::invalid_argument("Cannot change directory! " + dir_name + " not found!");
        }

        m_current = dir;
        return m_current;
    }

    Directory* Filesystem::get_current_directory() const {
        return m_current;
    }
}
