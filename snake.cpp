//
// Created by jnich on 30/12/2024.
//
#include "snake.h"

#include <iostream>
#include <ostream>


SnakeBodySection::SnakeBodySection(int X, int Y) : X(X), Y(Y) {}

SnakeBodySection *SnakeBodySection::GetNextSection() {
    return NextSection;
}

SnakeBodySection *SnakeBodySection::GetPreviousSection() {
    return PreviousSection;
}

void SnakeBodySection::SetNextSection(SnakeBodySection *NewNextSection) {
    NextSection = NewNextSection;
}

void SnakeBodySection::SetPreviousSection(SnakeBodySection *NewPreviousSection) {
    PreviousSection = NewPreviousSection;
}

int SnakeBodySection::GetX() {
    return X;
}

int SnakeBodySection::GetY() {
    return Y;
}

void SnakeBodySection::MoveTo(int NewX, int NewY) {
    X = NewX;
    Y = NewY;
}


Snake::Snake(int StartX, int StartY) {
    auto StartSection = new SnakeBodySection(StartX, StartY);
    Head = StartSection;
    Length = 1;
}

SnakeBodySection *Snake::GetHead() {
    return Head;
}

void Snake::AddSegment() {
    SnakeBodySection *CurrentSection = Head;
    while (CurrentSection->GetNextSection() != nullptr) {
        CurrentSection = CurrentSection->GetNextSection();
    }
    auto *NewSection = new SnakeBodySection(CurrentSection->GetX(), CurrentSection->GetY());
    CurrentSection->SetNextSection(NewSection);
    NewSection->SetPreviousSection(CurrentSection);
    Length++;
}

bool Snake::Move(int XChange, int YChange) {

    int NewX = Head->GetX()+XChange;
    int NewY = Head->GetY()+YChange;

    SnakeBodySection *CurrentSection = Head;
    while (CurrentSection->GetNextSection() != nullptr) {
        if (CurrentSection->GetX() == NewX && CurrentSection->GetY() == NewY) {
            return true;
        }
        CurrentSection = CurrentSection->GetNextSection();
    }
    if (CurrentSection->GetX() == NewX && CurrentSection->GetY() == NewY) {
        return true;
    }

    CurrentSection->MoveTo(NewX, NewY);

    CurrentSection->GetPreviousSection()->SetNextSection(nullptr);

    CurrentSection->SetNextSection(Head);
    CurrentSection->SetPreviousSection(nullptr);

    Head->SetPreviousSection(CurrentSection);
    Head = CurrentSection;
    return false;
}

void Snake::Draw(int ScreenWidth, int ScreenHeight, int BoxSize) {
    SnakeBodySection *CurrentSection = Head;
    while (CurrentSection->GetNextSection() != nullptr) {
        const int ScreenX = BoxSize * CurrentSection->GetX();
        const int ScreenY = BoxSize * CurrentSection->GetY();
        DrawRectangle(ScreenX, ScreenY, BoxSize, BoxSize, GREEN);
        CurrentSection = CurrentSection->GetNextSection();
    }
    const int ScreenX = BoxSize * CurrentSection->GetX();
    const int ScreenY = BoxSize * CurrentSection->GetY();
    DrawRectangle(ScreenX, ScreenY, BoxSize, BoxSize, GREEN);
}

int Snake::GetLength() {
    return Length;
}



