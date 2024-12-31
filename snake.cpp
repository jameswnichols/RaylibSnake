//
// Created by jnich on 30/12/2024.
//
#include "snake.h"

#include <iostream>
#include "include/raylib-cpp.hpp"
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

void SnakeBodySection::SetHasFood(bool NewHasFood) {
    HasFood = NewHasFood;
}

bool SnakeBodySection::GetHasFood() {
    return HasFood;
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

void Snake::ProcessFood() {
    bool MovedFood = false;
    SnakeBodySection *CurrentSection = Head;
    while (CurrentSection != nullptr and not MovedFood) {
        if (CurrentSection->GetHasFood()) {
            CurrentSection->SetHasFood(false);
            SnakeBodySection *NextSection = CurrentSection->GetNextSection();
            if (NextSection != nullptr) {
                MovedFood = true;
                NextSection->SetHasFood(true);
            }
        }
        CurrentSection = CurrentSection->GetNextSection();
    }
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

    CurrentSection->GetPreviousSection()->SetHasFood(CurrentSection->GetHasFood());
    CurrentSection->SetHasFood(false);

    CurrentSection->SetNextSection(Head);
    CurrentSection->SetPreviousSection(nullptr);

    Head->SetPreviousSection(CurrentSection);
    Head = CurrentSection;

    return false;
}

void Snake::Draw(int LastX, int LastY, bool AppleAhead, SnakeTextures *Textures, int BoxSize) {
    bool FirstSection = true;
    SnakeBodySection *CurrentSection = Head;
    while (CurrentSection != nullptr) {
        raylib::Texture2D *Texture = Textures->Vertical;

        //First Section
        if (FirstSection) {
            FirstSection = false;
            if (LastX == -1) {
                Texture = AppleAhead ? Textures->HeadLeftOpen : Textures->HeadLeft;
            }
            else if (LastX == 1) {
                Texture = AppleAhead ? Textures->HeadRightOpen : Textures->HeadRight;
            }
            else if (LastY == -1) {
                Texture = AppleAhead ? Textures->HeadUpOpen : Textures->HeadUp;
            }
            else if (LastY == 1) {
                Texture = AppleAhead ? Textures->HeadDownOpen : Textures->HeadDown;
            }
        }
        //Last Section
        else if (CurrentSection->GetNextSection() == nullptr) {
            SnakeBodySection *PreviousSection = CurrentSection->GetPreviousSection();
            int PreviousChangeX = PreviousSection->GetX() - CurrentSection->GetX();
            int PreviousChangeY = PreviousSection->GetY() - CurrentSection->GetY();
            bool PieceAbove = (PreviousChangeY < 0);
            bool PieceBelow = (PreviousChangeY > 0);
            bool PieceLeft = (PreviousChangeX < 0);
            bool PieceRight = (PreviousChangeX > 0);
            if (PieceAbove) {
                Texture = Textures->TailUp;
            }
            else if (PieceBelow) {
                Texture = Textures->TailDown;
            }
            else if (PieceLeft) {
                Texture = Textures->TailRight;
            }
            else if (PieceRight) {
                Texture = Textures->TailLeft;
            }
        }
        //Middle Pieces
        else {
            SnakeBodySection *NextSection = CurrentSection->GetNextSection();
            SnakeBodySection *PreviousSection = CurrentSection->GetPreviousSection();
            int PreviousChangeX = PreviousSection->GetX() - CurrentSection->GetX();
            int PreviousChangeY = PreviousSection->GetY() - CurrentSection->GetY();
            if (NextSection == nullptr) {
                if (PreviousChangeX != 0) {
                    Texture = CurrentSection->GetHasFood() ? Textures->HorizontalFood : Textures->Horizontal;
                    //Text = "-";
                }
                else if (PreviousChangeY != 0) {
                    Texture = CurrentSection->GetHasFood() ? Textures->VerticalFood : Textures->Vertical;
                    //Text = "|";
                }
            }
            else {
                int NextSectionChangeX = NextSection->GetX() - CurrentSection->GetX();
                int NextSectionChangeY = NextSection->GetY() - CurrentSection->GetY();
                bool PieceAbove = (NextSectionChangeY < 0 or PreviousChangeY < 0);
                bool PieceBelow = (NextSectionChangeY > 0 or PreviousChangeY > 0);
                bool PieceLeft = (NextSectionChangeX < 0 or PreviousChangeX < 0);
                bool PieceRight = (NextSectionChangeX > 0 or PreviousChangeX > 0);
                if (PieceAbove and PieceBelow) {
                    Texture = CurrentSection->GetHasFood() ? Textures->VerticalFood : Textures->Vertical;
                    //Text = "┃";
                }
                if (PieceLeft and PieceRight) {
                    Texture = CurrentSection->GetHasFood() ? Textures->HorizontalFood : Textures->Horizontal;
                    //Text = "━";
                }
                if (PieceAbove and PieceLeft) {
                    Texture = CurrentSection->GetHasFood() ? Textures->UpLeftFood : Textures->UpLeft;
                    //Text = "┛";
                }
                if (PieceAbove and PieceRight) {
                    Texture = CurrentSection->GetHasFood() ? Textures->UpRightFood : Textures->UpRight;
                    //Text = "┗";
                }
                if (PieceBelow and PieceLeft) {
                    Texture = CurrentSection->GetHasFood() ? Textures->DownLeftFood : Textures->DownLeft;
                    //Text = "┓";
                }
                if (PieceBelow and PieceRight) {
                    Texture = CurrentSection->GetHasFood() ? Textures->DownRightFood : Textures->DownRight;
                    //Text = "┏";
                }

            }

        }

        const int ScreenX = BoxSize * CurrentSection->GetX();
        const int ScreenY = BoxSize * CurrentSection->GetY();
        //DrawRectangle(ScreenX, ScreenY, BoxSize, BoxSize, GREEN);
        Texture->Draw(ScreenX, ScreenY);
        //DrawText(Text, (ScreenX + (BoxSize / 2)) - FontOffset.x, (ScreenY + (BoxSize / 2)) - FontOffset.y, 35.0, BLACK);
        //FontToUse->DrawText(Text, Vector2Subtract(Vector2(ScreenX + BoxSize / 2, ScreenY + BoxSize / 2), FontOffset), 35.0, 0.0, BLACK);
        CurrentSection = CurrentSection->GetNextSection();
    }
}

int Snake::GetLength() {
    return Length;
}

SnakeTextures::SnakeTextures(raylib::Texture2D *V, raylib::Texture2D *H, raylib::Texture2D *UL, raylib::Texture2D *DL, raylib::Texture2D *UR, raylib::Texture2D *DR, raylib::Texture2D *TU, raylib::Texture2D *TD, raylib::Texture2D *TL, raylib::Texture2D *TR, raylib::Texture2D *HU, raylib::Texture2D *HD, raylib::Texture2D *HR, raylib::Texture2D *HL, raylib::Texture2D *HUO, raylib::Texture2D *HDO, raylib::Texture2D *HRO, raylib::Texture2D *HLO, raylib::Texture2D *VF, raylib::Texture2D *HF, raylib::Texture2D *ULF, raylib::Texture2D *DLF, raylib::Texture2D *URF, raylib::Texture2D *DRF) :
Vertical(V),
Horizontal(H),
UpLeft(UL),
DownLeft(DL),
UpRight(UR),
DownRight(DR),
TailUp(TU),
TailDown(TD),
TailLeft(TR),
TailRight(TL),
HeadUp(HU),
HeadDown(HD),
HeadLeft(HL),
HeadRight(HR),
HeadUpOpen(HUO),
HeadDownOpen(HDO),
HeadLeftOpen(HLO),
HeadRightOpen(HRO),
VerticalFood(VF),
HorizontalFood(HF),
UpLeftFood(ULF),
DownLeftFood(DLF),
UpRightFood(URF),
DownRightFood(DRF)
{}




