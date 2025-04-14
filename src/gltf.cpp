/* <editor-fold desc="MIT License">

Copyright(c) 2025 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

//#include <vsg/io/json.h>

#include "JSONParser.h"
#include "gltf.h"

#include <vsg/io/Path.h>
#include <vsg/io/mem_stream.h>

#include <fstream>

using namespace vsg2;
using namespace vsgXchange;

template<typename T>
struct container_schema : public vsg2::ArraySchema
{
    std::vector<T> values;
    void read_number(JSONParser& parser, std::istream& input) override
    {
        T value;
        input >> value;
        values.push_back(value);
    }
};

struct accessor_schema : public vsg2::ObjectSchema
{
    uint32_t bufferView = 0;
    uint32_t byteOffset = 0;
    uint32_t componentType = 0;
    bool normalized = false;
    uint32_t count = 0;
    std::string type;
    container_schema<double> max;
    container_schema<double> min;

    // sparse
    // name
    // extensions
    // extras

    void report()
    {
        vsg::info("accessor_schema { ");
        vsg::info("    bufferView: ", bufferView);
        vsg::info("    byteOffset: ", byteOffset);
        vsg::info("    componentType: ", componentType);
        vsg::info("    normalized: ", normalized);
        vsg::info("    count: ", count);
        vsg::info("    type: ", type);
        for(auto& value : max.values) { vsg::info("    max : ", value); }
        for(auto& value : min.values) { vsg::info("    min : ", value); }
        vsg::info("} ");
    }

    void read_array(JSONParser& parser, const std::string_view& name) override
    {
        if (name == "max")
        {
            parser.read_array(max);
        }
        else if (name == "min")
        {
            parser.read_array(max);
        }
        else vsg::warn("A gltf parsing error, position = ", parser.pos);
    }

    void read_object(JSONParser& parser, const std::string_view& name) override
    {
        if (name == "sparse")
        {
        }
        else if (name == "name")
        {
        }
        else if (name == "extensions")
        {
        }
        else if (name == "extras")
        {
        }
        else vsg::warn("B gltf parsing error, position = ", parser.pos);
    }

    void read_string(JSONParser& parser, const std::string_view& name) override
    {
        if (name=="type" && parser.read_string(type)) {}
        else vsg::warn("C gltf parsing error, position = ", parser.pos);
    }

    void read_number(JSONParser& parser, const std::string_view& name, std::istream& input) override
    {
        if (name=="bufferView") input >> bufferView;
        else if (name=="byteOffset") input >> byteOffset;
        else if (name=="componentType") input >> componentType;
        else if (name=="count") input >> count;
        else vsg::warn("D gltf parsing error, position = ", parser.pos, ", name = ", name);
    }

    void read_bool(JSONParser& parser, const std::string_view& name, bool value) override
    {
        if (name=="normalized") normalized = value;
        else vsg::warn("E gltf parsing error, position = ", parser.pos);
    }

};

struct accessors_schema : public vsg2::ArraySchema
{
    std::vector<accessor_schema> accessors;

    void read_object(JSONParser& parser) override
    {
        vsg::info("accessors_schema::read_object()", this);

        accessors.emplace_back();
        parser.read_object(accessors.back());

        vsg::info("done accessors_schema::read_object()", &accessors.back());
        accessors.back().report();
    }
};


struct glTF_schema : public vsg2::ObjectSchema
{
    accessors_schema accessors;

    void read_array(JSONParser& parser, const std::string_view& name) override;
    void read_object(JSONParser& parser, const std::string_view& name) override;
    void read_string(JSONParser& parser, const std::string_view& name) override;
    void read_number(JSONParser& parser, const std::string_view& name, std::istream& input) override;
    void read_bool(JSONParser& parser, const std::string_view& name, bool value) override;
    void read_null(JSONParser& parser, const std::string_view& name) override;
};

void glTF_schema::read_array(JSONParser& parser, const std::string_view& name)
{
    if (name == "extensionsUsed")
    {
        vsg::info("extensionsUsed schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "extensionsRequired")
    {
        vsg::info("extensionsRequired schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "accessors")
    {
        vsg::info("accessors schema required (",name,") ");
        parser.read_array(accessors);
    }
    else if (name == "animations")
    {
        vsg::info("animations schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "buffers")
    {
        vsg::info("buffers schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "bufferViews")
    {
        vsg::info("bufferViews schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "cameras")
    {
        vsg::info("cameras schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "")
    {
        vsg::info(" schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "materials")
    {
        vsg::info("materials schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "meshes")
    {
        vsg::info("meshes schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "nodes")
    {
        vsg::info("nodes schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "samplers")
    {
        vsg::info("samplers schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "scenes")
    {
        vsg::info("scenes schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "skins")
    {
        vsg::info("skins schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "textures")
    {
        vsg::info("textures schema required (",name,") ");
        // parser.read_array(*this);
    }
    else
    {
        vsg::info("read_array(",name,") ");
        //parser.read_array(*this);
    }
}

void glTF_schema::read_object(JSONParser& parser, const std::string_view& name)
{
    if (name == "asset")
    {
        vsg::info("need to implement asset schema (",name,") ");
        parser.read_object(*this);

    }
    else if (name == "scene")
    {
        vsg::info("need to implement scene schema (",name,") ");
        parser.read_object(*this);
    }
    else if (name == "extensions")
    {
        vsg::info("need to implement extensions schema (",name,") ");
        parser.read_object(*this);

    }
    else if (name == "extras")
    {
        vsg::info("need to implement extras schema (",name,") ");
        parser.read_object(*this);
    }
    else
    {
        vsg::info("read_object(",name,") ");
        parser.read_object(*this);
    }
}

void glTF_schema::read_string(JSONParser& parser, const std::string_view& name)
{
    std::string str;
    if (parser.read_string(str))
    {
        vsg::info("read_string(",name,") ", str);
    }
}

void glTF_schema::read_number(JSONParser& parser, const std::string_view& name, std::istream& input)
{
    double number;
    input >> number;

    vsg::info("read_number(",name,") ", number);
}

void glTF_schema::read_bool(JSONParser& parser, const std::string_view& name, bool value)
{
    vsg::info("read_bool(",name,") ", value);
}

void glTF_schema::read_null(JSONParser& parser, const std::string_view& name)
{
    vsg::info("read_null(",name,")");
}


gltf::gltf()
{
}

bool gltf::supportedExtension(const vsg::Path& ext) const
{
    return ext == ".gltf" || ext == ".glb";
}

vsg::ref_ptr<vsg::Object> gltf::_read(std::istream& fin, vsg::ref_ptr<const vsg::Options> options) const
{
    fin.seekg(0, fin.end);
    size_t fileSize = fin.tellg();

    if (fileSize==0) return {};

    vsg2::JSONParser parser;

    glTF_schema schema;

    parser.buffer.resize(fileSize);

    fin.seekg(0);
    fin.read(reinterpret_cast<char*>(parser.buffer.data()), fileSize);

    vsg::ref_ptr<vsg::Object> result;

    // skip white space
    parser.pos = parser.buffer.find_first_not_of(" \t\r\n", 0);
    if (parser.pos == std::string::npos) return {};

    if (parser.buffer[parser.pos]=='{')
    {
        parser.read_object(schema);
    }
    else
    {
        vsg::info("Parsing error, could not find opening {");
    }

    return result;
}


vsg::ref_ptr<vsg::Object> gltf::read(const vsg::Path& filename, vsg::ref_ptr<const vsg::Options> options) const
{
    vsg::Path ext  = (options && options->extensionHint) ? options->extensionHint : vsg::lowerCaseFileExtension(filename);
    if (!supportedExtension(ext)) return {};

    vsg::Path filenameToUse = vsg::findFile(filename, options);
    if (!filenameToUse) return {};

    vsg::ref_ptr<vsg::stringValue> contents = vsg::stringValue::create();
    auto& buffer = contents->value();

    std::ifstream fin(filenameToUse, std::ios::ate | std::ios::binary);
    return _read(fin, options);
}

vsg::ref_ptr<vsg::Object> gltf::read(std::istream& fin, vsg::ref_ptr<const vsg::Options> options) const
{
    if (!options || !options->extensionHint) return {};
    if (!supportedExtension(options->extensionHint)) return {};

    return _read(fin, options);
}

vsg::ref_ptr<vsg::Object> gltf::read(const uint8_t* ptr, size_t size, vsg::ref_ptr<const vsg::Options> options) const
{
    if (!options || !options->extensionHint) return {};
    if (!supportedExtension(options->extensionHint)) return {};

    vsg::mem_stream fin(ptr, size);
    return _read(fin, options);
}


bool gltf::getFeatures(Features& features) const
{
    vsg::ReaderWriter::FeatureMask supported_features = static_cast<vsg::ReaderWriter::FeatureMask>(vsg::ReaderWriter::READ_FILENAME | vsg::ReaderWriter::READ_ISTREAM | vsg::ReaderWriter::READ_MEMORY);
    features.extensionFeatureMap[".gltf"] = supported_features;
    features.extensionFeatureMap[".glb"] = supported_features;

    return true;
}
