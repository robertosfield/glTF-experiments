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
struct container_schema : public vsg2::JSONParser::Schema
{
    std::vector<T> values;
    void read_number(JSONParser& parser, std::istream& input) override
    {
        T value;
        input >> value;
        values.push_back(value);
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// accessor_schema
//
struct accessor_schema : public vsg2::JSONParser::Schema
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
        if (name == "max") parser.read_array(max);
        else if (name == "min") parser.read_array(max);
        else parser.warning();
    }

    void read_string(JSONParser& parser, const std::string_view& name) override
    {
        if (name=="type" && parser.read_string(type)) {}
        else parser.warning();
    }

    void read_number(JSONParser& parser, const std::string_view& name, std::istream& input) override
    {
        if (name=="bufferView") input >> bufferView;
        else if (name=="byteOffset") input >> byteOffset;
        else if (name=="componentType") input >> componentType;
        else if (name=="count") input >> count;
        else parser.warning();
    }

    void read_bool(JSONParser& parser, const std::string_view& name, bool value) override
    {
        if (name=="normalized") normalized = value;
        else parser.warning();
    }

};

struct accessors_schema : public vsg2::JSONParser::Schema
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// asset_schema
//
struct asset_scheme : public vsg2::JSONParser::Schema
{
    std::string copyright;
    std::string version;
    std::string generator;
    std::string minVersion;

    void report()
    {
        vsg::info("asset_scheme = { copyright = ", copyright, ", generator = ", generator, ", version = ", version, ", minVersion = ", minVersion, " } ", this);
    }

