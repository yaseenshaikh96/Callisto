#include <Callisto.h>

class Sandbox : public Callisto::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{
	}
};


Callisto::Application* Callisto::CreateApplication()
{
	return new Sandbox{};
}