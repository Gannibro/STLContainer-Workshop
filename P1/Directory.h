#ifndef SENECA_DIRECTORY_H
#define SENECA_DIRECTORY_H

#include <string>
#include <vector>
#include <stdexcept>
#include "Resource.h"
#include "Flags.h"

namespace seneca {

    class Directory : public Resource {
    private:
        std::string m_name;
        std::vector<Resource*> m_contents;
        std::string m_parent_path;

        void update_resource_paths() {
            for (auto& resource : m_contents) {
                resource->update_parent_path(this->path());
            }
        }

    public:
        Directory(const std::string& name)
            : m_name(name) {}

        void update_parent_path(const std::string& parent_path) override {
            m_parent_path = parent_path;
            update_resource_paths();
        }

        NodeType type() const override {
            return NodeType::DIR;
        }

        std::string path() const override {
            return m_parent_path + m_name;
        }

        std::string name() const override {
            return m_name;
        }

        int count() const override {
            return m_contents.size();
        }

        size_t size() const override {
            size_t total_size = 0;
            for (const auto& resource : m_contents) {
                total_size += resource->size();
            }
            return total_size;
        }

        Directory& operator+=(Resource* resource) {
            for (const auto& existing_resource : m_contents) {
                if (existing_resource->name() == resource->name()) {
                    throw std::runtime_error("Resource with the same name already exists in the directory");
                }
            }
            m_contents.push_back(resource);
            resource->update_parent_path(this->path());
            return *this;
        }

        Resource* find(const std::string& name, const std::vector<OpFlags>& flags = {}) {
            for (const auto& resource : m_contents) {
                if (resource->name() == name) {
                    return resource;
                }
            }
            for (const auto& flag : flags) {
                if (flag == OpFlags::RECURSIVE) {
                    for (const auto& resource : m_contents) {
                        if (resource->type() == NodeType::DIR) {
                            Resource* found = dynamic_cast<Directory*>(resource)->find(name, flags);
                            if (found) {
                                return found;
                            }
                        }
                    }
                }
            }
            return nullptr;
        }

        ~Directory() {
            for (auto& resource : m_contents) {
                delete resource;
            }
        }

        // Prevent copy/move operations
        Directory(const Directory&) = delete;
        Directory& operator=(const Directory&) = delete;
        Directory(Directory&&) = delete;
        Directory& operator=(Directory&&) = delete;
    };

}

#endif // SENECA_DIRECTORY_H
