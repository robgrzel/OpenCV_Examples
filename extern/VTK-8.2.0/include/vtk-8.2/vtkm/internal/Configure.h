//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2014 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//  Copyright 2014 UT-Battelle, LLC.
//  Copyright 2014 Los Alamos National Security.
//
//  Under the terms of Contract DE-NA0003525 with NTESS,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================
#ifndef vtk_m_internal_Configure_h
#define vtk_m_internal_Configure_h

#ifdef __CUDACC__
#define VTKM_CUDA
#endif

#if defined(_MSC_VER)
//MSVC 2015+ can use a clang frontend, so we want to label it only as MSVC
//and not MSVC and clang
#define VTKM_MSVC

#elif defined(__INTEL_COMPILER)
//Intel 14+ on OSX uses a clang frontend, so again we want to label them as
//intel only, and not intel and clang
#define VTKM_ICC

#elif defined(__PGI)
// PGI reports as GNUC as it generates the same ABI, so we need to check for
// it before gcc.
#define VTKM_PGI

#elif defined(__clang__)
//Check for clang before GCC, as clang says it is GNUC since it has ABI
//compliance
#define VTKM_CLANG

#elif defined(__GNUC__)
// Several compilers pretend to be GCC but have minor differences. Try to
// compensate for that, by checking for those compilers first
#define VTKM_GCC
#endif


#if defined(unix) || defined(__unix) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
# ifdef _POSIX_VERSION
#  define VTKM_POSIX _POSIX_VERSION
# endif
#endif

#if defined(_WIN32)
# define VTKM_WINDOWS
#endif

#if !defined(VTKM_NO_ASSERT)
#define VTKM_NO_ASSERT
#endif

#if !defined(VTKM_USE_DOUBLE_PRECISION) && !defined(VTKM_NO_DOUBLE_PRECISION)
/* #undef VTKM_USE_DOUBLE_PRECISION */
#endif

#if defined(VTKM_USE_DOUBLE_PRECISION) && defined(VTKM_NO_DOUBLE_PRECISION)
# error Both VTKM_USE_DOUBLE_PRECISION and VTKM_NO_DOUBLE_PRECISION defined.  Do not know what to do.
#endif

#if !defined(VTKM_USE_64BIT_IDS) && !defined(VTKM_NO_64BIT_IDS)
#define VTKM_USE_64BIT_IDS
#endif

#if defined(VTKM_USE_64BIT_IDS) && defined(VTKM_NO_64BIT_IDS)
# error Both VTKM_USE_64BIT_IDS and VTKM_NO_64BIT_IDS defined.  Do not know what to do.
#endif

#define VTKM_SIZE_LONG 8
#define VTKM_SIZE_LONG_LONG 8

// Defines the cache line size in bytes to align allocations to
#ifndef VTKM_ALLOCATION_ALIGNMENT
#define VTKM_ALLOCATION_ALIGNMENT 64
#endif

// Define a pair of macros, VTKM_THIRDPARTY_PRE_INCLUDE and VTKM_THIRDPARTY_POST_INCLUDE,
// that should be wrapped around any #include for a tbb or thrust header file. Mostly
// this is used to set pragmas that dissable warnings that VTK-m checks for
// but tbb and thrust does not.
#if (defined(VTKM_GCC) || defined(VTKM_CLANG))

#define VTK_M_THIRDPARTY_GENERAL_WARNING_PRAGMAS \
  _Pragma("GCC diagnostic ignored \"-Wconversion\"") \
  _Pragma("GCC diagnostic ignored \"-Wshadow\"") \
  _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")

// GCC has a unused by set variable warnings that needs to be silenced.
#if defined(VTKM_GCC)
#define VTK_M_THIRDPARTY_GCC_WARNING_PRAGMAS \
  _Pragma("GCC diagnostic ignored \"-Wunused-but-set-variable\"") \
  _Pragma("GCC diagnostic ignored \"-Wunused-variable\"")
#else
#define VTK_M_THIRDPARTY_GCC_WARNING_PRAGMAS
#endif

// Newer versions of clang have an unused-local-typedef warning, but not older
// versions. This checks for the apple version of clang, which is different
// than other clang compiled versions. If using a non-apple version of clang,
// you might need to extend this condition.
#if defined(VTKM_CLANG) && (__apple_build_version__ >= 7000072)
#define VTK_M_THIRDPARTY_CLANG_WARNING_PRAGMAS \
  _Pragma("GCC diagnostic ignored \"-Wunused-local-typedef\"")
#else
#define VTK_M_THIRDPARTY_CLANG_WARNING_PRAGMAS
#endif

// Older versions of GCC don't support the push/pop pragmas. Right now we are
// not checking for GCC 3 or earlier. I'm not sure we have a use case for that.
#if defined(VTKM_GCC) && (__GNUC__ == 4 && __GNUC_MINOR__ < 6)
#define VTK_M_THIRDPARTY_WARNINGS_PUSH
#define VTK_M_THRIDPARTY_WARNINGS_POP
#else
#define VTK_M_THIRDPARTY_WARNINGS_PUSH _Pragma("GCC diagnostic push")
#define VTK_M_THRIDPARTY_WARNINGS_POP  _Pragma("GCC diagnostic pop")
#endif

