/*
  Copyright (c) 2004-2022 The FlameRobin Development Team

  Permission is hereby granted, free of charge, to any person obtaining
  a copy of this software and associated documentation files (the
  "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish,
  distribute, sublicense, and/or sell copies of the Software, and to
  permit persons to whom the Software is furnished to do so, subject to
  the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <fstream>
#include <sstream>

#include "core/FRError.h"
#include "core/StringUtils.h"

std::string wx2std(const wxString& input, wxMBConv* conv)
{
    if (input.empty())
        return "";
    if (!conv)
        conv = wxConvCurrent;
    const wxWX2MBbuf buf(input.mb_str(*conv));
    // conversion may fail and return 0, which isn't a safe value to pass
    // to std:string constructor
    if (!buf)
        return "";
    return std::string(buf);
}

wxString std2wxIdentifier(const std::string& input, wxMBConv* conv)
{
    if (input.empty())
        return wxEmptyString;
    if (!conv)
        conv = wxConvCurrent;
    // trim trailing whitespace
    size_t last = input.find_last_not_of(" ");
    return wxString(input.c_str(), *conv,
        (last == std::string::npos) ? std::string::npos : last + 1);
}

wxString getHtmlCharset()
{
    return "UTF-8";
}

wxString escapeHtmlChars(const wxString& input, bool processNewlines)
{
    if (input.empty())
        return input;
    wxString result;
    wxString::const_iterator start = input.begin();
    while (start != input.end())
    {
        wxString::const_iterator stop = start;
        while (stop != input.end())
        {
            const wxChar c = *stop;
            if (c == '&' || c == '<' || c == '>' || c == '"'
                || (processNewlines && (c == '\r' || c == '\n')))
            {
                if (stop > start)
                    result += wxString(start, stop);
                if (c == '&')
                    result += "&amp;";
                else if (c == '<')
                    result += "&lt;";
                else if (c == '>')
                    result += "&gt;";
                else if (c == '"')
                    result += "&quot;";
                else if (c == '\n')
                    result += "<BR>";
                else if (c == '\r')
                    /* swallow silently */;
                else
                    wxASSERT_MSG(false, "escape not handled");
                // start processing *after* the replaced character
                ++stop;
                start = stop;
                break;
            }
            ++stop;
        }
        if (stop > start)
            result += wxString(start, stop);
        start = stop;
    }
    return result;
}

wxString escapeXmlChars(const wxString& input)
{
    if (input.empty())
        return input;
    wxString result;
    wxString::const_iterator start = input.begin();
    while (start != input.end())
    {
        wxString::const_iterator stop = start;
        while (stop != input.end())
        {
            const wxChar c = *stop;
            if (c == '&' || c == '<' || c == '>' || c == '"')
            {
                if (stop > start)
                    result += wxString(start, stop);
                if (c == '&')
                    result += "&amp;";
                else if (c == '<')
                    result += "&lt;";
                else if (c == '>')
                    result += "&gt;";
                else if (c == '"')
                    result += "&quot;";
                else
                    wxASSERT_MSG(false, "escape not handled");
                // start processing *after* the replaced character
                ++stop;
                start = stop;
                break;
            }
            ++stop;
        }
        if (stop > start)
            result += wxString(start, stop);
        start = stop;
    }
    return result;
}

wxString wxArrayToString(const wxArrayString& arrayStr, const wxString& delimiter)
{
    wxString result;
    for (wxArrayString::const_iterator it = arrayStr.begin();
        it != arrayStr.end(); ++it)
    {
        if (result.IsEmpty())
            result << *(it);
        else
            result << delimiter << *(it);
    }
    return result;
}

wxString loadEntireFile(const wxFileName& filename)
{
    if (!filename.FileExists())
    {
        wxString msg;
        msg.Printf(_("The file \"%s\" does not exist."),
            filename.GetFullPath().c_str());
        throw FRError(msg);
    }

    // read entire file into wxString buffer
    std::ifstream filex(wx2std(filename.GetFullPath()).c_str());
    if (!filex)
    {
        wxString msg;
        msg.Printf(_("The file \"%s\" cannot be opened."),
            filename.GetFullPath().c_str());
        throw FRError(msg);
    }

    std::stringstream ss;
    ss << filex.rdbuf();
    wxString s(ss.str());
    filex.close();
    return s;
}

