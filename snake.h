//
// Created by jnich on 30/12/2024.
//

#ifndef SNAKE_H
#define SNAKE_H

#include "include/raylib-cpp.hpp"

class SnakeBodySection {
private:
    int X;
    int Y;
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
};

class Snake {
private:
    SnakeBodySection *Head = nullptr;
    int Length = 0;
public:
    Snake(int StartX, int StartY);
    SnakeBodySection *GetHead();
    bool Move(int XChange, int YChange);
    void AddSegment();
    void Draw(int ScreenWidth, int ScreenHeight, int BoxSize);
    int GetLength();
};

#endif //SNAKE_H
