# Cotton - a simple 2D game framework

# Example:
```cpp 
#include "cotton.h"

int main() {
    IntiWindow(800, 800, "hi");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKY_BLUE);

        DrawCircle(400, 400, 100,WHITE);

        EndDrawing();
    }
}
```
# How to build:
    mkdir out
    cd out 
    cmake ../
    cmake --build .
    ./Cotton.exe
## Note - i use this as a baseline for my projects
