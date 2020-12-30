#pragma once

#include <sstream>
#include <string>
#include <iomanip>

template <typename Ty>
inline std::ostream& writeStreamBinary(std::ostream& out, const Ty& data)
{
    out.write(reinterpret_cast<const char*>(&data), sizeof(Ty));
    return out;
}

inline std::ostream& writeStreamHex(std::ostream& out, const std::string& str)
{
    for (char ch : str)
    {
        out << std::hex << std::setfill('0') << std::setw(2) << (int)ch << " ";
    }
    return out;
}
