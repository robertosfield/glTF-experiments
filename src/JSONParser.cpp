/* <editor-fold desc="MIT License">

Copyright(c) 2025 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/core/Objects.h>
#include <vsg/core/Value.h>
#include <vsg/io/Path.h>
//#include <vsg/io/json.h>
#include <vsg/io/mem_stream.h>

#include "JSONParser.h"

#include <fstream>

using namespace vsg2;

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// JSONParser::Schema
//
void JSONParser::Schema::read_array(JSONParser& parser)
{
}

void JSONParser::Schema::read_object(JSONParser& parser)
{
}

void JSONParser::Schema::read_string(JSONParser& parser)
{
}

void JSONParser::Schema::read_number(JSONParser& parser, std::istream& input)
{
}

void JSONParser::Schema::read_bool(JSONParser& parser, bool value)
{
}

void JSONParser::Schema::read_null(JSONParser& parser)
{
}

void JSONParser::Schema::read_array(JSONParser& parser, const std::string_view& name)
{
}

void JSONParser::Schema::read_object(JSONParser& parser, const std::string_view& name)
{
}

void JSONParser::Schema::read_string(JSONParser& parser, const std::string_view& name)
{
}

void JSONParser::Schema::read_number(JSONParser& parser, const std::string_view& name, std::istream& input)
{
}

void JSONParser::Schema::read_bool(JSONParser& parser, const std::string_view& name, bool value)
{
}

void JSONParser::Schema::read_null(JSONParser& parser, const std::string_view& name)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// json parser
//
JSONParser::JSONParser() :
    mstr(nullptr, 0)
{
}

bool JSONParser::read_string(std::string& value)
{
    if (buffer[pos] != '"') return false;

    // read string
    auto end_of_value = buffer.find('"', pos + 1);
    if (end_of_value == std::string::npos) return false;

    // could have escape characters.

    value = buffer.substr(pos + 1, end_of_value - pos - 1);

    pos = end_of_value + 1;

    return true;
}


void JSONParser::read_object(JSONParser::Schema& schema)
{
    if (pos == std::string::npos) return;
    if (buffer[pos] != '{') return;

    // buffer[pos] == '{'
    // advance past open bracket
    pos = buffer.find_first_not_of(" \t\r\n", pos + 1);
    if (pos == std::string::npos) return;

    indent += 4;

    while (pos != std::string::npos && pos < buffer.size() && buffer[pos] != '}')
    {
        if (buffer[pos] == '"')
        {
            auto end_of_string = buffer.find('"', pos + 1);
            if (end_of_string == std::string::npos) break;

            std::string_view name(&buffer[pos + 1], end_of_string - pos - 1);

            // skip white space
            pos = buffer.find_first_not_of(" \t\r\n", end_of_string + 1);
            if (pos == std::string::npos)
            {
                vsg::info(indent, "read_object()  deliminator error end of buffer.");
                break;
            }

            // make sure next charater is the {name : value} deliminator
            if (buffer[pos] != ':')
            {
                vsg::info(indent, "read_object()  deliminator error buffer[", pos, "] = ", buffer[pos]);
                break;
            }

            // skip white space
            pos = buffer.find_first_not_of(" \t\r\n", pos + 1);
            if (pos == std::string::npos)
            {
                break;
            }

            // now look to pair with value after " : "
            if (buffer[pos] == '{')
            {
                schema.read_object(*this, name);
            }
            else if (buffer[pos] == '[')
            {
                schema.read_array(*this, name);
            }
            else if (buffer[pos] == '"')
            {
                schema.read_string(*this, name);
            }
            else
            {
                auto end_of_field = buffer.find_first_of(",}]", pos + 1);
                if (end_of_field == std::string::npos) break;

                auto end_of_value = end_of_field - 1;
                while (end_of_value > 0 && white_space(buffer[end_of_value])) --end_of_value;

                if (buffer.compare(pos, end_of_value - pos, "null") == 0)
                {
                    schema.read_null(*this, name);
                }
                else if (buffer.compare(pos, end_of_value - pos, "true") == 0)
                {
                    schema.read_bool(*this, name, true);
                }
                else if (buffer.compare(pos, end_of_value - pos, "false") == 0)
                {
                    schema.read_bool(*this, name, false);
                }
                else
                {
                    mstr.set(reinterpret_cast<const uint8_t*>(&buffer.at(pos)), end_of_value - pos + 1);
                    schema.read_number(*this, name, mstr);
                }

                // skip to end of field
                pos = end_of_field;
            }
        }
        else if (buffer[pos] == ',')
        {
            ++pos;
        }
        else
        {
            vsg::info(indent, "read_object() buffer[", pos, "] = ", buffer[pos]);
        }

        pos = buffer.find_first_not_of(" \t\r\n", pos);
    }

    if (pos < buffer.size() && buffer[pos] == '}')
    {
        ++pos;
    }

    indent -= 4;
}

void JSONParser::read_array(JSONParser::Schema& schema)
{
    pos = buffer.find_first_not_of(" \t\r\n", pos);
    if (pos == std::string::npos) return;
    if (buffer[pos] != '[')
    {
        vsg::info(indent, "read_array() could not match opening [");
        return;
    }

    // buffer[pos] == '['
    // advance past open bracket
    pos = buffer.find_first_not_of(" \t\r\n", pos + 1);
    if (pos == std::string::npos)
    {
        vsg::info(indent, "read_array() contents after [");
        return;
    }

    indent += 4;

    while (pos != std::string::npos && pos < buffer.size() && buffer[pos] != ']')
    {
        // now look to pair with value after " : "
        if (buffer[pos] == '{')
        {
            schema.read_object(*this);
        }
        else if (buffer[pos] == '[')
        {
            schema.read_array(*this);
        }
        else if (buffer[pos] == '"')
        {
            if (std::string value; read_string(value))
            {
                schema.read_string(*this);
            }
        }
        else if (buffer[pos] == ',')
        {
            ++pos;
        }
        else
        {
            auto end_of_field = buffer.find_first_of(",}]", pos + 1);
            if (end_of_field == std::string::npos) break;

            auto end_of_value = end_of_field - 1;
            while (end_of_value > 0 && white_space(buffer[end_of_value])) --end_of_value;

            if (buffer.compare(pos, end_of_value - pos, "null") == 0)
            {
                schema.read_null(*this);
            }
            else if (buffer.compare(pos, end_of_value - pos, "true") == 0)
            {
                schema.read_bool(*this, true);
            }
            else if (buffer.compare(pos, end_of_value - pos, "false") == 0)
            {
                schema.read_bool(*this, false);
            }
            else
            {
                mstr.set(reinterpret_cast<const uint8_t*>(&buffer.at(pos)), end_of_value - pos + 1);

                schema.read_number(*this, mstr);
            }

            // skip to end of field
            pos = end_of_field;
        }

        pos = buffer.find_first_not_of(" \t\r\n", pos);
    }

    if (pos < buffer.size() && buffer[pos] == ']')
    {
        ++pos;
    }

    indent -= 4;
}
