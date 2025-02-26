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
#ifndef vtk_m_cont_Logging_h
#define vtk_m_cont_Logging_h

#include <vtkm/internal/Configure.h>
#include <vtkm/internal/ExportMacros.h>

#include <vtkm/Types.h>

#include <vtkm/cont/vtkm_cont_export.h>

#ifdef VTKM_ENABLE_LOGGING

// disable MSVC warnings in loguru.hpp
#ifdef VTKM_MSVC
#pragma warning(push)
#pragma warning(disable : 4722)
#endif // VTKM_MSVC

#define LOGURU_EXPORT VTKM_CONT_EXPORT
#define LOGURU_WITH_STREAMS 1
#define LOGURU_SCOPE_TIME_PRECISION 6
#include <vtkm/thirdparty/loguru/vtkmloguru/loguru.hpp>

#ifdef VTKM_MSVC
#pragma warning(pop)
#endif // VTKM_MSVC

#else // VTKM_ENABLE_LOGGING
#include <iostream>
#endif // VTKM_ENABLE_LOGGING

#include <string>
#include <typeinfo>

/// \file Logging.h
/// \brief Logging utilities.
///
/// This file includes the logging system for VTK-m. There are a variety of
/// macros to print log messages using C++ stream or printf syntax. Nested
/// scopes may be created in the log output, and there are several helper
/// functions to help format common types of log data such as byte counts and
/// type names.
///
/// Logging is enabled by setting the CMake variable VTKm_ENABLE_LOGGING. When
/// this flag is enabled, any messages logged to the Info, Warn, Error, and
/// Fatal levels are printed to stderr by default.
///
/// Additional logging features are enabled by calling vtkm::cont::InitLogging
/// in an executable. This will:
/// - Set human-readable names for the log levels in the output.
/// - Allow the stderr logging level to be set at runtime by passing a
///   '-v [level]' argument to the executable.
/// - Name the main thread.
/// - Print a preamble with details of the program's startup (args, etc).
/// - Install signal handlers to automatically print stacktraces and error
///   contexts (linux only) on crashes.
///
/// The main logging entry points are the macros VTKM_LOG_S and VTKM_LOG_F,
/// which using C++ stream and printf syntax, repectively. Other variants exist,
/// including conditional logging and special-purpose logs for writing specific
/// events, such as DynamicObject cast results and TryExecute failures.
///
/// The logging backend supports the concept of "Scopes". By creating a new
/// scope with the macros VTKM_LOG_SCOPE or VTKM_LOG_SCOPE_FUNCTION, a new
/// "logging scope" is opened within the C++ scope the macro is called from. New
/// messages will be indented in the log until the scope ends, at which point
/// a message is logged with the elapsed time that the scope was active. Scopes
/// may be nested to arbitrary depths.
///
/// The logging implementation is thread-safe. When working in a multithreaded
/// environment, each thread may be assigned a human-readable name using
/// vtkm::cont::SetThreadName. This will appear in the log output so that
/// per-thread messages can be easily tracked.
///
/// By default, only Info, Warn, Error, and Fatal messages are printed to
/// stderr. This can be changed at runtime by passing the '-v' flag to an
/// executable that calls vtkm::cont::InitLogging. Alternatively, the
/// application can explicitly call vtkm::cont::SetStderrLogLevel to change the
/// verbosity. When specifying a verbosity, all log levels with enum values
/// less-than-or-equal-to the requested level are printed.
/// vtkm::cont::LogLevel::Off (or "-v Off") may be used to silence the log
/// completely.
///
/// The helper functions vtkm::cont::GetHumanReadableSize and
/// vtkm::cont::GetSizeString assist in formating byte sizes to a more readable
/// format. Similarly, the vtkm::cont::TypeName template functions provide RTTI
/// based type-name information. When logging is enabled, these use the logging
/// backend to demangle symbol names on supported platforms.
///
/// The more verbose VTK-m log levels are:
/// - Perf: Logs performance information, using the scopes feature to track
///   execution time of filters, worklets, and device algorithms with
///   microsecond resolution.
/// - MemCont / MemExec: These levels log memory allocations in the control and
///   execution environments, respectively.
/// - MemTransfer: This level logs memory transfers between the control and host
///   environments.
/// - Cast: Logs details of dynamic object resolution.
///
/// The log may be shared and extended by applications that use VTK-m. There
/// are two log level ranges left available for applications: User and
/// UserVerbose. The User levels may be enabled without showing any of the
/// verbose VTK-m levels, while UserVerbose levels will also enable all VTK-m
/// levels.

