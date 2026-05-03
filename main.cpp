#include "engine/core/Engine.h"
#include <sol/sol.hpp>

using namespace std;

// entry of the engine
int main() {

    if (init() != 0)
        // the init function return non-zero value if engine should be terminated so we send non-zero exit code
        return 1;

    // ok so we are ready to start the engine
    run();

    // when run() function has ended we call quit() to free up and close the engine
    quit();

    // we return 0 if no one has exited already
    return 0;
}