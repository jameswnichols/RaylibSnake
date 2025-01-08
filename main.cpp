#include "include/raylib-cpp.hpp"
#include <string>
#include <random>

#include "snake.h"

int main() {
    int ScreenWidth = 800;
    int ScreenHeight = 800;

    int BoxSize = 40;

    float CurrentSecondsForMovement = 0.0;
    float SecondsPerMovement = 0.125;

    raylib::Window window(ScreenWidth, ScreenHeight, "Raylib Snake");

    raylib::Color BackgroundColour(169, 224, 0);

    raylib::Font ScoreFont("ScoreFont.ttf", 40);

    raylib::Texture2D Vertical("Vertical.png");
    raylib::Texture2D Horizontal("Horizontal.png");
    raylib::Texture2D UpLeft("UpLeft.png");
    raylib::Texture2D DownLeft("DownLeft.png");
    raylib::Texture2D UpRight("UpRight.png");
    raylib::Texture2D DownRight("DownRight.png");

    raylib::Texture2D VerticalFood("VerticalFood.png");
    raylib::Texture2D HorizontalFood("HorizontalFood.png");
    raylib::Texture2D UpLeftFood("UpLeftFood.png");
    raylib::Texture2D DownLeftFood("DownLeftFood.png");
    raylib::Texture2D UpRightFood("UpRightFood.png");
    raylib::Texture2D DownRightFood("DownRightFood.png");

    raylib::Texture2D TailUp("TailUp.png");
    raylib::Texture2D TailDown("TailDown.png");
    raylib::Texture2D TailLeft("TailLeft.png");
    raylib::Texture2D TailRight("TailRight.png");

    raylib::Texture2D HeadUp("HeadUp.png");
    raylib::Texture2D HeadDown("HeadDown.png");
    raylib::Texture2D HeadLeft("HeadLeft.png");
    raylib::Texture2D HeadRight("HeadRight.png");

    raylib::Texture2D HeadUpOpen("HeadUpOpen.png");
    raylib::Texture2D HeadDownOpen("HeadDownOpen.png");
    raylib::Texture2D HeadLeftOpen("HeadLeftOpen.png");
    raylib::Texture2D HeadRightOpen("HeadRightOpen.png");

    raylib::Texture2D Food("Food.png");

    raylib::Rectangle Rectangle(40, 80, ScreenWidth - 80, ScreenHeight - 120);

    SnakeTextures SnakeStuff(&Vertical, &Horizontal, &UpLeft, &DownLeft, &UpRight, &DownRight, &TailUp, &TailDown, &TailLeft, &TailRight, &HeadUp, &HeadDown, &HeadRight, &HeadLeft, &HeadUpOpen, &HeadDownOpen, &HeadRightOpen, &HeadLeftOpen, &VerticalFood, &HorizontalFood, &UpLeftFood, &DownLeftFood, &UpRightFood, &DownRightFood);

    auto *TitleString = new std::string("Raylib Snake - "+std::to_string(window.GetFPS())+"FPS");

    SetTargetFPS(240);

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
    std::uniform_int_distribution<> RandomWidth(1, 18); //ScreenWidth / BoxSize - 1
    std::uniform_int_distribution<> RandomHeight(2, 18); //ScreenHeight / BoxSize - 1

    int ApplePositionX = RandomWidth(Random);
    int ApplePositionY = RandomHeight(Random);

    bool GameOver = false;
    bool FruitPickedUp = false;

    while (!window.ShouldClose())
    {
        BeginDrawing();

        window.ClearBackground(BackgroundColour);

        *TitleString = "Raylib Snake - "+std::to_string(window.GetFPS())+"FPS " + std::to_string(CurrentSecondsForMovement);
        window.SetTitle(*TitleString);

        if (GameOver) {
            auto DeathText = "RIP! You Died With A Score Of "+std::to_string(PlayerSnake->GetLength() - 3)+"!";
            auto DeathTextCString = DeathText.c_str();
            Vector2 TextWidth = ScoreFont.MeasureText(DeathText, 20, 0.0);//MeasureText(DeathTextCString, 20);
            ScoreFont.DrawText(DeathText, (ScreenWidth / 2) - (TextWidth.x / 2), ScreenHeight / 2, 20, 0.0, BLACK);
            EndDrawing();
            continue;
        }

        SnakeBodySection *Head = PlayerSnake->GetHead();
        int SpaceAheadX = Head->GetX() + LastTimedMoveDirectionX;
        int SpaceAheadY = Head->GetY() + LastTimedMoveDirectionY;
        bool NextMoveApple = (SpaceAheadX == ApplePositionX && SpaceAheadY == ApplePositionY) or (Head->GetX() == ApplePositionX && Head->GetY() == ApplePositionY);

        PlayerSnake->Draw(LastTimedMoveDirectionX, LastTimedMoveDirectionY, NextMoveApple, &SnakeStuff, BoxSize);

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
                PlayerSnake->GetHead()->SetHasFood(true);
                FruitPickedUp = true;
            }

            CurrentSecondsForMovement = 0.0;
            PlayerSnake->ProcessFood();
            GameOver = PlayerSnake->Move(DirectionToMoveX, DirectionToMoveY);

            int SnakeHeadPositionX = PlayerSnake->GetHead()->GetX();
            int SnakeHeadPositionY = PlayerSnake->GetHead()->GetY();

            if (SnakeHeadPositionX < 1 or SnakeHeadPositionX > 18 or SnakeHeadPositionY < 2 or SnakeHeadPositionY > 18) {
                GameOver = true;
            }


            LastTimedMoveDirectionX = DirectionToMoveX;
            LastTimedMoveDirectionY = DirectionToMoveY;
        }

        CurrentSecondsForMovement += window.GetFrameTime();

        if (FruitPickedUp) {
            FruitPickedUp = false;
        }else {
            Food.Draw(ApplePositionX * BoxSize, ApplePositionY * BoxSize);
        }

        ScoreFont.DrawText("Score: " + std::to_string(PlayerSnake->GetLength() - 3), 40, 30, 30, 0.0, BLACK);

        Rectangle.DrawLines(BLACK, 5.0);

        EndDrawing();

    }
    return 0;
}
