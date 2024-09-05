#pragma once
#include <iostream>

class Scoring {
public:
	static void addPoints(int points);
	static int getScore();

private:
	static int score;
};