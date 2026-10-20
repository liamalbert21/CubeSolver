#include "Application.h"

Application Application_init()
{
    Application app;

    app.state = FSM_INIT;
    app.hal = HAL_init();

    return app;
}