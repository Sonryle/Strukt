#pragma once
#include <Strukt/window.h>

struct strukt_instance {
    struct strukt_window window;
};

void struktReturnDefaultInstance(struct strukt_instance* instance);
void struktRun(struct strukt_instance* instance);

// OnUpdate
// OnFixedUpdate
// OnRender
