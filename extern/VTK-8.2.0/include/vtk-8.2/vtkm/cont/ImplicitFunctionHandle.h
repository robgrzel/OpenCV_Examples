//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2017 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//  Copyright 2017 UT-Battelle, LLC.
//  Copyright 2017 Los Alamos National Security.
//
//  Under the terms of Contract DE-NA0003525 with NTESS,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================
#ifndef vtk_m_cont_ImplicitFunctionHandle_h
#define vtk_m_cont_ImplicitFunctionHandle_h

#include <vtkm/ImplicitFunction.h>
#include <vtkm/cont/VirtualObjectHandle.h>

namespace vtkm
{
namespace cont
{

class VTKM_ALWAYS_EXPORT ImplicitFunctionHandle
  : public vtkm::cont::VirtualObjectHandle<vtkm::ImplicitFunction>
{
private:
  using Superclass = vtkm::cont::VirtualObjectHandle<vtkm::ImplicitFunction>;

public:
  ImplicitFunctionHandle() = default;

  template <typename ImplicitFunctionType,
            typename DeviceAdapterList = VTKM_DEFAULT_DEVICE_ADAPTER_LIST_TAG>
  explicit ImplicitFunctionHandle(ImplicitFunctionType* function,
                                  bool acquireOwnership = true,
                                  DeviceAdapterList devices = DeviceAdapterList())
    : Superclass(function, acquireOwnership, devices)
  {
  }
};

template <typename ImplicitFunctionType,
          typename DeviceAdapterList = VTKM_DEFAULT_DEVICE_ADAPTER_LIST_TAG>
VTKM_CONT ImplicitFunctionHandle
make_ImplicitFunctionHandle(ImplicitFunctionType&& func,
                            DeviceAdapterList devices = DeviceAdapterList())
{
  using IFType = typename std::remove_reference<ImplicitFunctionType>::type;
  return ImplicitFunctionHandle(
    new IFType(std::forward<ImplicitFunctionType>(func)), true, devices);
}

template <typename ImplicitFunctionType, typename... Args>
VTKM_CONT ImplicitFunctionHandle make_ImplicitFunctionHandle(Args&&... args)
{
  return ImplicitFunctionHandle(new ImplicitFunctionType(std::forward<Args>(args)...),
                                true,
                                VTKM_DEFAULT_DEVICE_ADAPTER_LIST_TAG());
}

template <typename ImplicitFunctionType, typename DeviceAdapterList, typename... Args>
VTKM_CONT ImplicitFunctionHandle make_ImplicitFunctionHandle(Args&&... args)
{
  return ImplicitFunctionHandle(
    new ImplicitFunctionType(std::forward<Args>(args)...), true, DeviceAdapterList());
}

//============================================================================
/// A helpful wrapper that returns a functor that calls the (virtual) value method of a given
/// ImplicitFunction. Can be passed to things that expect a functor instead of an ImplictFunction
/// class (like an array transform).
///
class VTKM_ALWAYS_EXPORT ImplicitFunctionValueHandle
  : public vtkm::cont::ExecutionAndControlObjectBase
{
  vtkm::cont::ImplicitFunctionHandle Handle;

public:
  ImplicitFunctionValueHandle() = default;

  ImplicitFunctionValueHandle(const ImplicitFunctionHandle& handle)
    : Handle(handle)
  {
  }

  template <typename ImplicitFunctionType,
            typename DeviceAdapterList = VTKM_DEFAULT_DEVICE_ADAPTER_LIST_TAG>
  explicit ImplicitFunctionValueHandle(ImplicitFunctionType* function,
                                       bool acquireOwnership = true,
                                       DeviceAdapterList devices = DeviceAdapterList())
    : Handle(function, acquireOwnership, devices)
  {
  }

  VTKM_CONT const vtkm::cont::ImplicitFunctionHandle& GetHandle() const { return this->Handle; }

  VTKM_CONT
  vtkm::ImplicitFunctionValue PrepareForExecution(vtkm::cont::DeviceAdapterId device) const
  {
    return vtkm::ImplicitFunctionValue(this->Handle.PrepareForExecution(device));
  }

  VTKM_CONT vtkm::ImplicitFunctionValue PrepareForControl() const
  {
    return vtkm::ImplicitFunctionValue(this->Handle.PrepareForControl());
  }
};

template <typename ImplicitFunctionType,
          typename DeviceAdapterList = VTKM_DEFAULT_DEVICE_ADAPTER_LIST_TAG>
VTKM_CONT ImplicitFunctionValueHandle
make_ImplicitFunctionValueHandle(ImplicitFunctionType&& func,
                                 DeviceAdapterList devices = DeviceAdapterList())
{
  using IFType = typename std::remove_reference<ImplicitFunctionType>::type;
  return ImplicitFunctionValueHandle(
    new IFType(std::forward<ImplicitFunctionType>(func)), true, devices);
}

template <typename ImplicitFunctionType, typename... Args>
VTKM_CONT ImplicitFunctionValueHandle make_ImplicitFunctionValueHandle(Args&&... args)
{
  return ImplicitFunctionValueHandle(new ImplicitFunctionType(std::forward<Args>(args)...),
                                     true,
                                     VTKM_DEFAULT_DEVICE_ADAPTER_LIST_TAG());
}

template <typename ImplicitFunctionType, typename DeviceAdapterList, typename... Args>
VTKM_CONT ImplicitFunctionValueHandle make_ImplicitFunctionValueHandle(Args&&... args)
{
  return ImplicitFunctionValueHandle(
    new ImplicitFunctionType(std::forward<Args>(args)...), true, DeviceAdapterList());
}
}
} // vtkm::cont

#endif // vtk_m_cont_ImplicitFunctionHandle_h
