#include "Directory.h"
#include <algorithm>
#include <stdexcept>
#include <iomanip>

namespace seneca {

Directory::Directory(const std::string& name) : Resource(name) {}

Directory::~Directory() {
    for (auto& resource : m_contents) {
        delete resource;
    }
}

void Directory::update_parent_path(const std::string& parent_path) {
    m_parent_path = parent_path;
    for (auto& resource : m_contents) {
        resource->update_parent_path(path());
    }
}

std::string Directory::name() const {
    return m_name;
}

int Directory::count() const {
    return static_cast<int>(m_contents.size());
}

std::string Directory::path() const {
    return m_parent_path + m_name + "/";
}

size_t Directory::size() const {
    size_t total_size = 0;
    for (const auto& resource : m_contents) {
        total_size += resource->size();
    }
    return total_size;
}

NodeType Directory::type() const {
    return NodeType::DIR;
}

Directory& Directory::operator+=(Resource* resource) {
    auto it = std::find_if(m_contents.begin(), m_contents.end(),
        [&](const Resource* r) { return r->name() == resource->name(); });
    
    if (it != m_contents.end()) {
        throw std::runtime_error("Resource with the same name already exists in the directory");
    }
    
    m_contents.push_back(resource);
    resource->update_parent_path(this->path());
    return *this;
}

void Directory::remove(const std::string& name, const std::vector<OpFlags>& flags) {
    auto it = std::find_if(m_contents.begin(), m_contents.end(),
        [&](const Resource* r) { return r->name() == name; });
    
    if (it == m_contents.end()) {
        throw std::runtime_error(name + " does not exist in " + this->name());
    }
    
    if ((*it)->type() == NodeType::DIR && 
        std::find(flags.begin(), flags.end(), OpFlags::RECURSIVE) == flags.end()) {
        throw std::invalid_argument(name + " is a directory. Pass the recursive flag to delete directories.");
    }
    
    delete *it;
    m_contents.erase(it);
}

Resource* Directory::find(const std::string& name, const std::vector<OpFlags>& flags) const {
    auto it = std::find_if(m_contents.begin(), m_contents.end(),
        [&](const Resource* r) { return r->name() == name; });
    
    if (it != m_contents.end()) {
        return *it;
    }
    
    if (std::find(flags.begin(), flags.end(), OpFlags::RECURSIVE) != flags.end()) {
        for (const auto& resource : m_contents) {
            if (resource->type() == NodeType::DIR) {
                Resource* found = dynamic_cast<Directory*>(resource)->find(name, flags);
                if (found) {
                    return found;
                }
            }
        }
    }
    
    return nullptr;
}

void Directory::display(std::ostream& os, const std::vector<FormatFlags>& flags) const {
    os << "Total size: " << size() << " bytes\n";
    
    for (const auto& resource : m_contents) {
        os << (resource->type() == NodeType::DIR ? "D" : "F") << " | "
           << std::left << std::setw(15);
        
        if (resource->type() == NodeType::DIR) {
            os << (resource->name() + "/");
        } else {
            os << resource->name();
        }
        
        os << " |";
        
        if (std::find(flags.begin(), flags.end(), FormatFlags::LONG) != flags.end()) {
            if (resource->type() == NodeType::DIR) {
                os << std::right << std::setw(3) << resource->count() << " |";
            } else {
                os << "    |"; // Four spaces for files
            }
            os << std::right << std::setw(10) << resource->size() << " bytes |";
        }
        os << '\n';
    }
}

}