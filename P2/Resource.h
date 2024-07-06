#ifndef SENECA_RESOURCE_H
#define SENECA_RESOURCE_H

#include <string>

namespace seneca {
    enum class NodeType {
        FILE,
        DIRECTORY
    };

    class Resource {
    protected:
        std::string m_name;
        std::string m_parent_path;

    public:
        // Default constructor
        Resource() : m_name("Unnamed") {}

        // Parameterized constructor
        Resource(const std::string& name) : m_name(name) {}

        virtual ~Resource() {}

        virtual void update_parent_path(const std::string&) = 0;
        virtual std::string name() const = 0;
        virtual std::string path() const = 0;
        virtual NodeType type() const = 0;
        virtual int count() const = 0;
        virtual size_t size() const = 0;
    };
}

#endif
