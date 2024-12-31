//
// Created by jnich on 30/12/2024.
//

#ifndef SNAKE_H
#define SNAKE_H

#include "include/raylib-cpp.hpp"

class SnakeTextures {
public:
    raylib::Texture2D *Vertical;
    raylib::Texture2D *Horizontal;
    raylib::Texture2D *UpLeft;
    raylib::Texture2D *DownLeft;
    raylib::Texture2D *UpRight;
    raylib::Texture2D *DownRight;
    raylib::Texture2D *TailUp;
    raylib::Texture2D *TailDown;
    raylib::Texture2D *TailLeft;
    raylib::Texture2D *TailRight;
    raylib::Texture2D *HeadUp;
    raylib::Texture2D *HeadDown;
    raylib::Texture2D *HeadLeft;
    raylib::Texture2D *HeadRight;
    raylib::Texture2D *HeadUpOpen;
    raylib::Texture2D *HeadDownOpen;
    raylib::Texture2D *HeadLeftOpen;
    raylib::Texture2D *HeadRightOpen;
    raylib::Texture2D *VerticalFood;
    raylib::Texture2D *HorizontalFood;
    raylib::Texture2D *UpLeftFood;
    raylib::Texture2D *DownLeftFood;
    raylib::Texture2D *UpRightFood;
    raylib::Texture2D *DownRightFood;
    SnakeTextures(raylib::Texture2D *V, raylib::Texture2D *H, raylib::Texture2D *UL, raylib::Texture2D *DL, raylib::Texture2D *UR, raylib::Texture2D *DR, raylib::Texture2D *TU, raylib::Texture2D *TD, raylib::Texture2D *TL, raylib::Texture2D *TR, raylib::Texture2D *HU, raylib::Texture2D *HD, raylib::Texture2D *HR, raylib::Texture2D *HL, raylib::Texture2D *HUO, raylib::Texture2D *HDO, raylib::Texture2D *HRO, raylib::Texture2D *HLO, raylib::Texture2D *VF, raylib::Texture2D *HF, raylib::Texture2D *ULF, raylib::Texture2D *DLF, raylib::Texture2D *URF, raylib::Texture2D *DRF);
};

class SnakeBodySection {
private:
    int X;
    int Y;
    bool HasFood;
    SnakeBodySection *NextSection = nullptr;
    SnakeBodySection *PreviousSection = nullptr;
public:
    SnakeBodySection(int X, int Y);
    SnakeBodySection *GetNextSection();
    SnakeBodySection *GetPreviousSection();
    void SetNextSection(SnakeBodySection *NewNextSection);
    void SetPreviousSection(SnakeBodySection *NewPreviousSection);
    int GetX();
    int GetY();
    void MoveTo(int NewX, int NewY);
    void SetHasFood(bool NewHasFood);
    bool GetHasFood();
};

class Snake {
private:
    SnakeBodySection *Head = nullptr;
    int Length = 0;
public:
    Snake(int StartX, int StartY);
    SnakeBodySection *GetHead();
    void ProcessFood();
    bool Move(int XChange, int YChange);
    void AddSegment();
    void Draw(int LastX, int LastY, bool AppleAhead, SnakeTextures *Textures, int BoxSize);
    int GetLength();
};

#endif //SNAKE_H
