#ifndef SENECA_FLAGS_H
#define SENECA_FLAGS_H

namespace seneca
{
    // Options for displaying the files and directories
    enum class FormatFlags {
        LONG,   // to display in long format
    };

    // Determine the type of filesystem resource
    enum class NodeType {
        DIR,
        FILE
    };

    enum class OpFlags {
        RECURSIVE,
    };
}

#endif