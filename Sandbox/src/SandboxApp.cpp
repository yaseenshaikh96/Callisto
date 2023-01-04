#include "SandBoxApp.h"
#include "Layers/ExampleLayer.h"

namespace MyApp
{
	SandBox::SandBox()
	{
		PushLayer(new ExampleLayer());
	}

	SandBox::~SandBox()
	{
	}
}