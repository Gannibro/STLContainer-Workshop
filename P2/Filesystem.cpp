#include "Filesystem.h"
#include "File.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace seneca {

Filesystem::Filesystem(const std::string& filename, const std::string& root_name) {
    m_root = new Directory(root_name);
    m_current = m_root;

    std::ifstream file(filename);
    if (!file) {
        delete m_root;
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string path, content;
        std::getline(iss, path, '|');
        std::getline(iss, content);

        // Trim leading and trailing spaces
        path.erase(0, path.find_first_not_of(" \t"));
        path.erase(path.find_last_not_of(" \t") + 1);
        content.erase(0, content.find_first_not_of(" \t"));
        content.erase(content.find_last_not_of(" \t") + 1);

        std::istringstream path_stream(path);
        std::string component;
        Directory* current_dir = m_root;

        while (std::getline(path_stream, component, '/')) {
            if (component.empty()) continue;

            if (path_stream.peek() != EOF) {
                // This is a directory
                Resource* found = current_dir->find(component);
                if (!found) {
                    Directory* new_dir = new Directory(component);
                    *current_dir += new_dir;
                    current_dir = new_dir;
                } else if (found->type() == NodeType::DIR) {
                    current_dir = dynamic_cast<Directory*>(found);
                } else {
                    throw std::runtime_error("File found where directory expected: " + component);
                }
            } else {
                // This is a file
                if (!content.empty()) {
                    File* new_file = new File(component, content);
                    *current_dir += new_file;
                } else {
                    Directory* new_dir = new Directory(component);
                    *current_dir += new_dir;
                }
            }
        }
    }
}

Filesystem::~Filesystem() {
    delete m_root;
}

Filesystem::Filesystem(Filesystem&& other) noexcept
    : m_root(other.m_root), m_current(other.m_current) {
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

Filesystem& Filesystem::operator+=(Resource* resource) {
    *m_current += resource;
    return *this;
}

Directory* Filesystem::change_directory(const std::string& dir_name) {
    if (dir_name.empty()) {
        m_current = m_root;
    } else {
        Resource* found = m_current->find(dir_name);
        if (!found || found->type() != NodeType::DIR) {
            throw std::invalid_argument("Cannot change directory! " + dir_name + " not found!");
        }
        m_current = dynamic_cast<Directory*>(found);
    }
    return m_current;
}

Directory* Filesystem::get_current_directory() const {
    return m_current;
}

}