#include "CallistoPCH.h"
#include "Layer.h"

namespace Callisto {
	Layer::Layer(const std::string& debugName)
		:
		m_DebugName(debugName)
	{}
	Layer::~Layer()
	{}
}