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

#include "gltf.h"

#include <vsg/nodes/Group.h>
#include <vsg/nodes/MatrixTransform.h>
#include <vsg/nodes/VertexIndexDraw.h>
#include <vsg/nodes/Switch.h>
#include <vsg/app/Camera.h>
#include <vsg/maths/transform.h>

using namespace vsgXchange;

gltf::SceneGraphBuilder::SceneGraphBuilder()
{
}

void gltf::SceneGraphBuilder::assign_extras(ExtensionsExtras& src, vsg::Object& dest)
{
    if (src.extras)
    {
        if (src.extras->object)
        {
            dest.setObject("extras", src.extras->object);
        }
        else if (src.extras->objects)
        {
            dest.setObject("extras", src.extras->objects);
        }
    }
};

void gltf::SceneGraphBuilder::assign_name_extras(NameExtensionsExtras& src, vsg::Object& dest)
{
    if (!src.name.empty())
    {
        dest.setValue("name", src.name);
    }

    if (src.extras)
    {
        if (src.extras->object)
        {
            vsg::info("Assignig extras object ", src.extras->object);
            dest.setObject("extras", src.extras->object);
        }
        else if (src.extras->objects)
        {
            vsg::info("Assignig extras objects ", src.extras->object);
            dest.setObject("extras", src.extras->objects);
        }
    }
};

vsg::ref_ptr<vsg::Data> gltf::SceneGraphBuilder::createBuffer(vsg::ref_ptr<gltf::Buffer> gltf_buffer)
{
    vsg::info("Assigning buffer ", gltf_buffer->data);
    return gltf_buffer->data;
}

vsg::ref_ptr<vsg::Data> gltf::SceneGraphBuilder::createBufferView(vsg::ref_ptr<gltf::BufferView> gltf_bufferView)
{
    if (!gltf_bufferView->buffer)
    {
        vsg::info("Warning: no buffer available to create BufferView.");
        return {};
    }

    if (!vsg_buffers[gltf_bufferView->buffer.value])
    {
        vsg::info("Warning: no vsg::Data available to create BufferView.");
        return {};
    }

    // TODO: deciode whether we need to do anything with the BufferView.target
    auto vsg_buffer =  vsg::ubyteArray::create(vsg_buffers[gltf_bufferView->buffer.value],
                                                gltf_bufferView->byteOffset,
                                                gltf_bufferView->byteStride,
                                                gltf_bufferView->byteLength / gltf_bufferView->byteStride);

    vsg::info("Created BufferView ", vsg_buffer);
    return vsg_buffer;
}

