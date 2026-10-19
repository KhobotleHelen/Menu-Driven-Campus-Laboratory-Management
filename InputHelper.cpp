// InputHelper.cpp   (INTEGRATION)
#include "InputHelper.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <sstream>

// If the input stream is closed (Ctrl+Z / Ctrl+D or end of a piped file) we must stop, otherwise
// the menus would ask the same question forever.
static void exitIfInputClosed()
{
    if (std::cin.eof()) {
        std::cout << "\nInput closed. Exiting.\n";
        std::exit(0);
    }
}

// Read a whole number inside a range.
int readInt(const std::string& prompt, int minimum, int maximum)
{
    while (true) {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        // Read the whole line, so stray characters cannot stay in the input buffer.

        exitIfInputClosed();

        std::istringstream stream(line);
        int value;
        char extra;
        // Try to read ONE integer and make sure nothing else follows it.

        if ((stream >> value) && !(stream >> extra)) {
            if (value >= minimum && value <= maximum) {
                return value;
                // Valid.
            }
            std::cout << "Please enter a number from " << minimum << " to " << maximum << ".\n";
        } else {
            std::cout << "Please enter a valid number.\n";
        }
    }
}

// Read a line of text; '|' is not allowed because it separates fields in the data files.
std::string readText(const std::string& prompt)
{
    std::cout << prompt;
    std::string value;
    std::getline(std::cin, value);
    exitIfInputClosed();

    for (std::size_t i = 0; i < value.size(); i++) {
        if (value[i] == '|') value[i] = '/';
        // Replace the field separator so it cannot corrupt a saved file.
    }
    if (!value.empty() && value[value.size() - 1] == '\r') {
        value.erase(value.size() - 1);
        // Remove a Windows carriage return if present.
    }
    return value;
}

// Keep asking until the user types something that is not empty/blank.
std::string readNonEmpty(const std::string& prompt)
{
    while (true) {
        std::string value = readText(prompt);

        std::size_t first = 0;
        while (first < value.size() && std::isspace(static_cast<unsigned char>(value[first]))) first++;
        // Skip leading spaces.

        if (first < value.size()) {
            return value.substr(first);
            // Return the text without the leading spaces.
        }
        std::cout << "This field cannot be empty.\n";
    }
}

// Lower-case copy.
std::string toLower(const std::string& text)
{
    std::string result = text;
    for (std::size_t i = 0; i < result.size(); i++) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

// Pause until Enter is pressed.
void pressEnter()
{
    std::cout << "\nPress Enter to continue...";
    std::string ignored;
    std::getline(std::cin, ignored);
    exitIfInputClosed();
}
