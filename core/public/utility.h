#ifndef INCLUDED_UTILITY_H
#define INCLUDED_UTILITY_H

#include <optional>
#include <string>
#include <vector>

namespace utility
{
std::optional<std::string> LoadTextFile(const std::string &filename);
std::vector<std::string> Split(const std::string &s, const std::string &sep);
} // namespace utility

#endif
