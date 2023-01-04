#include "SandBoxApp.h"
#include <Callisto/Core/EntryPoint.h>

Callisto::Application* Callisto::CreateApplication()
{
	return new MyApp::SandBox();
}