#pragma once

#include <sstream>

template <typename Ty>
inline void writeStreamBinary(std::ostream& out, const Ty& data)
{
    out.write(reinterpret_cast<const char*>(&data), sizeof(Ty));
}