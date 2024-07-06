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

public:
    Directory(const std::string& name);
    ~Directory();

    void update_parent_path(const std::string& parent_path) override;
    std::string name() const override;
    int count() const override;
    std::string path() const override;
    size_t size() const override;
    NodeType type() const override;

    Directory& operator+=(Resource* resource);
    void remove(const std::string& name, const std::vector<OpFlags>& flags = {});
    Resource* find(const std::string& name, const std::vector<OpFlags>& flags = {}) const;
    void display(std::ostream& out, const std::vector<FormatFlags>& flags = {}) const;
};

}

#endif