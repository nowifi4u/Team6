#pragma once

class BinCharIStream
{
    char const* m_data;
    size_t m_offset = 0;
    const bool m_todelete;

public:

    BinCharIStream(const char* str, bool todelete = false)
        : m_data(str), m_todelete(todelete) {}

    ~BinCharIStream()
    {
        if (m_todelete && m_data != nullptr)
            delete m_data;
    }

    size_t get_offset() const
    {
        return m_offset;
    }

    template <class Ty>
    const Ty& get()
    {
        return *reinterpret_cast<const Ty*>(m_data + m_offset);
    }

    template <class Ty>
    const Ty& read()
    {
        const Ty& result = get<Ty>();
        m_offset += sizeof(Ty);
        return result;
    }

    const char* release()
    {
        return std::exchange(m_data, nullptr);
    }
};

class BinCharOStream
{
    char* m_data;
    size_t m_offset = 0;
    const bool m_todelete;

public:

    BinCharOStream(char* str, bool todelete = false)
        : m_data(str), m_todelete(todelete) {}

    BinCharOStream(size_t size)
        : BinCharOStream(new char[size], true) {}

    ~BinCharOStream()
    {
        if (m_todelete && m_data != nullptr)
            delete m_data;
    }

    size_t get_offset() const
    {
        return m_offset;
    }

    template <class Ty>
    Ty& get()
    {
        return *reinterpret_cast<Ty*>(m_data + m_offset);
    }

    template <class Ty>
    Ty& read()
    {
        Ty& result = get<Ty>();
        m_offset += sizeof(Ty);
        return result;
    }

    char* release()
    {
        return std::exchange(m_data, nullptr);
    }
};