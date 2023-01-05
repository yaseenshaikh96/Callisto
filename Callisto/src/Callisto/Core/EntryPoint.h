#pragma once


#ifdef CALLISTO_PLATFORM_WINDOWS

extern Callisto::Application* Callisto::CreateApplication();

int main(int argc, char** argv)
{
	Callisto::Log::Init();
	CALLISTO_CORE_INFO("Initialized logger");
	CALLISTO_INFO("Initialized logger");

	CALLISTO_PROFILE_BEGIN_SESSION("Startup", "CallistoProfile_Startup.json");
	auto app = Callisto::CreateApplication();
	CALLISTO_PROFILE_END_SESSION();

	CALLISTO_PROFILE_BEGIN_SESSION("Running", "CallistoProfile_Running.json");
	app->Run();
	CALLISTO_PROFILE_END_SESSION();

	CALLISTO_PROFILE_BEGIN_SESSION("Shutdown", "CallistoProfile_Shutdown.json");
	delete app;
	CALLISTO_PROFILE_END_SESSION();

}

#endif // CALLISTO_PLATFORM_WINDOWS
