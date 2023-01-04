#include "SandBox/SandBoxApp.h"
//#include "SandBox/Layers/ExampleLayer.h"
#include "SandBox/Layers/ExampleLayer2D.h"

namespace MyApp
{
	SandBox::SandBox()
	{
		PushLayer(new ExampleLayer2D());
	}

	SandBox::~SandBox()
	{
	}
}