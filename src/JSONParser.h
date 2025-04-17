#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2025 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shimages be included in images
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/io/ReaderWriter.h>
#include <vsg/io/mem_stream.h>

#include <iostream>

namespace vsg2
{

    /// JSON parser based on spec: https://www.json.org/json-en.html
    struct JSONParser
    {
        std::string buffer;
        std::size_t pos = 0;
        vsg::mem_stream mstr;
        vsg::indentation indent;

        JSONParser();

        inline bool white_space(char c) const
        {
            return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
        }

        struct Schema
        {
            // array elements [ value, value.. ]
            virtual void read_array(JSONParser& parser);
            virtual void read_object(JSONParser& parser);
            virtual void read_string(JSONParser& parser);
            virtual void read_number(JSONParser& parser, std::istream& input);
            virtual void read_bool(JSONParser& parser, bool value);
            virtual void read_null(JSONParser& parser);

            // object properties { name, value; ... }
            virtual void read_array(JSONParser& parser, const std::string_view& name);
            virtual void read_object(JSONParser& parser, const std::string_view& name);
            virtual void read_string(JSONParser& parser, const std::string_view& name);
            virtual void read_number(JSONParser& parser, const std::string_view& name, std::istream& input);
            virtual void read_bool(JSONParser& parser, const std::string_view& name, bool value);
            virtual void read_null(JSONParser& parser, const std::string_view& name);
        };

        bool read_string(std::string& value);
        void read_object(Schema& schema);
        void read_array(Schema& schema);

        template<typename... Args>
        void warning(Args&&... args)
        {
            std::cout<<std::endl;
            vsg::warn("Parsing error at pos = ", pos, ". ", std::forward<Args>(args)...);
        }
    };

} // namespace vsg

VSG_type_name(vsg2::JSONParser);
