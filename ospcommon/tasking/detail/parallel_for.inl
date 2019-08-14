// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include <utility>

#ifdef OSPCOMMON_TASKING_TBB
#  include <tbb/parallel_for.h>
#elif defined(OSPCOMMON_TASKING_INTERNAL)
#  include "TaskSys.h"
#endif

namespace ospcommon {
  namespace tasking {
    namespace detail {

      template<typename INDEX_T, typename TASK_T>
      inline void parallel_for_impl(INDEX_T nTasks, TASK_T&& fcn)
      {
#ifdef OSPCOMMON_TASKING_TBB
        tbb::parallel_for(INDEX_T(0), nTasks, std::forward<TASK_T>(fcn));
#elif defined(OSPCOMMON_TASKING_OMP)
#       pragma omp parallel for schedule(dynamic)
        for (INDEX_T taskIndex = 0; taskIndex < nTasks; ++taskIndex) {
          fcn(taskIndex);
        }
#elif defined(OSPCOMMON_TASKING_INTERNAL)
        detail::parallel_for_internal(nTasks, std::forward<TASK_T>(fcn));
#else // Debug (no tasking system)
        for (INDEX_T taskIndex = 0; taskIndex < nTasks; ++taskIndex) {
          fcn(taskIndex);
        }
#endif
      }

    } // ::ospcommon::tasking::detail
  } // ::ospcommon::tasking
} // ::ospcommon
