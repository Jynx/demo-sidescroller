#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool IsOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

void updateAnimData(AnimData &data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0f;
        data.rec.x = data.frame * data.rec.width;
        data.frame = (data.frame + 1) % maxFrame;
    }
}

AnimData CreateAnimData(Texture2D texture, int windowDimensions[2])
{
    AnimData data;
    data.rec.width = texture.width / 6.0;
    data.rec.height = texture.height;
    data.rec.x = 0.0;
    data.rec.y = 0.0;
    data.pos.x = windowDimensions[0] / 2 - data.rec.width / 2;
    data.pos.y = windowDimensions[1] - data.rec.height;
    data.frame = 0;
    data.updateTime = 1.0 / 12.0;
    data.runningTime = 0.0f;

    return data;
}

AnimData CreateNebulaAnimData(Texture2D texture, int windowDimensions[2], float posX)
{
    AnimData data;
    data.rec.width = texture.width / 8.0;
    data.rec.height = texture.height / 8.0;
    data.rec.x = 0.0;
    data.rec.y = 0.0;
    data.pos.x = posX;
    data.pos.y = windowDimensions[1] - texture.height / 8;
    data.frame = 0;
    data.updateTime = 0.0;
    data.runningTime = 0.0f;

    return data;
}

int main()
{
    int windowDimensions[2]{512, 380};

    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher");
    SetTargetFPS(60);

    const int jumpVal{-600};
    const int gravity{1000};
    int velocity{0};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{0.0};

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{0.0};

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{0.0};

    Texture2D scarfyTexture = LoadTexture("textures/scarfy.png");
    AnimData scarfyData = CreateAnimData(scarfyTexture, windowDimensions);

    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    const int sizeOfNebulae{6};
    AnimData nebulae[sizeOfNebulae]{};
    for (int i = 0; i < sizeOfNebulae; i++)
    {
        float posXModifier{i * 300.0f + windowDimensions[0]};
        nebulae[i] = CreateNebulaAnimData(nebula, windowDimensions, posXModifier);
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    // pixels /s for neb x value
    float nebVelocity{-200};

    bool isJumping{false};
    bool collision{false};
    while (!WindowShouldClose())
    {
        float deltaTime{GetFrameTime()};
        BeginDrawing();
        ClearBackground(RAYWHITE);

        bgX -= 20 * deltaTime;
        // reset background if it's scrolled to the max width of 2 background widths
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }

        mgX -= 40 * deltaTime;
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }

        fgX -= 80 * deltaTime;
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }

        // draw scrolling background
        Vector2 background1Pos{bgX, 0.0};
        DrawTextureEx(background, background1Pos, 0.0, 2.0, WHITE);
        Vector2 background2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, background2Pos, 0.0, 2.0, WHITE);

        Vector2 midground1Pos{mgX, 0.0};
        DrawTextureEx(midground, midground1Pos, 0.0, 2.0, WHITE);
        Vector2 midground2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, midground2Pos, 0.0, 2.0, WHITE);

        Vector2 foreground1Pos{fgX, 0.0};
        DrawTextureEx(foreground, foreground1Pos, 0.0, 2.0, WHITE);
        Vector2 foreground2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, foreground2Pos, 0.0, 2.0, WHITE);

        if (IsOnGround(scarfyData, windowDimensions[1]))
        {
            velocity = 0;
            isJumping = false;
        }
        else
        {
            velocity += gravity * deltaTime;
            isJumping = true;
        }
        if (IsKeyPressed(KEY_SPACE) && !isJumping)
        {
            velocity += jumpVal;
        }

        scarfyData.pos.y += velocity * deltaTime;

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebVelocity * deltaTime;
        }

        finishLine += nebVelocity * deltaTime;

        if (!isJumping)
        {
            updateAnimData(scarfyData, deltaTime, 6);
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            updateAnimData(nebulae[i], deltaTime, 8);
        }

        for (AnimData nebula : nebulae)
        {
            float pad{30};

            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad};

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height};

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            DrawText("Game Over!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, RED);
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            DrawText("You Win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, GREEN);
        }
        else
        {
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            DrawTextureRec(scarfyTexture, scarfyData.rec, scarfyData.pos, WHITE);
        }
        EndDrawing();
    }
    UnloadTexture(scarfyTexture);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}