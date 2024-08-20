#include "BennettPCH.h"
#include "StaticProp.h"

namespace Bennett
{
	StaticProp::StaticProp(const std::string& name, const Transform& transform)
		: BProp(name, transform)
	{
	}

	StaticProp::~StaticProp()
	{
	}
}