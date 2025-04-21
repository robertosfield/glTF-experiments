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
#include <vsg/io/JSONParser.h>

namespace vsgXchange
{

    /// gltf ReaderWriter
    class gltf : public vsg::Inherit<vsg::ReaderWriter, gltf>
    {
    public:
        gltf();

        vsg::ref_ptr<vsg::Object> read(const vsg::Path&, vsg::ref_ptr<const vsg::Options>) const override;
        vsg::ref_ptr<vsg::Object> read(std::istream&, vsg::ref_ptr<const vsg::Options>) const override;
        vsg::ref_ptr<vsg::Object> read(const uint8_t* ptr, size_t size, vsg::ref_ptr<const vsg::Options> options = {}) const override;

        vsg::ref_ptr<vsg::Object> _read(std::istream&, vsg::ref_ptr<const vsg::Options>) const;

        bool supportedExtension(const vsg::Path& ext) const;

        bool getFeatures(Features& features) const override;

    public:

        struct glTFid
        {
            static const uint32_t invalid_value = std::numeric_limits<uint32_t>::max();

            uint32_t value = invalid_value;

            bool valid() const { return value != invalid_value; }

            explicit operator bool() const noexcept { return valid(); }
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // accessor_schema
        //
        struct accessor_schema : public vsg::JSONParser::Schema
        {
            std::string name;
            vsg::JSONtoMetaDataSchema extras;
            glTFid bufferView;
            uint32_t byteOffset = 0;
            uint32_t componentType = 0;
            bool normalized = false;
            uint32_t count = 0;
            std::string type;
            vsg::ValuesSchema<double> max;
            vsg::ValuesSchema<double> min;

            // sparse
            // extensions
            // extras

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_bool(vsg::JSONParser& parser, const std::string_view& property, bool value) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;

        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // asset_schema
        //
        struct asset_scheme : public vsg::JSONParser::Schema
        {
            vsg::JSONtoMetaDataSchema extras;
            std::string copyright;
            std::string version;
            std::string generator;
            std::string minVersion;

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // bufferView_schema
        //
        struct bufferView_schema : public vsg::JSONParser::Schema
        {
            std::string name;
            vsg::JSONtoMetaDataSchema extras;
            glTFid buffer;
            uint32_t byteOffset = 0;
            uint32_t byteLength = 0;
            uint32_t byteStride = 4;
            uint32_t target = 0;

            // name
            // extensions
            // extras

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // buffer_schema
        //
        struct buffer_schema : public vsg::JSONParser::Schema
        {
            std::string name;
            vsg::JSONtoMetaDataSchema extras;
            std::string uri;
            uint32_t byteLength = 0;

            // name
            // extensions
            // extras

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // image_schema
        //
        struct image_schema : public vsg::JSONParser::Schema
        {
            std::string name;
            vsg::JSONtoMetaDataSchema extras;
            std::string uri;
            std::string mimeType;
            glTFid bufferView;

            // name
            // extensions
            // extras

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // material_schema
        //

        struct textureInfo_schema : public vsg::JSONParser::Schema
        {
            glTFid index;
            uint32_t texCoord = 0;

            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct pbrMetallicRoughness_schema : public vsg::JSONParser::Schema
        {
            vsg::ValuesSchema<double> baseColorFactor; // default { 1.0, 1.0, 1.0, 1.0 }
            textureInfo_schema baseColorTexture;
            double metallicFactor = 1.0;
            double roughnessFactor = 1.0;
            textureInfo_schema metallicRoughnessTexture;

            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct normalTextureInfo_schema : public textureInfo_schema
        {
            double scale = 1.0;

            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct occlusionTextureInfo_schema : public textureInfo_schema
        {
            double strength = 1.0;

            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct material_schema : public vsg::JSONParser::Schema
        {
            std::string name;
            pbrMetallicRoughness_schema pbrMetallicRoughness;
            normalTextureInfo_schema normalTexture;
            occlusionTextureInfo_schema occlusionTexture;
            textureInfo_schema emissiveTexture;
            vsg::ValuesSchema<double> emissiveFactor; // default { 0.0, 0.0, 0.0 }
            std::string alphaMode = "OPAQUE";
            double alphaCutoff = 0.5;
            bool doubleSided = false;

            // extensions
            // extras

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_bool(vsg::JSONParser& parser, const std::string_view& property, bool value) override;

        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // mesh_schema
        //
        struct attributes_schema : public vsg::JSONParser::Schema
        {
            std::map<std::string, uint32_t> values;

            void read_number(vsg::JSONParser&, const std::string_view& property, std::istream& input) override;
        };

        struct primitive_schema : public vsg::JSONParser::Schema
        {
            vsg::JSONtoMetaDataSchema extras;
            attributes_schema attributes;
            glTFid indices;
            glTFid material;
            uint32_t mode = 0;
            vsg::ObjectsSchema<attributes_schema> targets;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct mesh_schema : public vsg::JSONParser::Schema
        {
            std::string name;
            vsg::JSONtoMetaDataSchema extras;
            vsg::ObjectsSchema<primitive_schema> primitives;
            vsg::ValuesSchema<double> weights;

            // extensions
            // extras

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // node_schema
        //
        struct node_schema : public vsg::JSONParser::Schema
        {
            std::string name;

            glTFid camera;
            glTFid skin;
            glTFid mesh;
            vsg::ValuesSchema<uint32_t> children;
            vsg::ValuesSchema<double> matrix;
            vsg::ValuesSchema<double> rotation;
            vsg::ValuesSchema<double> scale;
            vsg::ValuesSchema<double> translation;
            vsg::ValuesSchema<double> weights;

            // extensions
            // extras

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // sampler_schema
        //
        struct sampler_schema : public vsg::JSONParser::Schema
        {
            std::string name;
            uint32_t minFilter = 0;
            uint32_t magFilter = 0;
            uint32_t wrapS = 0;
            uint32_t wrapT = 0;

            // extensions
            // extras

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // scene_schema
        //
        struct scene_schema : public vsg::JSONParser::Schema
        {
            std::string name;
            vsg::ValuesSchema<glTFid> nodes;

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
        };


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // texture_schema
        //
        struct texture_schema : public vsg::JSONParser::Schema
        {
            std::string name;
            glTFid sampler;
            glTFid source;

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };


        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // glTF_schema
        //
        struct glTF_schema : public vsg::JSONParser::Schema
        {
            vsg::ValuesSchema<std::string> extensionsUsed;
            vsg::ValuesSchema<std::string> extensionsRequired;
            asset_scheme asset;
            vsg::ObjectsSchema<accessor_schema> accessors;
            vsg::ObjectsSchema<bufferView_schema> bufferViews;
            vsg::ObjectsSchema<buffer_schema> buffers;
            vsg::ObjectsSchema<image_schema> images;
            vsg::ObjectsSchema<material_schema> materials;
            vsg::ObjectsSchema<mesh_schema> meshes;
            vsg::ObjectsSchema<node_schema> nodes;
            vsg::ObjectsSchema<sampler_schema> samplers;
            glTFid scene;
            vsg::ObjectsSchema<scene_schema> scenes;
            vsg::ObjectsSchema<texture_schema> textures;

            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;

            void report();
        };

    };


    /// output stream support for glTFid
    inline std::ostream& operator<<(std::ostream& output, const gltf::glTFid& id)
    {
        output << "glTFid("<<id.value<<")";
        return output;
    }

    /// input stream support for glTFid
    inline std::istream& operator>>(std::istream& input, gltf::glTFid& id)
    {
        input >> id.value;
        return input;
    }

}

EVSG_type_name(vsgXchange::gltf)

