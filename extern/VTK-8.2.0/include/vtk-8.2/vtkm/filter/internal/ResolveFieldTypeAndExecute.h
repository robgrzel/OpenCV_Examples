//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2016 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//  Copyright 2016 UT-Battelle, LLC.
//  Copyright 2016 Los Alamos National Security.
//
//  Under the terms of Contract DE-NA0003525 with NTESS,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================
#ifndef vtk_m_filter_internal_ResolveFieldTypeAndExecute_h
#define vtk_m_filter_internal_ResolveFieldTypeAndExecute_h

#include <vtkm/cont/DataSet.h>
#include <vtkm/cont/RuntimeDeviceTracker.h>
#include <vtkm/cont/TryExecute.h>
#include <vtkm/cont/internal/DeviceAdapterTag.h>

#include <vtkm/filter/FieldMetadata.h>
#include <vtkm/filter/PolicyBase.h>

namespace vtkm
{
namespace filter
{
namespace internal
{

struct ResolveFieldTypeAndExecute
{
  template <typename T,
            typename Storage,
            typename DerivedClass,
            typename DerivedPolicy,
            typename ResultType>
  void operator()(const vtkm::cont::ArrayHandle<T, Storage>& field,
                  DerivedClass* derivedClass,
                  const vtkm::cont::DataSet& inputData,
                  const vtkm::filter::FieldMetadata& fieldMeta,
                  vtkm::filter::PolicyBase<DerivedPolicy> policy,
                  ResultType& result)
  {
    result = derivedClass->DoExecute(inputData, field, fieldMeta, policy);
  }
};
}
}
} // namespace vtkm::filter::internal

#endif //vtk_m_filter_internal_ResolveFieldTypeAndExecute_h
