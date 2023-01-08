#include "SandBox/SandBoxApp.h"
#include "SandBox/Layers/ExampleLayer.h"
#include "SandBox/Layers/ExampleLayer2D.h"
#include "SandBox/Layers/ExampleLayer2DSpriteSheets.h"

namespace MyApp
{
	SandBox::SandBox()
	{
		PushLayer(new ExampleLayer2DSpriteSheets());
	}

	SandBox::~SandBox()
	{
	}
}