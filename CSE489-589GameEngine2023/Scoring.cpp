#pragma once
#include "Scoring.h"

int Scoring::score = 0;

void Scoring::addPoints(int points) {
    score += points;
    std::cout << "Current score: " << score << std::endl;
}

int Scoring::getScore() {
    return score;
}