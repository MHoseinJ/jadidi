#include <fstream>
#include "engine/core/Engine.h"

using namespace std;

int main() {
    if (init() != 0) return 1;

    run();

    quit();

    return 0;
}
