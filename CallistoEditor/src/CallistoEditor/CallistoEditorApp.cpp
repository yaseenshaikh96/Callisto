#include "CallistoEditor/CallistoEditorApp.h"
#include "Layers/EditorLayer.h"

namespace Callisto
{
	CallistoEditorApp::CallistoEditorApp()
		: Application("Editor")
	{
		PushLayer(new EditorLayer());
	}

	CallistoEditorApp::~CallistoEditorApp()
	{
	}
}