/// \def VTKM_LOG_S(level, ...)
/// \brief Writes a message using stream syntax to the indicated log \a level.
///
/// The ellipsis may be replaced with the log message as if constructing a C++
/// stream, e.g:
///
/// \code
/// VTKM_LOG_S(vtkm::cont::LogLevel::Perf,
///            "Executed functor " << vtkm::cont::TypeName(functor)
///             << " on device " << deviceId.GetName());
/// \endcode

/// \def VTKM_LOG_F(level, ...)
/// \brief Writes a message using printf syntax to the indicated log \a level.
///
/// The ellipsis may be replaced with the log message as if constructing a
/// printf call, e.g:
///
/// \code
/// VTKM_LOG_F(vtkm::cont::LogLevel::Perf,
///            "Executed functor %s on device %s",
///            vtkm::cont::TypeName(functor).c_str(),
///            deviceId.GetName().c_str());
/// \endcode

/// \def VTKM_LOG_IF_S(level, cond, ...)
/// Same as VTKM_LOG_S, but only logs if \a cond is true.

/// \def VTKM_LOG_IF_F(level, cond, ...)
/// Same as VTKM_LOG_F, but only logs if \a cond is true.

/// \def VTKM_LOG_SCOPE(level, ...)
/// Creates a new scope at the requested \a level. The log scope ends when the
/// code scope ends. The ellipses form the scope name using printf syntax.
///
/// \code
/// {
///   VTKM_LOG_SCOPE(vtkm::cont::LogLevel::Perf,
///                  "Executing filter %s",
///                  vtkm::cont::TypeName(myFilter).c_str());
///   myFilter.Execute();
/// }
/// \endcode

/// \def VTKM_LOG_SCOPE_FUNCTION(level)
/// Equivalent to VTKM_LOG_SCOPE(__func__);

/// \def VTKM_LOG_CAST_SUCC(inObj, outObj)
/// \brief Convenience macro for logging the successful cast of dynamic object.
/// \param inObj The dynamic object.
/// \param outObj The resulting downcasted object.

/// \def VTKM_LOG_CAST_FAIL(inObj, outType)
/// \brief Convenience macro for logging a failed cast of dynamic object.
/// \param inObj The dynamic object.
/// \param outObj The candidate type (or typelist) that was unsuccessful.

/// \def VTKM_LOG_TRYEXECUTE_FAIL(errorMessage, functorName, deviceId)
/// \brief Convenience macro for logging a TryExecute failure to the Error level.
/// If logging is disabled, a message is still printed to stderr.
/// \param errorMessage The error message detailing the failure.
/// \param functorName The name of the functor (see vtkm::cont::TypeName)
/// \param deviceId The device tag / id for the device on which the functor
/// failed.

/// \def VTKM_LOG_TRYEXECUTE_DISABLE(errorMessage, functorName, deviceId)
/// \brief Similar to VTKM_LOG_TRYEXECUTE_FAIL, but also informs the user
/// that the device has been disable for future TryExecute calls.
/// \param errorMessage The error message detailing the failure.
/// \param functorName The name of the functor (see vtkm::cont::TypeName)
/// \param deviceId The device tag / id for the device on which the functor
/// failed.

#if defined(VTKM_ENABLE_LOGGING)

#define VTKM_LOG_S(level, ...) VLOG_S(static_cast<loguru::Verbosity>(level)) << __VA_ARGS__
#define VTKM_LOG_F(level, ...) VLOG_F(static_cast<loguru::Verbosity>(level), __VA_ARGS__)
#define VTKM_LOG_IF_S(level, cond, ...)                                                            \
  VLOG_IF_S(static_cast<loguru::Verbosity>(level), cond) << __VA_ARGS__
#define VTKM_LOG_IF_F(level, cond, ...)                                                            \
  VLOG_IF_F(static_cast<loguru::Verbosity>(level), cond, __VA_ARGS__)
#define VTKM_LOG_SCOPE(level, ...) VLOG_SCOPE_F(static_cast<loguru::Verbosity>(level), __VA_ARGS__)
#define VTKM_LOG_SCOPE_FUNCTION(level)                                                             \
  VTKM_LOG_SCOPE(static_cast<loguru::Verbosity>(level), __func__)