wxString wrapText(const wxString& text, size_t maxWidth, size_t indent)
{
    if (text.Length() <= maxWidth)
        return text;

    enum { none, insideSingle, insideDouble, } wrapState = none;

    wxString indentStr;
    bool eol(false);
    wxString wrappedText;
    wxString line;

    wxString::const_iterator lastSpace = text.end();
    wxString::const_iterator lineStart = text.begin();
    for (wxString::const_iterator it = lineStart; ; ++it)
    {
        if (it == text.end())
        {
            wrappedText << indentStr << line;
            break;
        }

        if (eol)
        {
            wrappedText += '\n';
            if (indentStr.IsEmpty())
                indentStr.Pad(indent);

            lastSpace = text.end();
            line.clear();
            lineStart = it;
            eol = false;
        }

        if (*it == '\n')
        {
            wrappedText << indentStr << line;
            eol = true;
        }
        else
        {
            if (wrapState == insideSingle)
            {
                if (*it == '\'')
                    wrapState = none;
            }
            else if (wrapState == insideDouble)
            {
                if (*it == '"')
                    wrapState = none;
            }
            else
            {
                if (*it == ' ')
                    lastSpace = it;
                else if (*it == '\'')
                    wrapState = insideSingle;
                else if (*it == '"')
                    wrapState = insideDouble;
            }

            line += *it;

            if (lastSpace != text.end())
            {
                size_t width = line.Length();
                if ((width > maxWidth - indentStr.Length()) && (wrapState == none))
                {
                    // remove the last word from this line
                    line.erase(lastSpace - lineStart, it + 1 - lineStart);
                    wrappedText << indentStr << line;
                    eol = true;

                    // go back to the last word of this line which we didn't
                    // output yet
                    it = lastSpace;
                }
            }
            // else: no wrapping at all or impossible to wrap
        }
    }
    return wrappedText;
}

namespace
{
    inline bool isIdentStart(wxUniChar c)
    {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
    }
    inline bool isIdentCont(wxUniChar c)
    {
        return isIdentStart(c) || (c >= '0' && c <= '9');
    }
}

wxString expandMustachePlaceholders(const wxString& sql)
{
    // Cheap fast-path: nothing to do if the SQL has no '{{' at all.
    if (sql.Find(wxT("{{")) == wxNOT_FOUND)
        return sql;

    wxString out;
    out.reserve(sql.length());

    const size_t n = sql.length();
    for (size_t i = 0; i < n; )
    {
        wxUniChar c = sql[i];

        // Single-line comment: -- ... \n
        if (c == '-' && i + 1 < n && sql[i + 1] == '-')
        {
            while (i < n && sql[i] != '\n')
                out += sql[i++];
            continue;
        }

        // Block comment: /* ... */
        if (c == '/' && i + 1 < n && sql[i + 1] == '*')
        {
            out += sql[i++]; out += sql[i++];
            while (i < n)
            {
                if (sql[i] == '*' && i + 1 < n && sql[i + 1] == '/')
                {
                    out += sql[i++]; out += sql[i++];
                    break;
                }
                out += sql[i++];
            }
            continue;
        }

        // String literals — copy through verbatim. Doubled quotes inside
        // the literal (Firebird's escape) are handled by treating the
        // pair as two ordinary characters.
        if (c == '\'' || c == '"')
        {
            wxUniChar quote = c;
            out += sql[i++];
            while (i < n)
            {
                if (sql[i] == quote)
                {
                    out += sql[i++];
                    if (i < n && sql[i] == quote) // doubled-quote escape
                    {
                        out += sql[i++];
                        continue;
                    }
                    break;
                }
                out += sql[i++];
            }
            continue;
        }

        // Mustache placeholder: {{Identifier}}
        if (c == '{' && i + 2 < n && sql[i + 1] == '{'
            && isIdentStart(sql[i + 2]))
        {
            size_t j = i + 2;
            while (j < n && isIdentCont(sql[j]))
                ++j;
            if (j + 1 < n && sql[j] == '}' && sql[j + 1] == '}')
            {
                out += ':';
                out += sql.SubString(i + 2, j - 1);
                i = j + 2;
                continue;
            }
            // Not a well-formed placeholder — fall through and copy
            // characters as-is so we don't change behaviour for code
            // that legitimately contains "{{" (rare, but possible
            // inside, say, a custom UDF that takes JSON).
        }

        out += sql[i++];
    }
    return out;
}
