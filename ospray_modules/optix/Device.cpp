/* Copyright (c) 2017, EPFL/Blue Brain Project
 * All rights reserved. Do not distribute without permission.
 * Responsible Author: Daniel Nachbaur <daniel.nachbaur@epfl.ch>
 *
 * This file is part of https://github.com/BlueBrain/ospray-modules
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "Device.h"

#include <ospray/SDK/common/Data.h>

#include "Context.h"
#include "Model.h"
#include "camera/Camera.h"
#include "framebuffer/FrameBuffer.h"
#include "geom/Cones.h"
#include "geom/Cylinders.h"
#include "geom/Geometry.h"
#include "geom/Instance.h"
#include "geom/Spheres.h"
#include "geom/TriangleMesh.h"
#include "lights/DirectionalLight.h"
#include "lights/Light.h"
#include "lights/PointLight.h"
#include "render/Material.h"
#include "render/Renderer.h"
#include "texture/Texture2D.h"

namespace bbp
{
namespace optix
{
Device::Device()
{
}

Device::~Device()
{
    Context::destroy();
}

void Device::initializeDevice()
{
    if (_initialized)
        return;

    ospray::api::Device::commit();

    _context = Context::get().getOptixContext();

    _initialized = true;
}

void Device::commit()
{
    if (!_initialized)
        initializeDevice();
}

OSPFrameBuffer Device::frameBufferCreate(const ospray::vec2i& size,
                                         const OSPFrameBufferFormat mode,
                                         const ospray::uint32 channels)
{
    return (OSPFrameBuffer)new FrameBuffer(
        size, FrameBuffer::ColorBufferFormat{mode}, channels);
}

const void* Device::frameBufferMap(OSPFrameBuffer _fb,
                                   OSPFrameBufferChannel channel)
{
    FrameBuffer* fb = (FrameBuffer*)_fb;
    return fb->mapBuffer(channel);
}

void Device::frameBufferUnmap(const void* mapped, OSPFrameBuffer _fb)
{
    Assert2(_fb != nullptr, "invalid framebuffer");
    FrameBuffer* fb = (FrameBuffer*)_fb;
    fb->unmap(mapped);
}

OSPModel Device::newModel()
{
    Model* model = new Model();
    return (OSPModel)model;
}

void Device::commit(OSPObject _object)
{
    auto lock = Context::get().getScopeLock();
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "null object in LocalDevice::commit()");
    object->commit();
}

void Device::addGeometry(OSPModel _model, OSPGeometry _geometry)
{
    Model* model = (Model*)_model;
    Assert2(model, "null model in Device::addModel()");

    Geometry* geometry = (Geometry*)_geometry;
    Assert2(geometry, "null geometry in Device::addGeometry()");

    model->geometry.push_back(geometry);
}

void Device::addVolume(OSPModel /*_model*/, OSPVolume /*_volume*/)
{
}

OSPData Device::newData(size_t nitems, OSPDataType format, const void* init,
                        int flags)
{
    ospray::Data* data = new ospray::Data(nitems, format, init, flags);
    return (OSPData)data;
}

void Device::setVoidPtr(OSPObject _object, const char* bufName, void* v)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam(bufName, v);
}

void Device::removeParam(OSPObject _object, const char* name)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(name, "invalid identifier for object parameter");
    object->removeParam(name);
}

int Device::setRegion(OSPVolume /*_volume*/, const void* /*source*/,
                      const ospray::vec3i& /*index*/,
                      const ospray::vec3i& /*count*/)
{
    return -1;
}

void Device::setString(OSPObject _object, const char* bufName, const char* s)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam<std::string>(bufName, s);
}

int Device::loadModule(const char*)
{
    return 0;
}

void Device::setBool(OSPObject _object, const char* bufName, const bool b)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam(bufName, b);
}

void Device::setFloat(OSPObject _object, const char* bufName, const float f)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam(bufName, f);
}

void Device::setInt(OSPObject _object, const char* bufName, const int i)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam(bufName, i);
}

void Device::setVec2f(OSPObject _object, const char* bufName,
                      const ospray::vec2f& v)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam(bufName, v);
}

void Device::setVec3f(OSPObject _object, const char* bufName,
                      const ospray::vec3f& v)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam(bufName, v);
}

void Device::setVec4f(OSPObject _object, const char* bufName,
                      const ospray::vec4f& v)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam(bufName, v);
}

void Device::setVec2i(OSPObject _object, const char* bufName,
                      const ospray::vec2i& v)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam(bufName, v);
}

