/**
 * @file str_manip.h
 * @author Pascal Enderli
 * @date 2020.01.19
 * @brief Collection of string manipulation functions.
 */

#include<algorithm>
#include <stdio.h>
#include<string>
#include <time.h>

/// Trims white spaces to the left of a string.
static void LTrim(std::string& s) noexcept
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
    [](int ch){ return !std::isspace(ch);}
    ));
}

/// Trims white spaces to the right of a string.
static void RTrim(std::string& s) noexcept
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
    [](int ch) { return !std::isspace(ch);}
    ).base(), s.end());
}

/// Trims white spaces on both ends of a string.
static void TrimWhitespace(std::string& s) noexcept
{
    LTrim(s);
    RTrim(s);
}

/// Cuts away the comments on the right end of a line delimited with # or ;
static void TrimComment(std::string& s) noexcept
{
    size_t pos = s.find_first_of(";#");
    if(pos != std::string::npos)
    {
        s.erase(pos,  std::string::npos);
    }
}

/// Checks if a string contains white spaces.
static bool ContainsWhiteSpace(const std::string& str) noexcept
{
    return str.find(" ") != std::string::npos;
}