#define VTKM_THIRDPARTY_PRE_INCLUDE \
  VTK_M_THIRDPARTY_WARNINGS_PUSH \
  VTK_M_THIRDPARTY_GENERAL_WARNING_PRAGMAS \
  VTK_M_THIRDPARTY_GCC_WARNING_PRAGMAS \
  VTK_M_THIRDPARTY_CLANG_WARNING_PRAGMAS
#define VTKM_THIRDPARTY_POST_INCLUDE \
  VTK_M_THRIDPARTY_WARNINGS_POP

#elif (defined(VTKM_MSVC))
#define VTKM_THIRDPARTY_PRE_INCLUDE \
  __pragma(warning(push)) \
  __pragma(warning(disable:4100)) \
  __pragma(warning(disable:4127)) \
  __pragma(warning(disable:4201)) \
  __pragma(warning(disable:4244)) \
  __pragma(warning(disable:4267)) \
  __pragma(warning(disable:4324)) \
  __pragma(warning(disable:4510)) \
  __pragma(warning(disable:4512)) \
  __pragma(warning(disable:4515)) \
  __pragma(warning(disable:4610)) \
  __pragma(warning(disable:4800))
#define VTKM_THIRDPARTY_POST_INCLUDE \
  __pragma(warning(pop))
#else
#define VTKM_THIRDPARTY_PRE_INCLUDE
#define VTKM_THIRDPARTY_POST_INCLUDE
#endif

//Determine if current compiler supports vectorization pragma's
//if so set the define VTKM_COMPILER_SUPPORTS_VECTOR_PRAGMAS
//
//While ICC 14 does support simd pragma, in testing we find that the
//simd code produces SIGBUS and isn't safe to use.
//
#if ( defined(VTKM_GCC) && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)  ) ) || \
    ( defined(VTKM_ICC) && (__INTEL_COMPILER >= 1500) ) || \
    ( defined(VTKM_CLANG) && defined(__apple_build_version__) && (__apple_build_version__ >= 7000000) ) || \
    ( defined(VTKM_CLANG) && !defined(__apple_build_version__) && (__clang_major__ > 3) ) || \
    ( defined(VTKM_CLANG) && !defined(__apple_build_version__) && (__clang_major__ == 3 && __clang_minor__ >= 5) )
#define VTKM_COMPILER_SUPPORTS_VECTOR_PRAGMAS 1
#endif


// Define a pair of macros, VTKM_VECTORIZATION_PRE_LOOP and VTKM_VECTORIZATION_IN_LOOP,
// that should be wrapped around any "for"/"while" that you want vectorized.
// This is used to set per compiler pragmas for vectorization, and to disable
// any warnings that about vectorization failures.
#define VTKM_VECTORIZATION_ENABLED

#if defined(VTKM_COMPILER_SUPPORTS_VECTOR_PRAGMAS) &&\
    defined(VTKM_VECTORIZATION_ENABLED)
#if defined(VTKM_CLANG)
//clang only needs pre loop
#define VTKM_VECTORIZATION_PRE_LOOP \
  _Pragma("clang loop vectorize(enable) interleave(enable)")
#define VTKM_VECTORIZATION_IN_LOOP
#elif defined(VTKM_ICC) && defined(NDEBUG)
//Note: icc can't do vectorization in debug builds
//icc needs pre and in loop
//For ICC we want ivdep over simd for the following reason:
//#pragma simd is a more powerful combo of '#pragma vector always' and '#pragma ivdep'
//The compiler does not check for aliasing or dependencies that might cause
//incorrect results after vectorization, and it does not protect against illegal
//memory references. #pragma ivdep overrides potential dependencies, but the
//compiler still performs a dependency analysis, and will not vectorize if it
//finds a proven dependency that would affect results. With #pragma simd, the
//compiler does no such analysis, and tries to vectorize regardless.
//
//Final: We are currently disabling all vectorization with ICC
// in the short-term. Both ivdep and simd cause a horrible decrease in compile
// time, and generates bad vectorization code.
#define VTKM_VECTORIZATION_PRE_LOOP
#define VTKM_VECTORIZATION_IN_LOOP
#elif defined(VTKM_GCC)
//gcc only needs in loop
#define VTKM_VECTORIZATION_PRE_LOOP \
  _Pragma("GCC ivdep")
#define VTKM_VECTORIZATION_IN_LOOP
#else
// Compiler is unknown so we don't define any vectortization pragmas
#define VTKM_VECTORIZATION_PRE_LOOP
#define VTKM_VECTORIZATION_IN_LOOP
#endif
#else
// Compiler doesn't support any vectortization pragmas
#define VTKM_VECTORIZATION_PRE_LOOP
#define VTKM_VECTORIZATION_IN_LOOP
#endif

