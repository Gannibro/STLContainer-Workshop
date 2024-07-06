#ifndef SENECA_DIRECTORY_H
#define SENECA_DIRECTORY_H

#include "Resource.h"
#include "Flags.h"
#include <vector>
#include <ostream>

namespace seneca {

    class Directory : public Resource {
    private:
        std::vector<Resource*> m_contents;
        Directory* find_directory(const std::string& name);

    public:
        Directory(const std::string& name);
        ~Directory();

        int count() const override;
        size_t size() const override;
        void operator+=(Resource* resource);
        void remove(const std::string& name, const std::vector<OpFlags>& flags = {});
        Resource* find(const std::string& name, const std::vector<OpFlags>& flags = {}) const;
        void display(std::ostream& out, const std::vector<FormatFlags>& flags = {}) const;

        void update_parent_path(const std::string& parent_path) override {
            m_parent_path = parent_path;
            for (auto& resource : m_contents) {
                resource->update_parent_path(this->path());
            }
        }

        std::string name() const override {
            return m_name;
        }

        std::string path() const override {
            return m_parent_path + m_name;
        }

        NodeType type() const override {
            return NodeType::DIRECTORY;
        }
    };
}

#endif