#define VTKM_LOG_ERROR_CONTEXT(desc, data) ERROR_CONTEXT(desc, data)

// Convenience macros:
// Cast success:
#define VTKM_LOG_CAST_SUCC(inObj, outObj)                                                          \
  VTKM_LOG_F(vtkm::cont::LogLevel::Cast,                                                           \
             "Cast succeeded: %s (%p) --> %s (%p)",                                                \
             vtkm::cont::TypeName(inObj).c_str(),                                                  \
             &inObj,                                                                               \
             vtkm::cont::TypeName(outObj).c_str(),                                                 \
             &outObj)

// Cast failure:
#define VTKM_LOG_CAST_FAIL(inObj, outType)                                                         \
  VTKM_LOG_F(vtkm::cont::LogLevel::Cast,                                                           \
             "Cast failed: %s (%p) --> %s",                                                        \
             vtkm::cont::TypeName(inObj).c_str(),                                                  \
             &inObj,                                                                               \
             vtkm::cont::TypeName<outType>().c_str())

// TryExecute failure
#define VTKM_LOG_TRYEXECUTE_FAIL(errorMessage, functorName, deviceId)                              \
  VTKM_LOG_S(vtkm::cont::LogLevel::Error, "TryExecute encountered an error: " << errorMessage);    \
  VTKM_LOG_S(vtkm::cont::LogLevel::Error, "Failing functor: " << functorName);                     \
  VTKM_LOG_S(vtkm::cont::LogLevel::Error, "Failing device: " << deviceId.GetName())

// Same, but disabling device:
#define VTKM_LOG_TRYEXECUTE_DISABLE(errorMessage, functorName, deviceId)                           \
  VTKM_LOG_S(vtkm::cont::LogLevel::Error, "TryExecute encountered an error: " << errorMessage);    \
  VTKM_LOG_S(vtkm::cont::LogLevel::Error, "Failing functor: " << functorName);                     \
  VTKM_LOG_S(vtkm::cont::LogLevel::Error, "Failing device: " << deviceId.GetName());               \
  VTKM_LOG_S(vtkm::cont::LogLevel::Error, "The failing device has been disabled.")

#else // VTKM_ENABLE_LOGGING

#define VTKM_LOG_S(level, ...)
#define VTKM_LOG_F(level, ...)
#define VTKM_LOG_IF_S(level, cond, ...)
#define VTKM_LOG_IF_F(level, cond, ...)
#define VTKM_LOG_SCOPE(level, ...)
#define VTKM_LOG_SCOPE_FUNCTION(level)
#define VTKM_LOG_ERROR_CONTEXT(desc, data)
#define VTKM_LOG_CAST_SUCC(inObj, outObj)
#define VTKM_LOG_CAST_FAIL(inObj, outType)

// TryExecute failures are still important enough to log, but we just write to
// std::cerr when logging is disabled.
#define VTKM_LOG_TRYEXECUTE_FAIL(errorMessage, functorName, deviceId)                              \
  std::cerr << "Error: TryExecute encountered an error: " << errorMessage << "\n"                  \
            << "\t- Failing functor: " << functorName << "\n"                                      \
            << "\t- Failing device: " << deviceId.GetName() << "\n\n"
#define VTKM_LOG_TRYEXECUTE_DISABLE(errorMessage, functorName, deviceId)                           \
  std::cerr << "Error: TryExecute encountered an error: " << errorMessage << "\n"                  \
            << "\t- Failing functor: " << functorName << "\n"                                      \
            << "\t- Failing device: " << deviceId.GetName() << "\n"                                \
            << "The failing device has been disabled.\n\n"

#endif // VTKM_ENABLE_LOGGING

namespace vtkm
{
namespace cont
{

/// Log levels for use with the logging macros.
enum class LogLevel
{
  /// Used with SetStderrLogLevel to silence the log. Do not actually log to
  /// this level.
  Off = -9, //loguru::Verbosity_OFF,

  /// Fatal errors that should abort execution.
  Fatal = -3, // loguru::Verbosity_FATAL,

  /// Important but non-fatal errors, such as device fail-over.
  Error = -2, // loguru::Verbosity_ERROR,

  /// Less important user errors, such as out-of-bounds parameters.
  Warn = -1, // loguru::Verbosity_WARNING,

