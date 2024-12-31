#include "include/raylib-cpp.hpp"
#include <string>
#include <random>

#include "snake.h"

int main() {
    int ScreenWidth = 800;
    int ScreenHeight = 800;

    int BoxSize = 20;

    float CurrentSecondsForMovement = 0.0;
    float SecondsPerMovement = 0.1;

    raylib::Window window(ScreenWidth, ScreenHeight, "Raylib Snake");

    raylib::Font BookFont("BookFont.ttf", 35);
    Vector2 FontSizeMiddle = BookFont.MeasureText("#", 35, 0.0) / 2;


    auto *TitleString = new std::string("Raylib Snake - "+std::to_string(window.GetFPS())+"FPS");

    //SetTargetFPS(240);

    auto *PlayerSnake = new Snake((ScreenWidth / BoxSize) / 2, (ScreenHeight / BoxSize) / 2);
    for (int i = 0; i < 2; i++) {
        PlayerSnake->AddSegment();
    }

    int DirectionToMoveX = 0;
    int DirectionToMoveY = 1;

    int LastTimedMoveDirectionX = DirectionToMoveX;
    int LastTimedMoveDirectionY = DirectionToMoveY;

    std::random_device RandomDevice;
    std::mt19937 Random(RandomDevice());
    std::uniform_int_distribution<> RandomWidth(0, ScreenWidth / BoxSize - 1);
    std::uniform_int_distribution<> RandomHeight(0, ScreenHeight / BoxSize - 1);

    int ApplePositionX = RandomWidth(Random);
    int ApplePositionY = RandomHeight(Random);

    bool GameOver = false;

    while (!window.ShouldClose())
    {
        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        *TitleString = "Raylib Snake - "+std::to_string(window.GetFPS())+"FPS " + std::to_string(CurrentSecondsForMovement);
        window.SetTitle(*TitleString);

        if (GameOver) {
            auto DeathText = "RIP! You Died With A Length Of "+std::to_string(PlayerSnake->GetLength())+"!";
            auto DeathTextCString = DeathText.c_str();
            int TextWidth = MeasureText(DeathTextCString, 30);
            DrawText(DeathTextCString, (ScreenWidth / 2) - (TextWidth / 2), ScreenHeight / 2, 30, LIGHTGRAY);
            EndDrawing();
            continue;
        }

        DrawRectangle(ApplePositionX * BoxSize, ApplePositionY * BoxSize, BoxSize, BoxSize, RED);
        PlayerSnake->Draw(LastTimedMoveDirectionX, LastTimedMoveDirectionY, FontSizeMiddle, &BookFont, BoxSize);

        EndDrawing();

        if (IsKeyPressed(KEY_UP) and LastTimedMoveDirectionY == 0){
            DirectionToMoveX = 0;
            DirectionToMoveY = -1;
        }
        else if (IsKeyPressed(KEY_DOWN) and LastTimedMoveDirectionY == 0) {
            DirectionToMoveX = 0;
            DirectionToMoveY = 1;
        }
        else if (IsKeyPressed(KEY_LEFT) and LastTimedMoveDirectionX == 0) {
            DirectionToMoveX = -1;
            DirectionToMoveY = 0;
        }
        else if (IsKeyPressed(KEY_RIGHT) and LastTimedMoveDirectionX == 0) {
            DirectionToMoveX = 1;
            DirectionToMoveY = 0;
        }

        if (CurrentSecondsForMovement >= SecondsPerMovement) {

            if (PlayerSnake->GetHead()->GetX() == ApplePositionX and PlayerSnake->GetHead()->GetY() == ApplePositionY) {
                ApplePositionX = RandomWidth(Random);
                ApplePositionY = RandomHeight(Random);
                PlayerSnake->AddSegment();
            }

            CurrentSecondsForMovement = 0.0;
            GameOver = PlayerSnake->Move(DirectionToMoveX, DirectionToMoveY);
            LastTimedMoveDirectionX = DirectionToMoveX;
            LastTimedMoveDirectionY = DirectionToMoveY;
        }

        CurrentSecondsForMovement += window.GetFrameTime();

    }
    return 0;
}
