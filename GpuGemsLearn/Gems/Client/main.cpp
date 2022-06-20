#include "../Engine/Application.h"
#include "App/CustomLayer.h"
int main()
{
	Application app;
	app.PushLayer(new MainLayer());
	app.Run();
	return 0;
}