  /// Information messages (detected hardware, etc) and temporary debugging
  /// output.
  Info = 0, //loguru::Verbosity_INFO,

  /// The range 1-255 are reserved to application use.
  UserFirst = 1,
  /// The range 1-255 are reserved to application use.
  UserLast = 255,

  /// General timing data and algorithm flow information, such as filter
  /// execution, worklet dispatches, and device algorithm calls.
  Perf,

  /// Host-side resource allocations/frees (e.g. ArrayHandle control buffers)
  MemCont,

  /// Device-side resource allocations/frees (e.g ArrayHandle device buffers)
  MemExec,

  /// Host->device / device->host data copies
  MemTransfer,

  /// When a dynamic object is (or isn't) resolved via CastAndCall, etc.
  Cast,

  /// 1024-2047 are reserved for application usage.
  UserVerboseFirst = 1024,
  /// 1024-2047 are reserved for application usage.
  UserVerboseLast = 2047
};

/**
 * Initializes logging. Sets up custom log level and thread names. Parses any
 * "-v [LogLevel]" arguments to set the stderr log level. This argument may
 * be either numeric, or the 4-character string printed in the output.
 *
 * If the parameterless overload is used, the `-v` parsing is not used, but
 * other functionality should still work.
 *
 * @note This function is not threadsafe and should only be called from a single
 * thread (ideally the main thread).
 * @{
 */
VTKM_CONT_EXPORT
VTKM_CONT
void InitLogging(int& argc, char* argv[]);
VTKM_CONT_EXPORT
VTKM_CONT
void InitLogging();
/**@}*/

/**
 * Set the range of log levels that will be printed to stderr. All levels
 * with an enum value less-than-or-equal-to \a level will be printed.
 */
VTKM_CONT_EXPORT
VTKM_CONT
void SetStderrLogLevel(vtkm::cont::LogLevel level);

/**
 * Register a custom name to identify a log level. The name will be truncated
 * to 4 characters internally.
 *
 * Must not be called after InitLogging. Such calls will fail and log an error.
 *
 * There is no need to call this for the default vtkm::cont::LogLevels. They
 * are populated in InitLogging and will be overwritten.
 */
VTKM_CONT_EXPORT
VTKM_CONT
void SetLogLevelName(vtkm::cont::LogLevel level, const std::string& name);

/**
 * The name to identify the current thread in the log output.
 * @{
 */
VTKM_CONT_EXPORT
VTKM_CONT
void SetLogThreadName(const std::string& name);
VTKM_CONT_EXPORT
VTKM_CONT
std::string GetLogThreadName();
/**@}*/

// Per-thread error context, not currently used, undocumented....
VTKM_CONT_EXPORT
VTKM_CONT
std::string GetLogErrorContext();

/**
 * Returns a stacktrace on supported platforms.
 * Argument is the number of frames to skip (GetStackTrace and below are already
 * skipped).
 */
VTKM_CONT_EXPORT
VTKM_CONT
std::string GetStackTrace(vtkm::Int32 skip = 0);

/// Convert a size in bytes to a human readable string (e.g. "64 bytes",
/// "1.44 MiB", "128 GiB", etc). @a prec controls the fixed point precision
/// of the stringified number.
VTKM_CONT_EXPORT
VTKM_CONT
std::string GetHumanReadableSize(vtkm::UInt64 bytes, int prec = 2);

/// Returns "%1 (%2 bytes)" where %1 is the result from GetHumanReadableSize
/// and two is the exact number of bytes.
VTKM_CONT_EXPORT
VTKM_CONT
std::string GetSizeString(vtkm::UInt64 bytes, int prec = 2);

/**
 * Use RTTI information to retrieve the name of the type T. If logging is
 * enabled and the platform supports it, the type name will also be demangled.
 * @{
 */
template <typename T>
static inline VTKM_CONT std::string TypeName()
{
#ifdef VTKM_ENABLE_LOGGING
  return loguru::demangle(typeid(T).name()).c_str();
#else  // VTKM_ENABLE_LOGGING
  return typeid(T).name();
#endif // VTKM_ENABLE_LOGGING
}

template <typename T>
static inline VTKM_CONT std::string TypeName(const T&)
{
  return TypeName<T>();
}
/**@}*/
}
} // end namespace vtkm::cont

#endif // vtk_m_cont_Logging_h
