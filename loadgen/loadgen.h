/* Copyright 2019 The MLPerf Authors. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

/// \file
/// \brief Provides the entry points for a SUT to start a test and respond
/// to issued queries.

#ifndef MLPERF_LOADGEN_LOADGEN_H_
#define MLPERF_LOADGEN_LOADGEN_H_

#include <cstddef>

/// \brief Contains the loadgen API.
namespace mlperf {

struct QuerySampleResponse;
class QuerySampleLibrary;
class SystemUnderTest;
struct TestSettings;
struct LogSettings;

/// \addtogroup LoadgenAPI Loadgen API
/// \brief **Note:** The Loadgen is not aware of the MLPerf specifications
///        nor the model it is running with. For official runs, clients must
///        ensure the TestSettings they use conform to submission rules.
/// \details
/// * Refer to demos, tests, and reference implementations for example API
///   usage.
/// * The general flow of a run:
///   + The client implements the QuerySampleLibrary and SystemUnderTest
///     interfaces.
///   + The client creates a TestSettings instance and calls StartTest.
///   + Then, for each QuerySample included in call to
///     SystemUnderTest::IssueQuery, the client must include it in a call to
///     QuerySampleComplete.
/// * The results are output to various log files, including:
///   + mlperf_log_summary.txt
///   + mlperf_log_detail.txt
///   + mlperf_log_trace.json
///   + mlperf_log_accuracy.json
/// @{

///
/// \brief SUT calls this to notify loadgen of completed samples.
/// \details
/// * The samples may be from any combination of queries or partial queries as
///   issued by \link mlperf::SystemUnderTest::IssueQuery
///   SystemUnderTest::IssueQuery \endlink.
/// * The SUT is responsible for allocating and owning the response data
///   which must remain valid until the duration of this call. The loadgen
///   will copy the response data if needed for e.g. accuracy mode.
///   + Note: This setup requires the allocation to be timed, which
///     will benefit SUTs that efficiently recycle response memory.
/// * All calls to QuerySampleComplete are thread-safe and wait-free bounded.
///   + Any number of threads can call QuerySampleComplete simultaneously.
///   + Regardless of where any other thread stalls, the current thread will
///     finish QuerySampleComplete in a bounded number of cycles.
void QuerySamplesComplete(QuerySampleResponse* responses,
                          size_t response_count);

///
/// \brief Starts the test against SUT with the specified settings.
/// \details This is the C++ entry point. See mlperf::c::StartTest for the
/// C entry point.
///
void StartTest(SystemUnderTest* sut, QuerySampleLibrary* qsl,
               const TestSettings& requested_settings,
               const LogSettings& log_settings);

/// @}

}  // namespace mlperf

#endif  // MLPERF_LOADGEN_LOADGEN_H_
