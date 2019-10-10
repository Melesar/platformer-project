#include "Application.h"

#undef main

int main (int argc, char** argv)
{
	Platformer::Application app;
	app.run(argc, argv);
	
	return 0;
}