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

    // TODO: need to add exports for Windows.

    /// gltf ReaderWriter
    class gltf : public vsg::Inherit<vsg::ReaderWriter, gltf>
    {
    public:
        gltf();

        vsg::ref_ptr<vsg::Object> read(const vsg::Path&, vsg::ref_ptr<const vsg::Options>) const override;
        vsg::ref_ptr<vsg::Object> read(std::istream&, vsg::ref_ptr<const vsg::Options>) const override;
        vsg::ref_ptr<vsg::Object> read(const uint8_t* ptr, size_t size, vsg::ref_ptr<const vsg::Options> options = {}) const override;

        vsg::ref_ptr<vsg::Object> _read(std::istream&, vsg::ref_ptr<const vsg::Options>, const vsg::Path& filename = {}) const;

        vsg::Logger::Level level = vsg::Logger::LOGGER_WARN;

        bool supportedExtension(const vsg::Path& ext) const;

        bool getFeatures(Features& features) const override;

        static constexpr const char* report = "report";

        bool readOptions(vsg::Options& options, vsg::CommandLine& arguments) const override;

    public:

        struct glTFid
        {
            static const uint32_t invalid_value = std::numeric_limits<uint32_t>::max();

            uint32_t value = invalid_value;

            bool valid() const { return value != invalid_value; }

            explicit operator bool() const noexcept { return valid(); }
        };


        struct Extensions : public vsg::Inherit<vsg::JSONtoMetaDataSchema, Extensions>
        {
            std::map<std::string, vsg::ref_ptr<vsg::JSONParser::Schema>> values;

            void report();

            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        using Extras = vsg::JSONtoMetaDataSchema;

        struct ExtensionsExtras : public vsg::Inherit<vsg::JSONParser::Schema, ExtensionsExtras>
        {
            vsg::ref_ptr<Extensions> extensions;
            vsg::ref_ptr<Extras> extras;

            void report();

            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct NameExtensionsExtras : public vsg::Inherit<ExtensionsExtras, NameExtensionsExtras>
        {
            std::string name;

            void report();

            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct SparseIndices : public vsg::Inherit<NameExtensionsExtras, SparseIndices>
        {
            glTFid bufferView;
            uint32_t byteOffset = 0;
            uint32_t componentType = 0;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct SparseValues : public vsg::Inherit<NameExtensionsExtras, SparseValues>
        {
            glTFid bufferView;
            uint32_t byteOffset = 0;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct Sparse : public vsg::Inherit<NameExtensionsExtras, Sparse>
        {
            uint32_t count = 0;
            vsg::ref_ptr<SparseIndices> indices;
            vsg::ref_ptr<SparseValues> values;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct Accessor : public vsg::Inherit<NameExtensionsExtras, Accessor>
        {
            glTFid bufferView;
            uint32_t byteOffset = 0;
            uint32_t componentType = 0;
            bool normalized = false;
            uint32_t count = 0;
            std::string type;
            vsg::ValuesSchema<double> max;
            vsg::ValuesSchema<double> min;
            vsg::ref_ptr<Sparse> sparse;

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_bool(vsg::JSONParser& parser, const std::string_view& property, bool value) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct Asset : public vsg::Inherit<ExtensionsExtras, Asset>
        {
            std::string copyright;
            std::string version;
            std::string generator;
            std::string minVersion;

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct BufferView : public vsg::Inherit<NameExtensionsExtras, BufferView>
        {
            glTFid buffer;
            uint32_t byteOffset = 0;
            uint32_t byteLength = 0;
            uint32_t byteStride = 4;
            uint32_t target = 0;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct Buffer : public vsg::Inherit<NameExtensionsExtras, Buffer>
        {
            std::string_view uri;
            uint32_t byteLength = 0;

            // loaded from uri
            vsg::ref_ptr<vsg::Data> data;

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct Image : public vsg::Inherit<NameExtensionsExtras, Image>
        {
            std::string_view uri;
            std::string mimeType;
            glTFid bufferView;

            // loaded from uri
            vsg::ref_ptr<vsg::Data> data;

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct TextureInfo : public vsg::Inherit<ExtensionsExtras, TextureInfo>
        {
            glTFid index;
            uint32_t texCoord = 0;

            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct PbrMetallicRoughness : public vsg::Inherit<ExtensionsExtras, PbrMetallicRoughness>
        {
            vsg::ValuesSchema<double> baseColorFactor; // default { 1.0, 1.0, 1.0, 1.0 }
            TextureInfo baseColorTexture;
            double metallicFactor = 1.0;
            double roughnessFactor = 1.0;
            TextureInfo metallicRoughnessTexture;

            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct NormalTextureInfo : public vsg::Inherit<TextureInfo, NormalTextureInfo>
        {
            double scale = 1.0;

            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct OcclusionTextureInfo : public vsg::Inherit<TextureInfo, OcclusionTextureInfo>
        {
            double strength = 1.0;

            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        /// https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Khronos/KHR_materials_specular/README.md
        struct KHR_materials_specular : public vsg::Inherit<vsg::JSONParser::Schema, KHR_materials_specular>
        {
            double specularFactor = 1.0;
            TextureInfo specularTexture;
            vsg::ValuesSchema<double> specularColorFactor;
            TextureInfo specularColorTexture;

            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        /// https://github.com/KhronosGroup/glTF/blob/main/extensions/2.0/Khronos/KHR_materials_ior/README.md
        struct KHR_materials_ior : public vsg::Inherit<vsg::JSONParser::Schema, KHR_materials_ior>
        {
            double ior = 1.5;

            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct Material : public vsg::Inherit<NameExtensionsExtras, Material>
        {
            PbrMetallicRoughness pbrMetallicRoughness;
            NormalTextureInfo normalTexture;
            OcclusionTextureInfo occlusionTexture;
            TextureInfo emissiveTexture;
            vsg::ValuesSchema<double> emissiveFactor; // default { 0.0, 0.0, 0.0 }
            std::string alphaMode = "OPAQUE";
            double alphaCutoff = 0.5;
            bool doubleSided = false;

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_bool(vsg::JSONParser& parser, const std::string_view& property, bool value) override;

        };

        struct Attributes : public vsg::Inherit<vsg::JSONParser::Schema, Attributes>
        {
            std::map<std::string, glTFid> values;

            void read_number(vsg::JSONParser&, const std::string_view& property, std::istream& input) override;
        };

        struct Primitive : public vsg::Inherit<ExtensionsExtras, Primitive>
        {
            Attributes attributes;
            glTFid indices;
            glTFid material;
            uint32_t mode = 0;
            vsg::ObjectsSchema<Attributes> targets;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct Mesh : public vsg::Inherit<NameExtensionsExtras, Mesh>
        {
            vsg::ObjectsSchema<Primitive> primitives;
            vsg::ValuesSchema<double> weights;

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct Node : public vsg::Inherit<NameExtensionsExtras, Node>
        {
            glTFid camera;
            glTFid skin;
            glTFid mesh;
            vsg::ValuesSchema<glTFid> children;
            vsg::ValuesSchema<double> matrix;
            vsg::ValuesSchema<double> rotation;
            vsg::ValuesSchema<double> scale;
            vsg::ValuesSchema<double> translation;
            vsg::ValuesSchema<double> weights;

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct Sampler : public vsg::Inherit<NameExtensionsExtras, Sampler>
        {
            uint32_t minFilter = 0;
            uint32_t magFilter = 0;
            uint32_t wrapS = 0;
            uint32_t wrapT = 0;

            // extensions
            // extras

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct Scene : public vsg::Inherit<NameExtensionsExtras, Scene>
        {
            vsg::ValuesSchema<glTFid> nodes;

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct Texture : public vsg::Inherit<NameExtensionsExtras, Texture>
        {
            glTFid sampler;
            glTFid source;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct AnimationTarget : public vsg::Inherit<ExtensionsExtras, AnimationTarget>
        {
            glTFid node;
            std::string path;

            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct AnimationChannel : public vsg::Inherit<ExtensionsExtras, AnimationChannel>
        {
            glTFid sampler;
            AnimationTarget target;

            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct AnimationSampler : public vsg::Inherit<ExtensionsExtras, AnimationSampler>
        {
            glTFid input;
            std::string interpolation;
            glTFid output;

            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct Animation : public vsg::Inherit<NameExtensionsExtras, Animation>
        {
            vsg::ObjectsSchema<AnimationChannel> channels;
            vsg::ObjectsSchema<AnimationSampler> samplers;

            void report();
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct Orthographic : public vsg::Inherit<ExtensionsExtras, Orthographic>
        {
            double xmag = 1.0;
            double ymag = 1.0;
            double znear = 1.0;
            double zfar = 1000.0;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct Perspective : public vsg::Inherit<ExtensionsExtras, Perspective>
        {
            double aspectRatio = 1.0;
            double yfov = 1.0;
            double znear = 1.0;
            double zfar = 1000.0;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
        };

        struct Camera : public vsg::Inherit<NameExtensionsExtras, Camera>
        {
            vsg::ref_ptr<Orthographic> orthographic;
            vsg::ref_ptr<Perspective> perspective;
            std::string type;

            void report();
            void read_string(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct Skins : public vsg::Inherit<NameExtensionsExtras, Skins>
        {
            glTFid inverseBindMatrices;
            glTFid skeleton;
            vsg::ValuesSchema<glTFid> joints;

            void report();
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;
            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
        };

        struct glTF : public vsg::Inherit<ExtensionsExtras, glTF>
        {
            vsg::ValuesSchema<std::string> extensionsUsed;
            vsg::ValuesSchema<std::string> extensionsRequired;
            vsg::ref_ptr<Asset> asset;
            vsg::ObjectsSchema<Accessor> accessors;
            vsg::ObjectsSchema<BufferView> bufferViews;
            vsg::ObjectsSchema<Buffer> buffers;
            vsg::ObjectsSchema<Image> images;
            vsg::ObjectsSchema<Material> materials;
            vsg::ObjectsSchema<Mesh> meshes;
            vsg::ObjectsSchema<Node> nodes;
            vsg::ObjectsSchema<Sampler> samplers;
            glTFid scene;
            vsg::ObjectsSchema<Scene> scenes;
            vsg::ObjectsSchema<Texture> textures;
            vsg::ObjectsSchema<Animation> animations;
            vsg::ObjectsSchema<Camera> cameras;
            vsg::ObjectsSchema<Skins> skins;

            void read_array(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_object(vsg::JSONParser& parser, const std::string_view& property) override;
            void read_number(vsg::JSONParser& parser, const std::string_view& property, std::istream& input) override;

            void report();

            virtual void resolveURIs(vsg::ref_ptr<const vsg::Options> options);

        };

        class SceneGraphBuilder : public vsg::Inherit<vsg::Object, SceneGraphBuilder>
        {
        public:
            SceneGraphBuilder();

            std::vector<vsg::ref_ptr<vsg::Camera>> vsg_cameras;
            std::vector<vsg::ref_ptr<vsg::Node>> vsg_skins;
            std::vector<vsg::ref_ptr<vsg::Node>> vsg_meshes;
            std::vector<vsg::ref_ptr<vsg::Node>> vsg_nodes;
            std::vector<vsg::ref_ptr<vsg::Node>> vsg_scenes;

            void assign_extras(ExtensionsExtras& src, vsg::Object& dest);
            void assign_name_extras(NameExtensionsExtras& src, vsg::Object& dest);

            vsg::ref_ptr<vsg::Camera> createCamera(vsg::ref_ptr<gltf::Camera> gltf_camera);
            vsg::ref_ptr<vsg::Node> createMesh(vsg::ref_ptr<gltf::Mesh> gltf_mesh);
            vsg::ref_ptr<vsg::Node> createNode(vsg::ref_ptr<gltf::Node> gltf_node);
            vsg::ref_ptr<vsg::Node> createScene(vsg::ref_ptr<gltf::Scene> gltf_scene);

            vsg::ref_ptr<vsg::Object> createSceneGraph(vsg::ref_ptr<gltf::glTF> root, vsg::ref_ptr<const vsg::Options> options);
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

