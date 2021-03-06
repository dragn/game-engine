#pragma once

#ifdef WITH_GLOG
#include <glog/logging.h>
#else
#define LOG(X) std::cout << std::endl << #X << ": "
#define CHECK(X) assert(X)
#endif