void Device::setVec3i(OSPObject _object, const char* bufName,
                      const ospray::vec3i& v)
{
    ospray::ManagedObject* object = (ospray::ManagedObject*)_object;
    Assert2(object, "invalid object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    object->setParam(bufName, v);
}

void Device::setObject(OSPObject _target, const char* bufName, OSPObject _value)
{
    ospray::ManagedObject* target = (ospray::ManagedObject*)_target;
    ospray::ManagedObject* value = (ospray::ManagedObject*)_value;

    Assert2(target, "invalid target object handle");
    Assert2(bufName, "invalid identifier for object parameter");

    target->setParam(bufName, value);
}

OSPPixelOp Device::newPixelOp(const char* /*type*/)
{
    return nullptr;
}

void Device::setPixelOp(OSPFrameBuffer /*_fb*/, OSPPixelOp /*_op*/)
{
}

OSPRenderer Device::newRenderer(const char* type)
{
    UNUSED(type);
    Assert2(type, "invalid render type identifier");
    auto renderer = new bbp::optix::Renderer();
    return (OSPRenderer)renderer;
}

OSPCamera Device::newCamera(const char* type)
{
    UNUSED(type);
    Assert2(type, "invalid camera type identifier");
    auto camera = new bbp::optix::Camera();
    return (OSPCamera)camera;
}

OSPVolume Device::newVolume(const char* /*type*/)
{
    return nullptr;
}

OSPGeometry Device::newGeometry(const char* type)
{
    Assert2(type, "invalid render type identifier");
    Geometry* geometry = nullptr;

    const auto typeStr = std::string(type);
    if (typeStr == "cones")
        geometry = new bbp::optix::Cones;
    else if (typeStr == "cylinders")
        geometry = new bbp::optix::Cylinders;
    else if (typeStr == "spheres")
        geometry = new bbp::optix::Spheres;
    else if (typeStr == "trianglemesh" || typeStr == "triangles")
        geometry = new bbp::optix::TriangleMesh;
    else if (typeStr == "instance")
        geometry = new bbp::optix::Instance;

    Assert2(geometry != nullptr, "invalid geometry type");

    return (OSPGeometry)geometry;
}

OSPMaterial Device::newMaterial(OSPRenderer _renderer, const char* type)
{
    UNUSED(type);
    UNUSED(_renderer);
    Assert2(type != nullptr, "invalid material type identifier");

    auto material = new bbp::optix::Material();
    return (OSPMaterial)material;
}

OSPMaterial Device::newMaterial(const char* renderer_type,
                                const char* material_type)
{
    auto renderer = newRenderer(renderer_type);
    auto material = newMaterial(renderer, material_type);
    release(renderer);
    return material;
}

OSPTransferFunction Device::newTransferFunction(const char* /*type*/)
{
    return nullptr;
}

OSPLight Device::newLight(const char* type)
{
    ospray::Light* light = nullptr;

    const auto typeStr = std::string(type);
    if (typeStr == "DirectionalLight" || typeStr == "DistantLight" ||
        typeStr == "distant" || typeStr == "directional")
        light = new bbp::optix::DirectionalLight();
    else if (typeStr == "PointLight" || typeStr == "point" ||
             typeStr == "SphereLight" || typeStr == "sphere")
        light = new bbp::optix::PointLight();
    else
        light = nullptr;

    Assert2(light != nullptr, "invalid light type");

    return (OSPLight)light;
}

void Device::frameBufferClear(OSPFrameBuffer _fb,
                              const ospray::uint32 fbChannelFlags)
{
    FrameBuffer* fb = (FrameBuffer*)_fb;
    fb->clear(fbChannelFlags);
}

void Device::removeGeometry(OSPModel _model, OSPGeometry _geometry)
{
    Model* model = (Model*)_model;
    Assert2(model, "null model in LocalDevice::removeGeometry");

    Geometry* geometry = (Geometry*)_geometry;
    Assert2(geometry, "null geometry in LocalDevice::removeGeometry");

    auto it = std::find_if(model->geometry.begin(), model->geometry.end(),
                           [&](const ospray::Ref<Geometry>& g) {
                               return geometry == &*g;
                           });

    if (it != model->geometry.end())
    {
        model->geometry.erase(it);
    }
}

void Device::removeVolume(OSPModel /*_model*/, OSPVolume /*_volume*/)
{
}

float Device::renderFrame(OSPFrameBuffer _fb, OSPRenderer _renderer,
                          const ospray::uint32 fbChannelFlags)
{
    FrameBuffer* fb = (FrameBuffer*)_fb;
    Renderer* renderer = (Renderer*)_renderer;

    Assert2(fb, "invalid frame buffer handle");
    Assert2(renderer, "invalid renderer handle");

    try
    {
        return renderer->renderFrame(fb, fbChannelFlags);
    }
    catch (const std::exception& e)
    {
        ospray::postStatusMsg()
            << "================================================\n"
            << "# >>> ospray fatal error <<< \n"
            << std::string(e.what()) + '\n'
            << "================================================";
        exit(1);
    }
}

void Device::release(OSPObject _obj)
{
    if (!_obj)
        return;
    ((ospray::ManagedObject*)_obj)->refDec();
}

void Device::setMaterial(OSPGeometry _geometry, OSPMaterial _material)
{
    Geometry* geometry = (Geometry*)_geometry;
    Material* material = (Material*)_material;
    assert(geometry != nullptr);
    assert(material != nullptr);
    geometry->setMaterial(material);
}

OSPTexture Device::newTexture(const char* type)
{
    Assert2(type, "invalid render type identifier");
    Texture* texture = nullptr;

    const auto typeStr = std::string(type);
    if (typeStr == "texture2d")
        texture = new bbp::optix::Texture2D;

    Assert2(texture != nullptr, "invalid geometry type");

    return (OSPTexture)texture;
}

OSPPickResult Device::pick(OSPRenderer /*renderer*/,
                           const ospray::vec2f& /*screenPos*/)
{
    return OSPPickResult();
}

ospray::ObjectHandle Device::allocateHandle() const
{
    return ospray::ObjectHandle();
}

OSP_REGISTER_DEVICE(Device, optix);
}
}

extern "C" OSPRAY_DLLEXPORT void ospray_init_module_optix()
{
    std::cout << "#ospray: initializing optix plugin" << std::endl;
}
