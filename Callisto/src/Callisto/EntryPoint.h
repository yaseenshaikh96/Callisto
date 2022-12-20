#pragma once

#ifdef CALLISTO_PLATFORM_WINDOWS

extern Callisto::Application* Callisto::CreateApplication();

int main(int argc, char** argv)
{
	auto app{ Callisto::CreateApplication() };
	app->Run();
	delete app;
}

#endif // CALLISTO_PLATFORM_WINDOWS
