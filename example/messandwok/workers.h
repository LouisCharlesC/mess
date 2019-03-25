
#include "workerpool.h"

#include <stack>

#pragma once

using Workers = mess::WorkerPool<std::stack<mess::ExecutableWork>>;