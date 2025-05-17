# Cotton - a simple 2D game framework

# Example:
```cpp 
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
```
``` 
``` 
```
```
```
```
```
``` 
```
```
```
```
```  
```
```
```
```
```
```
```
``` 
```
```
```
```
# Note - i use this as a baseline for my projects

# How to build:
    mkdir out
    cd out 
    cmake ./cotton
    cmake --build .
    ./Cotton.exe
