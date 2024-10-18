/// @author Jakub Antonín Štigler

#pragma once

#include <string>

namespace p2np {

/// @brief Gets the error message of the last error stored in errno.
std::string strerror();

/// @brief Gets the error message for the given C error.
std::string strerror(int err);

} // namespace p2np
