#include <iostream>
#include <iomanip>
#include <exception>
#include <string>
#include <type_traits>
#include <forward_list>
#include "Filesystem.h"
#include "Filesystem.h" // intentional
#include "Directory.h"
#include "Directory.h" // intentional
#include "File.h"
#include "File.h" // intentional
#include "Flags.h"
#include "Flags.h" // intentional

int cout = 0; // won't compile if headers don't follow convention

void printLine() {
    std::cout << "--------------------------\n";
}

void printHeader(const std::string& label) {
    printLine();
    std::cout << std::setw((26 - label.length()) / 2 + label.length()) << label << std::endl;
    printLine();
}

int main(int argc, char** argv) {
    std::cout << "Command Line:\n";
    printLine();
    for (int i = 0; i < argc; ++i)
        std::cout << std::setw(3) << i + 1 << ": " << argv[i] << '\n';
    printLine();
    std::cout << "\n";

    printHeader("FILESYSTEM");
    
    try {
        seneca::Filesystem failedFs("non-existentfile");
    } catch (...) {
        std::cout << "**EXPECTED EXCEPTION: Filesystem not created with invalid filename.\n\n";
    }

    seneca::Filesystem fs(argv[1]);

    std::vector<seneca::FormatFlags> fflags;
    std::vector<seneca::OpFlags> oflags;
    oflags.push_back(seneca::OpFlags::RECURSIVE);

    seneca::Directory* working_dir = fs.get_current_directory();
    working_dir->display(std::cout);

    printLine();

    printHeader("CHANGE DIR");
    fflags.push_back(seneca::FormatFlags::LONG);

    try {
        working_dir = fs.change_directory("pics");
    } catch(std::invalid_argument&) {
        std::cout << "**EXPECTED EXCEPTION: Couldn't change directory to invalid directory.\n\n";
    }

    working_dir = fs.change_directory("images");
    working_dir->display(std::cout, fflags);

    printLine();

    printHeader("FIND");

    seneca::File* elephant_image = dynamic_cast<seneca::File*>(working_dir->find("elephant", oflags));
    if(!elephant_image) {
        std::cout << "**EXPECTED ERROR: File elephant not found in /images/ recursively\n";
    }

    elephant_image = dynamic_cast<seneca::File*>(working_dir->find("elephant.png"));
    if (!elephant_image) {
        std::cout << "**EXPECTED ERROR: File elephant.png not found in /images/ non-recursively\n";
    }

    elephant_image = dynamic_cast<seneca::File*>(working_dir->find("elephant.png", oflags));
    std::cout << "/images/pngs/elephant.png was found in fileystem\n";

    printLine();

    printHeader("ADD TO DIRECTORY");
    seneca::Directory* classified = new seneca::Directory("classified");
    *classified += new seneca::File(".aliens.txt", "Are aliens real? Go to Area 51 and find out!");
    *classified += new seneca::File(".polls.txt", "Polling results for the current election are in here.");

    std::cout << "Created directory classified/\n";
    classified->display(std::cout, fflags);

    std::cout << "\n";
    working_dir = fs.change_directory();
    working_dir = fs.change_directory("documents");
    std::cout << "Adding classified/ to /documents/\n";
    *working_dir += classified;
    working_dir->display(std::cout, fflags);

    printLine();

    printHeader("REMOVE");

    working_dir = fs.change_directory();
    std::cout << "Current size of filesystem is 515 bytes\n";
    std::cout << "Current size of documents/ is 207 bytes\n\n";

    try {
        working_dir->remove("documents");
    } catch(...) {
        std::cout << "**EXPECTED EXCEPTION: Trying to remove a directory without passing the recursive flag.\n\n";
    }

    working_dir->remove("documents", oflags);

    std::cout << "After removing documents/\n";
    working_dir->display(std::cout, fflags);

    printLine();

    return 0;
}