    void read_string(JSONParser& parser, const std::string_view& name) override
    {
        if (name=="copyright") { parser.read_string(copyright); }
        else if (name=="generator") { parser.read_string(generator); }
        else if (name=="version") { parser.read_string(version); }
        else if (name=="minVersion") { parser.read_string(minVersion); }
        else parser.warning();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// bufferViews_schema
//
struct bufferView_schema : public vsg2::JSONParser::Schema
{
    uint32_t buffer = 0;
    uint32_t byteOffset = 0;
    uint32_t byteLength = 0;
    uint32_t byteStride = 4;
    uint32_t target = 0;

    // name
    // extensions
    // extras

    void report()
    {
        vsg::info("bufferView_schema { ");
        vsg::info("    buffer: ", buffer);
        vsg::info("    byteOffset: ", byteOffset);
        vsg::info("    byteLength: ", byteLength);
        vsg::info("    byteStride: ", byteStride);
        vsg::info("    target: ", target);
        vsg::info("} ");
    }

    void read_number(JSONParser& parser, const std::string_view& name, std::istream& input) override
    {
        if (name=="buffer") input >> buffer;
        else if (name=="byteOffset") input >> byteOffset;
        else if (name=="byteLength") input >> byteLength;
        else if (name=="byteStride") input >> byteStride;
        else if (name=="target") input >> target;
        else parser.warning();
    }
};

struct bufferViews_schema : public vsg2::JSONParser::Schema
{
    std::vector<bufferView_schema> bufferViews;

    void read_object(JSONParser& parser) override
    {
        vsg::info("accessors_schema::read_object()", this);

        bufferViews.emplace_back();
        parser.read_object(bufferViews.back());

        vsg::info("done accessors_schema::read_object()", &bufferViews.back());
        bufferViews.back().report();
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// buffers_schema
//
struct buffer_schema : public vsg2::JSONParser::Schema
{
    std::string uri;
    uint32_t byteLength = 0;

    // name
    // extensions
    // extras

    void report()
    {
        vsg::info("buffer_schema { ");
        vsg::info("    uri: ", uri);
        vsg::info("    byteLength: ", byteLength);
        vsg::info("} ");
    }


    void read_string(JSONParser& parser, const std::string_view& name) override
    {
        if (name=="uri" && parser.read_string(uri)) {}
        else parser.warning();
    }

    void read_number(JSONParser& parser, const std::string_view& name, std::istream& input) override
    {
        if (name=="byteLength") input >> byteLength;
        else parser.warning();
    }
};

struct buffers_schema : public vsg2::JSONParser::Schema
{
    std::vector<buffer_schema> buffers;

    void read_object(JSONParser& parser) override
    {
        vsg::info("buffers_schema::read_object()", this);

        buffers.emplace_back();
        parser.read_object(buffers.back());

        vsg::info("done buffers_schema::read_object()", &buffers.back());
        buffers.back().report();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// images_schema
//
struct image_schema : public vsg2::JSONParser::Schema
{
    std::string uri;
    std::string mimeType;
    uint32_t bufferView = 0;

    // name
    // extensions
    // extras

    void report()
    {
        vsg::info("image_schema { ");
        vsg::info("    uri: ", uri);
        vsg::info("    mimeType: ", mimeType);
        vsg::info("    bufferView: ", bufferView);
        vsg::info("} ");
    }

    void read_string(JSONParser& parser, const std::string_view& name) override
    {
        if (name=="uri" && parser.read_string(uri)) {}
        else if (name=="mimeType" && parser.read_string(mimeType)) {}
        else parser.warning();
    }

    void read_number(JSONParser& parser, const std::string_view& name, std::istream& input) override
    {
        if (name=="bufferView") input >> bufferView;
        else parser.warning();
    }
};

struct images_schema : public vsg2::JSONParser::Schema
{
    std::vector<image_schema> images;

    void read_object(JSONParser& parser) override
    {
        vsg::info("images_schema::read_object()", this);

        images.emplace_back();
        parser.read_object(images.back());

        vsg::info("done images_schema::read_object()", &images.back());
        images.back().report();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// materials_schema
//

struct textureInfo_schema : public vsg2::JSONParser::Schema
{
    uint32_t index = 0;
    uint32_t texCoord = 0;

    void read_number(JSONParser& parser, const std::string_view& property, std::istream& input) override
    {
        if (property=="index") input >> index;
        else if (property=="texCoord") input >> texCoord;
        else parser.warning();
    }
};

struct pbrMetallicRoughness_schema : public vsg2::JSONParser::Schema
{
    container_schema<double> baseColorFactor; // default { 1.0, 1.0, 1.0, 1.0 }
    textureInfo_schema baseColorTexture;
    double metallicFactor = 1.0;
    double roughnessFactor = 1.0;
    textureInfo_schema metallicRoughnessTexture;

    void read_array(JSONParser& parser, const std::string_view& property) override
    {
        if (property == "baseColorFactor") parser.read_array(baseColorFactor);
        else parser.warning();
    }

    void read_object(JSONParser& parser, const std::string_view& property) override
    {
        if (property == "baseColorTexture") parser.read_object(baseColorTexture);
        else if (property == "metallicRoughnessTexture") parser.read_object(metallicRoughnessTexture);
        else parser.warning();
    }

    void read_number(JSONParser& parser, const std::string_view& property, std::istream& input) override
    {
        if (property=="metallicFactor") input >> metallicFactor;
        else if (property=="roughnessFactor") input >> roughnessFactor;
        else parser.warning();
    }
};

struct normalTextureInfo_schema : public textureInfo_schema
{
    double scale = 1.0;

    void read_number(JSONParser& parser, const std::string_view& property, std::istream& input) override
    {
        read_number(parser, property, input);
        if (property == "scale") input >> scale;
    }
};

struct occlusionTextureInfo_schema : public textureInfo_schema
{
    double strength = 1.0;

    void read_number(JSONParser& parser, const std::string_view& property, std::istream& input) override
    {
        read_number(parser, property, input);
        if (property == "strength") input >> strength;
    }
};

struct material_schema : public vsg2::JSONParser::Schema
{
    std::string name;
    pbrMetallicRoughness_schema pbrMetallicRoughness;
    normalTextureInfo_schema normalTexture;
    occlusionTextureInfo_schema occlusionTexture;
    textureInfo_schema emissiveTexture;
    container_schema<double> emissiveFactor; // default { 0.0, 0.0, 0.0 }
    std::string alphaMode = "OPAQUE";
    double alphaCutoff = 0.5;
    bool doubleSided = false;

    // extensions
    // extras

    void report()
    {
        vsg::info("material_schema { ");
        vsg::info("    name: ", name);
        vsg::info("    pbrMetallicRoughness.baseColorFactor = ", pbrMetallicRoughness.baseColorFactor.values.size(), " }" );
        vsg::info("    pbrMetallicRoughness.baseColorTexture = { ", pbrMetallicRoughness.baseColorTexture.index, ", ", pbrMetallicRoughness.baseColorTexture.texCoord, " }" );
        vsg::info("    pbrMetallicRoughness.metallicFactor ", pbrMetallicRoughness.metallicFactor);
        vsg::info("    pbrMetallicRoughness.roughnessFactor ", pbrMetallicRoughness.roughnessFactor);
        vsg::info("    pbrMetallicRoughness.metallicRoughnessTexture = { ", pbrMetallicRoughness.metallicRoughnessTexture.index, ", ", pbrMetallicRoughness.metallicRoughnessTexture.texCoord, " }" );
        vsg::info("    normalTexture = { ", normalTexture.index, ", ", normalTexture.texCoord, " }");
        vsg::info("    occlusionTexture = { ", occlusionTexture.index, ", ", occlusionTexture.texCoord, " }");
        vsg::info("    emissiveTexture = { ", emissiveTexture.index, ", ", emissiveTexture.texCoord, " }");
        vsg::info("    emissiveFactor : ", emissiveFactor.values.size(), " {");
        for(auto value : emissiveFactor.values) vsg::info("     ", value);
        vsg::info("    }");
        vsg::info("    alphaMode : ", alphaMode);
        vsg::info("    alphaCutoff : ", alphaCutoff);
        vsg::info("    doubleSided : ", doubleSided);
        vsg::info("} ");
    }

    void read_array(JSONParser& parser, const std::string_view& property) override
    {
        if (name == "emissiveFactor") parser.read_array(emissiveFactor);
        else parser.warning();
    }

    void read_object(JSONParser& parser, const std::string_view& property) override
    {
        if (property == "pbrMetallicRoughness") parser.read_object(pbrMetallicRoughness);
        else if (property == "normalTexture") parser.read_object(normalTexture);
        else if (property == "occlusionTexture") parser.read_object(occlusionTexture);
        else if (property == "emissiveTexture") parser.read_object(emissiveTexture);
        else parser.warning();
    }

    void read_string(JSONParser& parser, const std::string_view& property) override
    {
        if (property=="name" && parser.read_string(name)) {}
        else if (property=="alphaMode" && parser.read_string(alphaMode)) {}
        else parser.warning();
    }

    void read_number(JSONParser& parser, const std::string_view& property, std::istream& input) override
    {
        if (property=="alphaCutoff") input >> alphaCutoff;
        else parser.warning();
    }
};

struct materials_schema : public vsg2::JSONParser::Schema
{
    std::vector<material_schema> materials;

    void read_object(JSONParser& parser) override
    {
        vsg::info("materials_schema::read_object()", this);

        materials.emplace_back();
        parser.read_object(materials.back());

        vsg::info("done materials_schema::read_object()", &materials.back());
        materials.back().report();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// meshes_schema
//
struct attributes_schema : public vsg2::JSONParser::Schema
{
    std::map<std::string, uint32_t> values;

    void read_number(JSONParser&, const std::string_view& property, std::istream& input) override
    {
        input >> values[std::string(property)];
    }
};

struct primitive_schema : public vsg2::JSONParser::Schema
{
    attributes_schema attributes;
    uint32_t indices = 0;
    uint32_t material = 0;
    uint32_t mode = 0;

    void read_object(JSONParser& parser, const std::string_view& property) override
    {
        if (property == "attributes") parser.read_object(attributes);
        else parser.warning();
    }

    void read_number(JSONParser& parser, const std::string_view& property, std::istream& input) override
    {
        if (property == "indices") input >> indices;
        else if (property == "material") input >> material;
        else if (property == "mode") input >> mode;
        else parser.warning();
    }

    void report()
    {
        vsg::info("primitive_schema { ");
        vsg::info("    attributes = {");
        for(auto& [semantic, value] : attributes.values) vsg::info("        ", semantic, ", ", value);
        vsg::info("    }");
        vsg::info("    indices = ", indices);
        vsg::info("    material = ", material);
        vsg::info("    mode = ", mode);
        vsg::info("} ");
    }
};

struct primitives_schema : public vsg2::JSONParser::Schema
{
    std::vector<primitive_schema> primitives;

    void read_object(JSONParser& parser) override
    {
        vsg::info("primitives_schema::read_object()", this);

        primitives.emplace_back();
        parser.read_object(primitives.back());

        vsg::info("done primitives_schema::read_object()", &primitives.back());
        primitives.back().report();
    }
};


struct mesh_schema : public vsg2::JSONParser::Schema
{
    std::string name;
    primitives_schema primitives;
    container_schema<double> weights;

    // extensions
    // extras

    void report()
    {
        vsg::info("mesh_schema { ");
        vsg::info("    name: ", name);
        vsg::info("    primitives: ", primitives.primitives.size());
        vsg::info("    weights: ", weights.values.size());
        vsg::info("} ");
    }

    void read_array(JSONParser& parser, const std::string_view& property) override
    {
        if (property == "primitives") parser.read_array(primitives);
        else if (property == "weights") parser.read_array(weights);
        else parser.warning();
    }

    void read_string(JSONParser& parser, const std::string_view& property) override
    {
        if (property=="name" && parser.read_string(name)) {}
        else parser.warning();
    }
};

struct meshes_schema : public vsg2::JSONParser::Schema
{
    std::vector<mesh_schema> meshes;

    void read_object(JSONParser& parser) override
    {
        vsg::info("meshes_schema::read_object()", this);

        meshes.emplace_back();
        parser.read_object(meshes.back());

        vsg::info("done meshes_schema::read_object()", &meshes.back());
        meshes.back().report();
    }
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// glTF_schema
//
struct glTF_schema : public vsg2::JSONParser::Schema
{
    accessors_schema accessors;
    asset_scheme asset;
    bufferViews_schema bufferViews;
    buffers_schema buffers;
    images_schema images;
    materials_schema materials;
    meshes_schema meshes;

    void read_array(JSONParser& parser, const std::string_view& name) override;
    void read_object(JSONParser& parser, const std::string_view& name) override;
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
        parser.read_array(accessors);
    }
    else if (name == "animations")
    {
        vsg::info("animations schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "buffers")
    {
        parser.read_array(buffers);
    }
    else if (name == "bufferViews")
    {
        parser.read_array(bufferViews);
    }
    else if (name == "cameras")
    {
        vsg::info("cameras schema required (",name,") ");
        // parser.read_array(*this);
    }
    else if (name == "materials")
    {
        parser.read_array(materials);
    }
    else if (name == "meshes")
    {
        parser.read_array(meshes);
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
    else if (name == "images")
    {
        parser.read_array(images);
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
        parser.read_object(asset);

        asset.report();
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// gltf
//
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
