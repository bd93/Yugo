#include "pch.h"
#include "Yugo.h"


class Engine : public Yugo::Application
{
	
};


int main()
{
	Engine* engine = new Engine();
	engine->Run();
	delete engine;
}