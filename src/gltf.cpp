/* <editor-fold desc="MIT License">

Copyright(c) 2025 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

//#include <vsg/io/json.h>

#include "gltf.h"

#include <vsg/io/Path.h>
#include <vsg/io/mem_stream.h>
#include <vsg/io/write.h>

#include <fstream>

using namespace vsgXchange;



////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Acessor
//
void gltf::Acessor::report()
{
    vsg::info("Acessor { ");
    vsg::info("    name = ", name);
    vsg::info("    extras.object = ", extras.object, ", extras.objects = ", extras.objects);
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

void gltf::Acessor::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "max") parser.read_array(max);
    else if (property == "min") parser.read_array(max);
    else parser.warning();
}

void gltf::Acessor::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else if (property=="type") parser.read_string(type);
    else parser.warning();
}

void gltf::Acessor::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="bufferView") input >> bufferView;
    else if (property=="byteOffset") input >> byteOffset;
    else if (property=="componentType") input >> componentType;
    else if (property=="count") input >> count;
    else parser.warning();
}

void gltf::Acessor::read_bool(vsg::JSONParser& parser, const std::string_view& property, bool value)
{
    if (property=="normalized") normalized = value;
    else parser.warning();
}

void gltf::Acessor::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="extras") parser.read_object(extras);
    else parser.warning();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// asset_schema
//
void gltf::Asset::report()
{
    vsg::info("Asset = {");
    vsg::info("    extras.object = ", extras.object, ", extras.objects = ", extras.objects);
    vsg::info("    copyright = ", copyright, ", generator = ", generator, ", version = ", version, ", minVersion = ", minVersion, " } ", this);
    vsg::info(" } ", this);
}

void gltf::Asset::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="copyright") { parser.read_string(copyright); }
    else if (property=="generator") { parser.read_string(generator); }
    else if (property=="version") { parser.read_string(version); }
    else if (property=="minVersion") { parser.read_string(minVersion); }
    else parser.warning();
}

void gltf::Asset::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="extras") parser.read_object(extras);
    else parser.warning();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// BufferView
//
void gltf::BufferView::report()
{
    vsg::info("BufferView { ");
    vsg::info("    name = ", name);
    vsg::info("    extras.object = ", extras.object, ", extras.objects = ", extras.objects);
    vsg::info("    buffer: ", buffer);
    vsg::info("    byteOffset: ", byteOffset);
    vsg::info("    byteLength: ", byteLength);
    vsg::info("    byteStride: ", byteStride);
    vsg::info("    target: ", target);
    vsg::info("} ");
}

void gltf::BufferView::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else parser.warning();
}

void gltf::BufferView::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="buffer") input >> buffer;
    else if (property=="byteOffset") input >> byteOffset;
    else if (property=="byteLength") input >> byteLength;
    else if (property=="byteStride") input >> byteStride;
    else if (property=="target") input >> target;
    else parser.warning();
}

void gltf::BufferView::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="extras") parser.read_object(extras);
    else parser.warning();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Buffer
//
void gltf::Buffer::report()
{
    vsg::info("Buffer { ");
    vsg::info("    name = ", name);
    vsg::info("    extras.object = ", extras.object, ", extras.objects = ", extras.objects);
    vsg::info("    uri: ", uri);
    vsg::info("    byteLength: ", byteLength);
    vsg::info("} ");
}

void gltf::Buffer::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else if (property=="uri" && parser.read_string(uri)) {}
    else parser.warning();
}

void gltf::Buffer::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="byteLength") input >> byteLength;
    else parser.warning();
}

void gltf::Buffer::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="extras") parser.read_object(extras);
    else parser.warning();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Image
//
void gltf::Image::report()
{
    vsg::info("Image { ");
    vsg::info("    name = ", name);
    vsg::info("    extras.object = ", extras.object, ", extras.objects = ", extras.objects);
    vsg::info("    uri: ", uri);
    vsg::info("    mimeType: ", mimeType);
    vsg::info("    bufferView: ", bufferView);
    vsg::info("} ");
}

void gltf::Image::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else if (property=="uri" && parser.read_string(uri)) {}
    else if (property=="mimeType" && parser.read_string(mimeType)) {}
    else parser.warning();
}

void gltf::Image::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="bufferView") input >> bufferView;
    else parser.warning();
}

void gltf::Image::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="extras") parser.read_object(extras);
    else parser.warning();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Material
//
void gltf::TextureInfo::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="index") input >> index;
    else if (property=="texCoord") input >> texCoord;
    else parser.warning();
}

void gltf::PbrMetallicRoughness::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "baseColorFactor") parser.read_array(baseColorFactor);
    else parser.warning();
}

void gltf::PbrMetallicRoughness::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "baseColorTexture") parser.read_object(baseColorTexture);
    else if (property == "metallicRoughnessTexture") parser.read_object(metallicRoughnessTexture);
    else parser.warning();
}

void gltf::PbrMetallicRoughness::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="metallicFactor") input >> metallicFactor;
    else if (property=="roughnessFactor") input >> roughnessFactor;
    else parser.warning();
}

void gltf::NormalTextureInfo::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property == "scale") input >> scale;
    else TextureInfo::read_number(parser, property, input);
}

void gltf::OcclusionTextureInfo::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property == "strength") input >> strength;
    else TextureInfo::read_number(parser, property, input);
}

void gltf::KHR_materials_specular::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "specularColorFactor") parser.read_array(specularColorFactor);
    else parser.warning();
}

void gltf::KHR_materials_specular::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "specularTexture") parser.read_object(specularTexture);
    else if (property == "specularColorTexture") parser.read_object(specularColorTexture);
    else parser.warning();
}

void gltf::KHR_materials_specular::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property == "specularFactor") input >> specularFactor;
    else parser.warning();
}

void gltf::KHR_materials_ior::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="ior") input >> ior;
    else parser.warning();
}

void gltf::Material_extensions::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "KHR_materials_specular") parser.read_object(specular);
    else if (property == "KHR_materials_ior") parser.read_object(ior);
    else parser.warning();
}

void gltf::Material::report()
{
    vsg::info("Material { ");
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
    vsg::info("    extensions.specular.specularFactor : ", extensions.specular.specularFactor);
    vsg::info("    extensions.specular.specularTexture : ", extensions.specular.specularTexture.index);
    vsg::info("    extensions.specular.specularColorFactor.size() : ", extensions.specular.specularColorFactor.values.size());
    vsg::info("    extensions.specular.specularColorTexture : ", extensions.specular.specularTexture.index);
    vsg::info("    extensions.ior.ior : ", extensions.ior.ior);
    vsg::info("} ");
}

void gltf::Material::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "emissiveFactor") parser.read_array(emissiveFactor);
    else parser.warning();
}

void gltf::Material::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "pbrMetallicRoughness") parser.read_object(pbrMetallicRoughness);
    else if (property == "normalTexture") parser.read_object(normalTexture);
    else if (property == "occlusionTexture") parser.read_object(occlusionTexture);
    else if (property == "emissiveTexture") parser.read_object(emissiveTexture);
    else if (property == "extensions") parser.read_object(extensions);
    else parser.warning();
}

void gltf::Material::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else if (property=="alphaMode" && parser.read_string(alphaMode)) {}
    else parser.warning();
}

void gltf::Material::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="alphaCutoff") input >> alphaCutoff;
    else parser.warning();
}

void gltf::Material::read_bool(vsg::JSONParser& parser, const std::string_view& property, bool value)
{
    if (property=="doubleSided") doubleSided = value;
    else parser.warning();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Mesh
//
void gltf::Attributes::read_number(vsg::JSONParser&, const std::string_view& property, std::istream& input)
{
    input >> values[std::string(property)];
}

void gltf::Primitive::report()
{
    vsg::info("Primitive { ");
    vsg::info("    attributes = {");
    for(auto& [semantic, id] : attributes.values) vsg::info("        ", semantic, ", ", id);
    vsg::info("    }");
    vsg::info("    indices = ", indices);
    vsg::info("    material = ", material);
    vsg::info("    mode = ", mode);
    vsg::info("    targets = [");
    for(auto& value : targets.values)
    {
        vsg::info("        {");
        for(auto& [semantic, id] : value.values) vsg::info("            ", semantic, ", ", id);
        vsg::info("        }");
    }
    vsg::info("    ]");
    vsg::info("} ");
}

void gltf::Primitive::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property == "indices") input >> indices;
    else if (property == "material") input >> material;
    else if (property == "mode") input >> mode;
    else parser.warning();
}

void gltf::Primitive::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="targets") parser.read_array(targets);
    else parser.warning();
}

void gltf::Primitive::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="extras") parser.read_object(extras);
    else if (property == "attributes") parser.read_object(attributes);
    else parser.warning();
}

void gltf::Mesh::report()
{
    vsg::info("Mesh { ");
    vsg::info("    name = ", name);
    vsg::info("    extras.object = ", extras.object, ", extras.objects = ", extras.objects);
    vsg::info("    primitives: ", primitives.values.size());
    vsg::info("    weights: ", weights.values.size());
    vsg::info("} ");
}

void gltf::Mesh::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "primitives") parser.read_array(primitives);
    else if (property == "weights") parser.read_array(weights);
    else parser.warning();
}

void gltf::Mesh::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else parser.warning();
}

void gltf::Mesh::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="extras") parser.read_object(extras);
    else parser.warning();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Node
//
void gltf::Node::report()
{
    vsg::info("Node { ");
    vsg::info("    name: ", name);
    if (camera) vsg::info("    camera: ", camera);
    if (skin) vsg::info("    skin: ", skin);
    if (mesh) vsg::info("    mesh: ", mesh);
    vsg::info("    children: ", children.values.size());
    vsg::info("    matrix: ", matrix.values.size());
    vsg::info("    rotation: ", rotation.values.size());
    vsg::info("    scale: ", scale.values.size());
    vsg::info("    translation: ", translation.values.size());
    vsg::info("    weights: ", weights.values.size());
    vsg::info("} ");
}

void gltf::Node::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "children") parser.read_array(children);
    else if (property == "matrix") parser.read_array(matrix);
    else if (property == "rotation") parser.read_array(rotation);
    else if (property == "scale") parser.read_array(scale);
    else if (property == "translation") parser.read_array(translation);
    else if (property == "weights") parser.read_array(weights);
    else parser.warning();
}

void gltf::Node::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else parser.warning();
}

void gltf::Node::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="camera") input >> camera;
    else if (property=="skin") input >> skin;
    else if (property=="mesh") input >> mesh;
    else parser.warning();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Sampler
//
void gltf::Sampler::report()
{
    vsg::info("Sampler { ");
    vsg::info("    name: ", name);
    vsg::info("    minFilter: ", minFilter);
    vsg::info("    magFilter: ", magFilter);
    vsg::info("    wrapS: ", wrapS);
    vsg::info("    wrapT: ", wrapT);
    vsg::info("} ");
}

void gltf::Sampler::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else parser.warning();
}

void gltf::Sampler::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="minFilter") input >> minFilter;
    else if (property=="magFilter") input >> magFilter;
    else if (property=="wrapS") input >> wrapS;
    else if (property=="wrapT") input >> wrapT;
    else parser.warning();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Scene
//
void gltf::Scene::report()
{
    vsg::info("Scene = { name = ", name, ", nodes = ", nodes.values.size(), " } ", this);
}

void gltf::Scene::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "nodes") parser.read_array(nodes);
    else parser.warning();
}

void gltf::Scene::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else parser.warning();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Texture
//
void gltf::Texture::report()
{
    vsg::info("Texture = { name = ", name, ", sampler = ", sampler, ", ", source, " } ", this);
}

void gltf::Texture::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else parser.warning();
}

void gltf::Texture::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="sampler") input >> sampler;
    else if (property=="source") input >> source;
    else parser.warning();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Animation
//
void gltf::AnimationTarget::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="path") parser.read_string(path);
    else parser.warning();
}

void gltf::AnimationTarget::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="node") input >> node;
    else parser.warning();
}

void gltf::AnimationChannel::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property=="sampler") input >> sampler;
    else parser.warning();
}

void gltf::AnimationChannel::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="target") parser.read_object(target);
}

void gltf::AnimationSampler::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="interpolation") parser.read_string(interpolation);
    else parser.warning();
}

void gltf::AnimationSampler::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& in_input)
{
    if (property=="input") in_input >> input;
    else if (property=="output") in_input >> output;
    else parser.warning();
}

void gltf::Animation::report()
{
    vsg::info("Animation { ", name, ", extras = ", extras.object, ", channels.size() = ", channels.values.size(), ", samplers.size() = ", samplers.values.size());
}

void gltf::Animation::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "channels") parser.read_array(channels);
    else if (property == "samplers") parser.read_array(samplers);
    else parser.warning();
}

void gltf::Animation::read_string(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property=="name") parser.read_string(name);
    else parser.warning();
}

void gltf::Animation::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "extras") parser.read_object(extras);
    else parser.warning();
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//
// glTF
//
void gltf::glTF::report()
{
    asset.report();
    accessors.report();
    bufferViews.report();
    buffers.report();
    images.report();
    materials.report();
    meshes.report();
    nodes.report();
    samplers.report();
    vsg::info("scene = ", scene);
    scenes.report();
    textures.report();
    animations.report();
}

void gltf::glTF::read_array(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "extensionsUsed") parser.read_array(extensionsUsed);
    else if (property == "extensionsRequired") parser.read_array(extensionsRequired);
    else if (property == "accessors") parser.read_array(accessors);
    else if (property == "animations") parser.read_array(animations);
    else if (property == "buffers") parser.read_array(buffers);
    else if (property == "bufferViews") parser.read_array(bufferViews);
    else if (property == "cameras")
    {
        vsg::info("cameras schema required (",property,") ");
    }
    else if (property == "materials") parser.read_array(materials);
    else if (property == "meshes") parser.read_array(meshes);
    else if (property == "nodes") parser.read_array(nodes);
    else if (property == "samplers") parser.read_array(samplers);
    else if (property == "scenes") { parser.read_array(scenes); }
    else if (property == "skins")
    {
        vsg::info("skins schema required (",property,") ");
    }
    else if (property == "images") parser.read_array(images);
    else if (property == "textures") parser.read_array(textures);
    else parser.warning();
}

void gltf::glTF::read_object(vsg::JSONParser& parser, const std::string_view& property)
{
    if (property == "asset")
    {
        parser.read_object(asset);
    }
    else parser.warning();
}

void gltf::glTF::read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input)
{
    if (property == "scene") input >> scene;
    else parser.warning();
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

    vsg::JSONParser parser;
    parser.level =  level;

    glTF schema;

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
        schema.report();
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
