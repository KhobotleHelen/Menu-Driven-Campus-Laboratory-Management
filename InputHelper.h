// InputHelper.h   (INTEGRATION - shared input functions)
// Safe keyboard input used by every menu. Combines the readInt / readText helpers written by Students B and C.
// Typing letters where a number is expected never crashes the program: the user is asked again.
#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <string>

int readInt(const std::string& prompt, int minimum, int maximum);
// Asks until the user types a whole number between minimum and maximum (inclusive).

std::string readText(const std::string& prompt);
// Reads a whole line (spaces allowed). The '|' character is replaced by '/' because '|' separates fields in the data files.

std::string readNonEmpty(const std::string& prompt);
// Like readText but asks again until something other than blanks is typed.

std::string toLower(const std::string& text);
// Lower-case copy of a string (used for case-insensitive comparisons).

void pressEnter();
// Waits for the user to press Enter (keeps menu output readable).

#endif
