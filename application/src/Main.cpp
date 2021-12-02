#include "Application.h"

int main()
{
    Application* app = Application::Create();
    app->run();
    delete app;

    return 0;
}