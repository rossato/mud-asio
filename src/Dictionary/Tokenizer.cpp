#include "Tokenizer.hpp"

using namespace Mud::Dictionary;

// UTF-8 can't be supported without supporting Telnet negotiation for binary mode.
// After that, there may be some griefing/security consequences to consider before enabling it.

namespace
{
    const std::string NO_STRING;

    const char ASCII_DOWNCASE = 0x20;
    const char ASCII_DEL      = 0x7f;

    const char TELNET_WILL = 0xfb;
    const char TELNET_WONT = 0xfc;
    const char TELNET_DO   = 0xfd;
    const char TELNET_DONT = 0xfe;
    const char TELNET_IAC  = 0xff;
}

void Tokenizer::Str(const std::string &line)
{
    m_cache.clear();
    m_cacheIterator = m_cache.end();
    m_pos = line.begin();
    m_end = line.end();

    AdvanceToNextToken();
}

void Tokenizer::IgnoreTelnetCommand()
{
    ++m_pos;

    // Telnet "will", "won't", "do", "don't" have a 3rd argument byte
    if (*m_pos >= TELNET_WILL && *m_pos <= TELNET_DONT)
        ++m_pos;
}

void Tokenizer::AdvanceToNextToken()
{
    for (; m_pos != m_end; ++m_pos)
    {
        if (*m_pos == TELNET_IAC)
        {
            IgnoreTelnetCommand();
        }
        else if (*m_pos > ' ' && *m_pos < ASCII_DEL)
        {
            return;
        }
    }
}

Token Tokenizer::GetToken()
{
    if (m_cacheIterator != m_cache.end())
    {
        auto it = m_cacheIterator++;

        if (it->second)
            return it->second;
        else
            return it->second = m_dictionary.LookUp(it->first);
    }
    else if (m_pos == m_end)
    {
        return Dictionary::NoWord;
    }
    const auto &nextString = ReadNextString();
    return (m_cacheIterator-1)->second = m_dictionary.LookUp(nextString);
}

const std::string &Tokenizer::GetString()
{
    if (m_cacheIterator != m_cache.end())
    {
        return (m_cacheIterator++)->first;
    }
    else if (m_pos == m_end)
    {
        return NO_STRING;
    }
    return ReadNextString();
}

const std::string &Tokenizer::ReadNextString()
{
    auto &result = m_cache.emplace_back().first;
    m_cacheIterator = m_cache.end();

    for (; m_pos != m_end; ++m_pos)
    {
        if (*m_pos == TELNET_IAC)
        {
            IgnoreTelnetCommand();
        }
        else if (*m_pos <= ' ' || *m_pos >= ASCII_DEL)
        {
            break;
        }
        else if (*m_pos >= 'A' && *m_pos <= 'Z')
        {
            result.push_back(*m_pos + ASCII_DOWNCASE);
        }
        else
        {
            result.push_back(*m_pos);
        }
    }

    AdvanceToNextToken();
    return result;
}

std::string Tokenizer::DumpRestOfLine()
{
    std::string result;
    for (; m_pos != m_end; ++m_pos)
    {
        if (*m_pos == TELNET_IAC)
        {
            IgnoreTelnetCommand();
        }
        else if (*m_pos == '\r')
        {
            // do nothing
        }
        else if (*m_pos <= ' ' || *m_pos >= ASCII_DEL)
        {
            result.push_back(' ');
        }
        else
        {
            result.push_back(*m_pos);
        }
    }
    return result;
}
