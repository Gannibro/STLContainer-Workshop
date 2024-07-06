#include "File.h"

namespace seneca {

File::File(const std::string& name, const std::string& contents)
    : Resource(name), m_contents(contents) {}

void File::update_parent_path(const std::string& parent_path) {
    m_parent_path = parent_path;
}

std::string File::name() const {
    return m_name;
}

int File::count() const {
    return -1;  // Files don't have a count, so we return -1 as per the specification
}

std::string File::path() const {
    return m_parent_path + m_name;
}

size_t File::size() const {
    return m_contents.size();
}

NodeType File::type() const {
    return NodeType::FILE;
}

}