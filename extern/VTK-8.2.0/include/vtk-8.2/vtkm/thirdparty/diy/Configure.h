//=============================================================================
//
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2015 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//  Copyright 2015 UT-Battelle, LLC.
//  Copyright 2015 Los Alamos National Security.
//
//  Under the terms of Contract DE-NA0003525 with NTESS,
//  the U.S. Government retains certain rights in this software.
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//
//=============================================================================
#ifndef vtkm_diy_h
#define vtkm_diy_h

#include <vtkm/internal/Configure.h>

// Diy headers include windows.h. Use the following header file so that only the
// lean version of windows.h is included.
// (Without problematic macros like MIN, Max, GetMessage, etc)
#include <vtkm/internal/Windows.h>

/* Use the diy library configured for VTM-m. */
#define VTKM_USE_EXTERNAL_DIY 0

/* Whether to use MPI support in DIY */
#if !defined(VTKM_ENABLE_MPI)
# define DIY_NO_MPI
#endif

/* initially, we disable DIY threads.
 * once we've sorted out how DIY threads and vtkm work together
 * we will make this configurable.*/
#define DIY_NO_THREADS

#if VTKM_USE_EXTERNAL_DIY
# define VTKM_DIY(header) <header>
#else
# define VTKM_DIY(header) <vtkmdiy/include/header>
# define diy vtkmdiy // mangle namespace diy
#endif

#endif
