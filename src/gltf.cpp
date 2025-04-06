/* <editor-fold desc="MIT License">

Copyright(c) 2025 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include "gltf.h"

using namespace vsgXchange;

gltf::gltf()
{
}

vsg::ref_ptr<vsg::Object> gltf::read(const vsg::Path& filename, vsg::ref_ptr<const vsg::Options> options) const
{
    vsg::Path ext = (options && options->extensionHint) ? options->extensionHint : vsg::lowerCaseFileExtension(filename);
    vsg::Path filenameToUse = vsg::findFile(filename, options);
    if (!filenameToUse) return {};


    vsg::info("gltf::read(",filename ,") Not implemented yet");
    return {};
}

vsg::ref_ptr<vsg::Object> gltf::read(std::istream&, vsg::ref_ptr<const vsg::Options> options) const
{
    if (!options || !options->extensionHint) return {};

    vsg::info("gltf::read(std::istream&) Not implemented yet");
    return {};
}

vsg::ref_ptr<vsg::Object> gltf::read(const uint8_t* ptr, size_t size, vsg::ref_ptr<const vsg::Options> options) const
{
    if (!options || !options->extensionHint) return {};

    vsg::info("gltf::read(const uint8_t* ptr) Not implemented yet");
    return {};
}


bool gltf::getFeatures(Features& features) const
{
    vsg::ReaderWriter::FeatureMask supported_features = static_cast<vsg::ReaderWriter::FeatureMask>(vsg::ReaderWriter::READ_FILENAME | vsg::ReaderWriter::READ_ISTREAM | vsg::ReaderWriter::READ_MEMORY);
    features.extensionFeatureMap[".gltf"] = supported_features;
    features.extensionFeatureMap[".glb"] = supported_features;

    return true;
}
