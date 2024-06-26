#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Callisto/Core/Log.h"

#ifdef CALLISTO_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // CALLISTO_PLATFORM_WINDOWS

/* Debug */
#include <Callisto/DebugUtils/Instrumental.h>