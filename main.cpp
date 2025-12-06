#include <iostream>
#include "engine/core/Engine.h"

using namespace std;

int main() {

    if (init() != 0) return 1;

    if (run(renderer) != 0) return 2;

    quit();

    return 0;
}