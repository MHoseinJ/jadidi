#include "engine/core/Engine.h"
#include <sol/sol.hpp>

using namespace std;

int main() {
    if (init() != 0)
        return 1;

    run();

    quit();

    return 0;
}