vsg::ref_ptr<vsg::Data> gltf::SceneGraphBuilder::createAccessor(vsg::ref_ptr<gltf::Accessor> gltf_accessor)
{
    if (!gltf_accessor->bufferView)
    {
        vsg::info("Warning: no bufferView available to create Accessor.");
        return {};
    }

    if (!vsg_bufferViews[gltf_accessor->bufferView.value])
    {
        vsg::info("Warning: no vsg::Data available to create BufferView.");
        return {};
    }

    auto bufferView = vsg_bufferViews[gltf_accessor->bufferView.value];

    vsg::ref_ptr<vsg::Data> vsg_data;
    switch(gltf_accessor->componentType)
    {
        case(5120): // BYTE
            if      (gltf_accessor->type=="SCALAR") vsg_data = vsg::byteArray::create(bufferView, gltf_accessor->byteOffset, 1, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC2")   vsg_data = vsg::bvec2Array::create(bufferView, gltf_accessor->byteOffset, 2, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC3")   vsg_data = vsg::bvec3Array::create(bufferView, gltf_accessor->byteOffset, 3, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC4")   vsg_data = vsg::bvec4Array::create(bufferView, gltf_accessor->byteOffset, 4, gltf_accessor->count);
            else vsg::warn("Unsupported gltf_accessor->componentType = ", gltf_accessor->componentType);
            break;
        case(5121): // UNSIGNED_BYTE
            if      (gltf_accessor->type=="SCALAR") vsg_data = vsg::ubyteArray::create(bufferView, gltf_accessor->byteOffset, 1, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC2")   vsg_data = vsg::ubvec2Array::create(bufferView, gltf_accessor->byteOffset, 2, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC3")   vsg_data = vsg::ubvec3Array::create(bufferView, gltf_accessor->byteOffset, 3, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC4")   vsg_data = vsg::ubvec4Array::create(bufferView, gltf_accessor->byteOffset, 4, gltf_accessor->count);
            else vsg::warn("Unsupported gltf_accessor->componentType = ", gltf_accessor->componentType);
            break;
        case(5122): // SHORT
            if      (gltf_accessor->type=="SCALAR") vsg_data = vsg::shortArray::create(bufferView, gltf_accessor->byteOffset, 1, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC2")   vsg_data = vsg::svec2Array::create(bufferView, gltf_accessor->byteOffset, 2, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC3")   vsg_data = vsg::svec3Array::create(bufferView, gltf_accessor->byteOffset, 3, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC4")   vsg_data = vsg::svec4Array::create(bufferView, gltf_accessor->byteOffset, 4, gltf_accessor->count);
            else vsg::warn("Unsupported gltf_accessor->componentType = ", gltf_accessor->componentType);
            break;
        case(5123): // UNSIGNED_SHORT
            if      (gltf_accessor->type=="SCALAR") vsg_data = vsg::ushortArray::create(bufferView, gltf_accessor->byteOffset, 1, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC2")   vsg_data = vsg::usvec2Array::create(bufferView, gltf_accessor->byteOffset, 2, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC3")   vsg_data = vsg::usvec3Array::create(bufferView, gltf_accessor->byteOffset, 3, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC4")   vsg_data = vsg::usvec4Array::create(bufferView, gltf_accessor->byteOffset, 4, gltf_accessor->count);
            else vsg::warn("Unsupported gltf_accessor->componentType = ", gltf_accessor->componentType);
            break;
        case(5125): // UNSIGNED_INT
            if      (gltf_accessor->type=="SCALAR") vsg_data = vsg::uintArray::create(bufferView, gltf_accessor->byteOffset, 1, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC2")   vsg_data = vsg::uivec2Array::create(bufferView, gltf_accessor->byteOffset, 2, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC3")   vsg_data = vsg::uivec3Array::create(bufferView, gltf_accessor->byteOffset, 3, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC4")   vsg_data = vsg::uivec4Array::create(bufferView, gltf_accessor->byteOffset, 4, gltf_accessor->count);
            else vsg::warn("Unsupported gltf_accessor->componentType = ", gltf_accessor->componentType);
            break;
        case(5126): // FLOAT
            if      (gltf_accessor->type=="SCALAR") vsg_data = vsg::byteArray::create(bufferView, gltf_accessor->byteOffset, 1, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC2")   vsg_data = vsg::vec2Array::create(bufferView, gltf_accessor->byteOffset, 2, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC3")   vsg_data = vsg::vec3Array::create(bufferView, gltf_accessor->byteOffset, 3, gltf_accessor->count);
            else if (gltf_accessor->type=="VEC4")   vsg_data = vsg::vec4Array::create(bufferView, gltf_accessor->byteOffset, 4, gltf_accessor->count);
            //else if (gltf_accessor->type=="MAT2")   vsg_data = vsg::mat2Array::create(bufferView, gltf_accessor->byteOffset, 4, gltf_accessor->count);
            //else if (gltf_accessor->type=="MAT3")   vsg_data = vsg::mat3Array::create(bufferView, gltf_accessor->byteOffset, 4, gltf_accessor->count);
            else if (gltf_accessor->type=="MAT4")   vsg_data = vsg::mat4Array::create(bufferView, gltf_accessor->byteOffset, 4, gltf_accessor->count);
            else vsg::warn("Unsupported gltf_accessor->componentType = ", gltf_accessor->componentType);
            break;
    }

    vsg::info("created vsg_Accessor ", vsg_data, ", gltf_accessor->count = ", gltf_accessor->count);

    return vsg_data;
}


vsg::ref_ptr<vsg::Camera> gltf::SceneGraphBuilder::createCamera(vsg::ref_ptr<gltf::Camera> gltf_camera)
{
    auto vsg_camera = vsg::Camera::create();

    if (gltf_camera->perspective)
    {
        auto perspective = gltf_camera->perspective;
        // note, vsg::Perspective implements GLU Perspective style settings so uses degress for fov, while glTF uses radians so need to convert to degrees
        vsg_camera->projectionMatrix = vsg::Perspective::create(vsg::degrees(perspective->yfov), perspective->aspectRatio, perspective->znear, perspective->zfar);
    }

    if (gltf_camera->orthographic)
    {
        auto orthographic = gltf_camera->orthographic;
        double halfWidth = orthographic->xmag; // TODO: figure how to map to GLU/VSG style orthographic
        double halfHeight = orthographic->ymag; // TODO: figure how to mapto GLU/VSG style orthographic
        vsg_camera->projectionMatrix = vsg::Orthographic::create(-halfWidth, halfWidth, -halfHeight, halfHeight, orthographic->znear, orthographic->zfar);
    }

    vsg::info("Assigned camera");

    vsg_camera->name = gltf_camera->name;
    assign_extras(*gltf_camera, *vsg_camera);

    return vsg_camera;
}

vsg::ref_ptr<vsg::Node> gltf::SceneGraphBuilder::createMesh(vsg::ref_ptr<gltf::Mesh> gltf_mesh)
{
/*
    struct Attributes : public vsg::Inherit<vsg::JSONParser::Schema, Attributes>
    {
        std::map<std::string, glTFid> values;
    };

    struct Primitive : public vsg::Inherit<ExtensionsExtras, Primitive>
    {
        Attributes attributes;
        glTFid indices;
        glTFid material;
        uint32_t mode = 0;
        vsg::ObjectsSchema<Attributes> targets;
    };

    struct Mesh : public vsg::Inherit<NameExtensionsExtras, Mesh>
    {
        vsg::ObjectsSchema<Primitive> primitives;
        vsg::ValuesSchema<double> weights;
    };
*/

    const VkPrimitiveTopology topologyLookup[] = {
            VK_PRIMITIVE_TOPOLOGY_POINT_LIST, // 0, POINTS
            VK_PRIMITIVE_TOPOLOGY_LINE_LIST, // 1, LINES
            VK_PRIMITIVE_TOPOLOGY_LINE_LIST, // 2, LINE_LOOP, need special handling
            VK_PRIMITIVE_TOPOLOGY_LINE_STRIP, // 3, LINE_STRIP
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, // 4, TRIANGLES
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, // 5, TRIANGLE_STRIP
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN // 6, TRIANGLE_FAN
    };

    vsg::info("mesh = {");
    vsg::info("    primitives = ", gltf_mesh->primitives.values.size());
    vsg::info("    weight = ", gltf_mesh->weights.values.size());


    std::vector<vsg::ref_ptr<vsg::Node>> nodes;

    for(auto& primitive : gltf_mesh->primitives.values)
    {
        vsg::info("    primitive = {");
        vsg::info("        attributes = {");
        for(auto& [semantic, id] : primitive->attributes.values)
        {
             vsg::info("            ", semantic, ", ", id);
        }
        vsg::info("        }");
        vsg::info("        indices = ", primitive->indices);
        vsg::info("        material = ", primitive->material);
        vsg::info("        mode = ", primitive->mode);
        vsg::info("        targets = ", primitive->targets.values.size()) ;

        vsg::info("        * topology = ", topologyLookup[primitive->mode]) ;
        if (primitive->mode==2) vsg::info("        * LINE_LOOP needs special handling.");

        vsg::info("    }");


        auto vid = vsg::VertexIndexDraw::create();
        nodes.push_back(vid);
        assign_extras(*primitive, *vid);


        auto& attributes = primitive->attributes.values;
        std::vector<vsg::ref_ptr<vsg::Data>> arrays;

        // TODO need to map to ShadserSet values and use GraphicsPipelineConfigurator.
        if (auto vertices_itr = attributes.find("POSITION"); vertices_itr != attributes.end())
        {
            arrays.push_back(vsg_accessors[vertices_itr->second.value]);
        }
        if (auto vertices_itr = attributes.find("NORMAL"); vertices_itr != attributes.end())
        {
            arrays.push_back(vsg_accessors[vertices_itr->second.value]);
        }
        if (auto vertices_itr = attributes.find("TEXCOORD_0"); vertices_itr != attributes.end())
        {
            arrays.push_back(vsg_accessors[vertices_itr->second.value]);
        }
        if (auto vertices_itr = attributes.find("TANGENT"); vertices_itr != attributes.end())
        {
            arrays.push_back(vsg_accessors[vertices_itr->second.value]);
        }

        vid->assignArrays(arrays);

        if (primitive->indices)
        {
            auto indices = vsg_accessors[primitive->indices.value];
            vid->assignIndices(indices);
        }
    }

    if (nodes.empty())
    {
        vsg::warn("Empty mesh");
        return {};
    }

    vsg::ref_ptr<vsg::Node> vsg_mesh;
    if (nodes.size()==1)
    {
        vsg::info("Mesh with single primtiive");
        vsg_mesh = nodes.front();
    }
    else
    {
        vsg::info("Mesh with multiple primtiives - could possible use vsg::Geomterty.");
        auto group = vsg::Group::create();
        for(auto node : nodes)
        {
            group->addChild(node);
        }

        vsg_mesh = group;
    }

    assign_name_extras(*gltf_mesh, *vsg_mesh);

    return vsg_mesh;
}

vsg::ref_ptr<vsg::Node> gltf::SceneGraphBuilder::createNode(vsg::ref_ptr<gltf::Node> gltf_node)
{
    vsg::ref_ptr<vsg::Node> vsg_node;

    bool isTransform = !(gltf_node->matrix.values.empty()) ||
                        !(gltf_node->rotation.values.empty()) ||
                        !(gltf_node->scale.values.empty()) ||
                        !(gltf_node->translation.values.empty());

    size_t numChildren = gltf_node->children.values.size();
    if (gltf_node->camera) ++numChildren;
    if (gltf_node->skin) ++numChildren;
    if (gltf_node->mesh) ++numChildren;

    if (isTransform)
    {
        auto transform = vsg::MatrixTransform::create();
        if (gltf_node->camera) transform->addChild(vsg_cameras[gltf_node->camera.value]);
        else if (gltf_node->skin) transform->addChild(vsg_skins[gltf_node->skin.value]);
        else if (gltf_node->mesh) transform->addChild(vsg_meshes[gltf_node->mesh.value]);

        if (gltf_node->matrix.values.size()==16)
        {
            auto& m = gltf_node->matrix.values;
            transform->matrix.set(m[0], m[1], m[2], m[3],
                                    m[4], m[5], m[6], m[7],
                                    m[8], m[9], m[10], m[11],
                                    m[12], m[13], m[14], m[15]);
        }
        else
        {
            auto& t = gltf_node->translation.values;
            auto& r = gltf_node->rotation.values;
            auto& s = gltf_node->scale.values;

            vsg::dvec3 vsg_t(0.0, 0.0, 0.0);
            vsg::dquat vsg_r;
            vsg::dvec3 vsg_s(1.0, 1.0, 1.0);

            if (t.size()>=3) vsg_t.set(t[0], t[1], t[2]);
            if (r.size()>=4) vsg_r.set(t[0], r[1], r[2], r[3]);
            if (s.size()>=3) vsg_s.set(s[0], s[1], s[2]);

            transform->matrix = vsg::translate(vsg_t) * vsg::rotate(vsg_r) * vsg::scale(vsg_s);
        }

        vsg::info("Created transfrom ", transform, " with numChildren = ", numChildren);

        vsg_node = transform;
    }
    else if (numChildren > 1)
    {
        auto group = vsg::Group::create();

        if (gltf_node->camera) group->addChild(vsg_cameras[gltf_node->camera.value]);
        else if (gltf_node->skin) group->addChild(vsg_skins[gltf_node->skin.value]);
        else if (gltf_node->mesh) group->addChild(vsg_meshes[gltf_node->mesh.value]);

        vsg::info("Created group ", group, " with numChildren = ", numChildren);

        vsg_node = group;
    }
    else
    {
        if (gltf_node->camera) vsg_node = vsg_cameras[gltf_node->camera.value];
        else if (gltf_node->skin) vsg_node = vsg_skins[gltf_node->skin.value];
        else if (gltf_node->mesh) vsg_node = vsg_meshes[gltf_node->mesh.value];
        else vsg_node = vsg::Group::create(); // TODO: single child so should this just point to the child?

        vsg::info("Created node ", vsg_node, " with numChildren = ", numChildren);
    }

    assign_name_extras(*gltf_node, *vsg_node);

    return vsg_node;
}

vsg::ref_ptr<vsg::Node> gltf::SceneGraphBuilder::createScene(vsg::ref_ptr<gltf::Scene> gltf_scene)
{
    vsg::ref_ptr<vsg::Node> vsg_scene;
    if (gltf_scene->nodes.values.size()==1)
    {
        vsg_scene = vsg_nodes[gltf_scene->nodes.values[0].value];
    }
    else
    {
        auto group = vsg::Group::create();
        for(auto& id : gltf_scene->nodes.values)
        {
            group->addChild(vsg_nodes[id.value]);
        }
        vsg_scene = group;

    }
    assign_name_extras(*gltf_scene, *vsg_scene);

    return vsg_scene;
}

vsg::ref_ptr<vsg::Object> gltf::SceneGraphBuilder::createSceneGraph(vsg::ref_ptr<gltf::glTF> root, vsg::ref_ptr<const vsg::Options> options)
{
    if (!root) return {};

    vsg_buffers.resize(root->buffers.values.size());
    for(size_t bi = 0; bi<root->buffers.values.size(); ++bi)
    {
        vsg_buffers[bi] = createBuffer(root->buffers.values[bi]);
    }

    vsg_bufferViews.resize(root->bufferViews.values.size());
    for(size_t bvi = 0; bvi<root->bufferViews.values.size(); ++bvi)
    {
        vsg_bufferViews[bvi] = createBufferView(root->bufferViews.values[bvi]);
    }

    vsg_accessors.resize(root->accessors.values.size());
    for(size_t ai = 0; ai<root->accessors.values.size(); ++ai)
    {
        vsg_accessors[ai] = createAccessor(root->accessors.values[ai]);
    }

    vsg::info("create cameras = ", root->cameras.values.size());
    vsg_cameras.resize(root->cameras.values.size());
    for(size_t ci=0; ci<root->cameras.values.size(); ++ci)
    {
        vsg_cameras[ci] = createCamera(root->cameras.values[ci]);
    }

    vsg::info("create skins = ", root->skins.values.size());
    vsg_skins.resize(root->skins.values.size());
    for(size_t si=0; si<root->skins.values.size(); ++si)
    {
        auto& gltf_skin = root->skins.values[si];
        auto& vsg_skin = vsg_skins[si];
        vsg_skin = vsg::Node::create();

        assign_name_extras(*gltf_skin, *vsg_skin);
    }

    vsg::info("create meshes = ", root->meshes.values.size());
    vsg_meshes.resize(root->meshes.values.size());
    for(size_t mi=0; mi<root->meshes.values.size(); ++mi)
    {
        vsg_meshes[mi] = createMesh(root->meshes.values[mi]);
    }

    vsg::info("create nodes = ", root->nodes.values.size());
    vsg_nodes.resize(root->nodes.values.size());
    for(size_t ni=0; ni<root->nodes.values.size(); ++ni)
    {
        vsg_nodes[ni] = createNode(root->nodes.values[ni]);
    }

    for(size_t ni=0; ni<root->nodes.values.size(); ++ni)
    {
        auto& gltf_node = root->nodes.values[ni];

        if (!gltf_node->children.values.empty())
        {
            auto vsg_group = vsg_nodes[ni].cast<vsg::Group>();
            for(auto id : gltf_node->children.values)
            {
                auto vsg_child = vsg_nodes[id.value];
                if (vsg_child) vsg_group->addChild(vsg_child);
                else vsg::info("Unassigned vsg_child");
            }
        }
    }

    for(auto& vsg_node : vsg_nodes)
    {
        vsg::info("vsg_node = ", vsg_node);
    }

    vsg::info("scene = ", root->scene);
    vsg::info("scenes = ", root->scenes.values.size());

    vsg_scenes.resize(root->scenes.values.size());
    for(size_t sci = 0; sci < root->scenes.values.size(); ++sci)
    {
        vsg_scenes[sci] = createScene(root->scenes.values[sci]);
    }

    // create root node
    if (vsg_scenes.size() > 1)
    {
        auto vsg_switch = vsg::Switch::create();
        for(size_t sci = 0; sci < root->scenes.values.size(); ++sci)
        {
            auto& vsg_scene = vsg_scenes[sci];
            vsg_switch->addChild(true, vsg_scene);
        }

        vsg_switch->setSingleChildOn(root->scene.value);

        vsg::info("Created a scenes with a switch");

        return vsg_switch;
    }
    else
    {
        vsg::info("Created a single scene");
        return vsg_scenes.front();
    }
}

