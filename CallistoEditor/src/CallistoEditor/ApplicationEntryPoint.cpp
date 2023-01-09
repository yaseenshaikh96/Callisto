#include "CallistoEditor/CallistoEditorApp.h"
#include <Callisto/Core/EntryPoint.h>

namespace Callisto
{
	Application* CreateApplication()
	{
		return new CallistoEditorApp();
	}
}