#include <vsg/all.h>

#ifdef vsgXchange_FOUND
#include <vsgXchange/all.h>
#include <vsgXchange/images.h>
#endif

#include <iostream>

#include "gltf.h"
#include "bin.h"

int main(int argc, char** argv)
{
    auto options = vsg::Options::create();
    options->sharedObjects = vsg::SharedObjects::create();
    options->fileCache = vsg::getEnv("VSG_FILE_CACHE");
    options->paths = vsg::getEnvPaths("VSG_FILE_PATH");

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->windowTitle = "gltf-experiments";

    // set up defaults and read command line arguments to override them
    vsg::CommandLine arguments(&argc, argv);
    windowTraits->debugLayer = arguments.read({"--debug","-d"});
    windowTraits->apiDumpLayer = arguments.read({"--api","-a"});
    if (arguments.read({"--fullscreen", "--fs"})) windowTraits->fullscreen = true;
    if (arguments.read({"--window", "-w"}, windowTraits->width, windowTraits->height)) { windowTraits->fullscreen = false; }
    auto horizonMountainHeight = arguments.value(0.0, "--hmh");
    arguments.read("--screen", windowTraits->screenNum);
    arguments.read("--display", windowTraits->display);

    if (arguments.errors()) return arguments.writeErrorMessages(std::cerr);

    uint32_t numOperationThreads = 0;
    if (arguments.read("--ot", numOperationThreads)) options->operationThreads = vsg::OperationThreads::create(numOperationThreads);

    auto gltf = vsgXchange::gltf::create();
    if (int log_level = 0; arguments.read("--log-level", log_level)) gltf->level = vsg::Logger::Level(log_level);

    if (arguments.read("--old"))
    {
        options->add(vsgXchange::all::create());
    }
    else
    {
        options->add(gltf);
        options->add(vsgXchange::bin::create());
        options->add(vsgXchange::images::create());
    }

    arguments.read(options);

    auto outputFilename = arguments.value<vsg::Path>("", "-o");

    auto group = vsg::Objects::create();

    auto before_read = vsg::clock::now();

    // read any vsg files from command line arguments
    for (int i=1; i<argc; ++i)
    {
        vsg::Path filename = arguments[i];
        if (auto object = vsg::read(filename, options))
        {
            group->addChild(object);
            arguments.remove(i, 1);
            --i;
        }
    }


    if (group->children.empty())
    {
        return 1;
    }

    auto after_read = vsg::clock::now();
    std::cout<<"time to read data "<<std::chrono::duration<double, std::chrono::seconds::period>(after_read - before_read).count()<<std::endl;


    if (outputFilename)
    {
        if (group->children.size() == 1) vsg::write(group->children[0], outputFilename, options);
        else vsg::write(group, outputFilename, options);

        return 0;
    }

    vsg::ref_ptr<vsg::Node> scene;
    if (group->children.size() == 1)
    {
        scene = group->children[0].cast<vsg::Node>();
    }
    else
    {
        auto node_group = vsg::Group::create();
        for(auto child : group->children)
        {
            if (auto node = child.cast<vsg::Node>())
            {
                node_group->addChild(node);
            }
        }
    }

    if (!scene)
    {
        std::cout<<"No 3D model to render"<<std::endl;
        return 0;
    }

    auto numFrames = arguments.value<uint32_t>(-1, "--nf");
    if (numFrames == 0) return 0;

    // create the viewer and assign window(s) to it
    auto viewer = vsg::Viewer::create();

    vsg::ref_ptr<vsg::Window> window(vsg::Window::create(windowTraits));
    if (!window)
    {
        std::cout<<"Could not create window."<<std::endl;
        return 1;
    }

    viewer->addWindow(window);

    // compute the bounds of the scene graph to help position the camera
    vsg::ComputeBounds computeBounds;
    scene->accept(computeBounds);
    vsg::dvec3 centre = (computeBounds.bounds.min+computeBounds.bounds.max)*0.5;
    double radius = vsg::length(computeBounds.bounds.max-computeBounds.bounds.min)*0.6;
    double nearFarRatio = 0.0001;

    // set up the camera
    auto lookAt = vsg::LookAt::create(centre+vsg::dvec3(0.0, -radius*3.5, 0.0), centre, vsg::dvec3(0.0, 0.0, 1.0));

    vsg::ref_ptr<vsg::ProjectionMatrix> perspective;
    if (vsg::ref_ptr<vsg::EllipsoidModel> ellipsoidModel(scene->getObject<vsg::EllipsoidModel>("EllipsoidModel")); ellipsoidModel)
    {
        // EllipsoidPerspective is useful for whole earth databases where per frame management of the camera's near & far values is optimized
        // to the current view relative to an ellipsoid model of the earth so that when near to the earth the near and far planes are pulled in close to the eye
        // and when far away from the earth's surface the far plane is pushed out to ensure that it encompasses the horizon line, accounting for mountains over the horizon.
        perspective = vsg::EllipsoidPerspective::create(lookAt, ellipsoidModel, 30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio, horizonMountainHeight);
    }
    else
    {
        perspective = vsg::Perspective::create(30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio*radius, radius * 4.5);
    }

    auto camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));

    // add close handler to respond to pressing the window's close window button and to pressing escape
    viewer->addEventHandler(vsg::CloseHandler::create(viewer));

    // add a trackball event handler to control the camera view using the mouse
    viewer->addEventHandler(vsg::Trackball::create(camera));

    // create a command graph to render the scene on the specified window
    auto commandGraph = vsg::createCommandGraphForView(window, camera, scene);
    viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});

    // compile all the Vulkan objects and transfer data required to render the scene
    viewer->compile();

    // rendering main loop
    while (viewer->advanceToNextFrame() && (numFrames < 0 || numFrames-- > 0))
    {
        // pass any events into EventHandlers assigned to the Viewer
        viewer->handleEvents();

        viewer->update();

        viewer->recordAndSubmit();

        viewer->present();
    }

    // clean up done automatically thanks to ref_ptr<>
    return 0;
}