//Mark if we are building with CUDA enabled
#ifndef VTKM_ENABLE_CUDA
/* #undef VTKM_ENABLE_CUDA */
#endif
//Mark if we are building with TBB enabled
#ifndef VTKM_ENABLE_TBB
#define VTKM_ENABLE_TBB
#endif
//Mark if we are building with OpenMP enabled
#ifndef VTKM_ENABLE_OPENMP
#define VTKM_ENABLE_OPENMP
#endif

//Mark if we are building with MPI enabled.
#define VTKM_ENABLE_MPI

//Mark what version of the CUDA compiler we have. This is needed to correctly
//choose consistent implementation ( so we don't violate ODR ) when we compile
//with CUDA 7.5
#ifdef VTKM_ENABLE_CUDA
#define VTKM_CUDA_VERSION_MAJOR 
#define VTKM_CUDA_VERSION_MINOR 
#endif

#if __cplusplus >= 201103L || \
    ( defined(VTKM_MSVC) && _MSC_VER >= 1900  ) || \
    ( defined(VTKM_ICC)  && defined(__INTEL_CXX11_MODE__) )
#define VTKM_HAVE_CXX_11
#else
#error "VTK-m requires at least a C++11 compiler"
#endif

//Mark if we have enabled logging.
#define VTKM_ENABLE_LOGGING


// Define a pair of macros, VTKM_SWALLOW_SEMICOLON_PRE_BLOCK and
// VTKM_SWALLOW_SEMICOLON_POST_BLOCK that can be used around a block in a
// macro to "swallow" the semicolon after the macro's use so that it is
// used like a function without warnings about extra semicolons. It is
// generally implemented by wrapping the block as a do/while with a false
// condition so that it executes exactly once. (See, for example,
// https://gcc.gnu.org/onlinedocs/cpp/Swallowing-the-Semicolon.html)
// However, some compilers might complain about this, too. For example, you
// might get a complaint about a loop with a constant expression. These
// macros take care of these conditions.
#ifdef VTKM_MSVC
#define VTKM_SWALLOW_SEMICOLON_PRE_BLOCK \
  __pragma(warning(push)) \
  __pragma(warning(disable:4127)) \
  do
#define VTKM_SWALLOW_SEMICOLON_POST_BLOCK \
  while (false) \
  __pragma(warning(pop))
#else // not VTKM_MSVC
#define VTKM_SWALLOW_SEMICOLON_PRE_BLOCK \
  do
#define VTKM_SWALLOW_SEMICOLON_POST_BLOCK \
  while (false)
#endif

// Use a trick to pass arguments containing commas through a macro. This is
// helpful for mixing template code with macros.
// See https://stackoverflow.com/questions/13842468
#define VTKM_PASS_COMMAS(...) __VA_ARGS__

#ifdef VTKM_MSVC
//With MSVC the types that we generate cause warning C4503 (long symbol names)
//this doesn't affect the resulting binary so we just suppress that warning
//
#pragma warning(disable:4503)

//MSVC has a warning for using "unsafe" algorithms such as fill_n that can
//easily overrun the end of unchecked pointers. The problem is that there
//are lots of valid uses of these algorithms, and the signal that this is
//a valid use is MSVC-specific. Even less fortunate is the fact that if
//another header violates the warning, it is impossible to suppress it
//because it happens in a system header file (xutility), which was included
//far before the actual offending code occurred. Even less fortunate than
//that, a boost header we (indirectly) use sets off this warning, cannot
//be suppressed, and is not going to be fixed
//(https://svn.boost.org/trac/boost/ticket/11426). The best solution is to
//turn off this problematic warning for everybody.
//
#pragma warning(disable:4996)

//In VTK-m libraries, we are typically exporting (with declspec) classes rather
//than the independent methods within the class. When you are exporting to a
//dll in MSVC and the class contains a field that is not similarly exported,
//then you get a warning. This is generally not an issue for our use since we
//are really only using classes from the standard library and distributed with
//VTK-m itself. The only sane way to fix the issue is to just disable the
//warning.
//
#pragma warning(disable:4251)

#endif

//----------------------------------------------------------------------------
// Switch case fall-through policy.

// Use "VTKM_FALLTHROUGH;" to annotate deliberate fall-through in switches,
// use it analogously to "break;".  The trailing semi-colon is required.
#if !defined(VTKM_FALLTHROUGH) && defined(__has_cpp_attribute)
# if __cplusplus >= 201703L && __has_cpp_attribute(fallthrough)
#  define VTKM_FALLTHROUGH [[fallthrough]]
# elif __cplusplus >= 201103L && __has_cpp_attribute(gnu::fallthrough)
#  define VTKM_FALLTHROUGH [[gnu::fallthrough]]
# elif __cplusplus >= 201103L && __has_cpp_attribute(clang::fallthrough)
#  define VTKM_FALLTHROUGH [[clang::fallthrough]]
# endif
#endif

#ifndef VTKM_FALLTHROUGH
# define VTKM_FALLTHROUGH ((void)0)
#endif

#endif //vtkm_internal_Configure_h
