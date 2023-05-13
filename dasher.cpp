#include "raylib.h"

int main()
{
    const int windowWidth = 800;
    const int windowHeight = 450;
    InitWindow(windowWidth, windowHeight, "Dasher");

    const int jumpVal{-20};

    const int gravity{1};
    Texture2D scarfy = LoadTexture("textures/scarfy.png");

    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width / 6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;

    Vector2 scarfyPos;
    scarfyPos.x = windowWidth / 2 - scarfyRec.width / 2;
    scarfyPos.y = windowHeight - scarfyRec.height;

    int velocity{0};

    SetTargetFPS(60);

    bool isJumping{false};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);


        if (scarfyPos.y >= windowHeight - scarfyRec.height)
        {
            // rectangle is on the ground
            velocity = 0;
            isJumping = false;
        }
        else
        {
            // rectangle is in the air
            velocity += gravity;
            isJumping = true;
        }

        // check for jumping
        if (IsKeyPressed(KEY_SPACE) && !isJumping)
        {
            velocity += jumpVal;
        }

        scarfyPos.y += velocity;

        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        EndDrawing();
    }
    UnloadTexture(scarfy);
    CloseWindow();
    return 0;
}