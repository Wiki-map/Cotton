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
