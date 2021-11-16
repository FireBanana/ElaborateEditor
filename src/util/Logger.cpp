#include "Logger.h"

void LOG_UNFORMATTED(std::ostream& out, std::string const& s)
{
    for (auto& ch : s)
    {
        switch (ch)
        {
        case '\'':
            out << "\\'";
            break;

        case '\"':
            out << "\\\"";
            break;

        case '\?':
            out << "\\?";
            break;

        case '\\':
            out << "\\\\";
            break;

        case '\a':
            out << "\\a";
            break;

        case '\b':
            out << "\\b";
            break;

        case '\f':
            out << "\\f";
            break;

        case '\n':
            out << "\\n";
            break;

        case '\r':
            out << "\\r";
            break;

        case '\t':
            out << "\\t";
            break;

        case '\v':
            out << "\\v";
            break;

        case '\0':
            out << "\\0";
            break;

        default:
            out << ch;
        }
    }
}