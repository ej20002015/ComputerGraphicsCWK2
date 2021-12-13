#include "Application.h"

int main()
{
    Application* app = new Application("Computer Graphics CWK2");
    app->run();
    delete app;

    return 0;
}