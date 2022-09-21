#pragma once
#include <vector>
#include <string>
#define LEFT_FRONT_DOWN_CORNER 0
#define LEFT_FRONT_EDGE 1
#define LEFT_FRONT_UP_CORNER 2
#define LEFT_DOWN_EDGE 3
#define LEFT_CENTER 4
#define LEFT_UP_EDGE 5
#define LEFT_BACK_DOWN_CORNER 6
#define LEFT_BACK_EDGE 7
#define LEFT_BACK_UP_CORNER 8
#define FRONT_DOWN_EDGE 9
#define FRONT_CENTER 10
#define FRONT_UP_EDGE 11
#define DOWN_CENTER 12
#define UP_CENTER 13
#define BACK_DOWN_EDGE 14
#define BACK_CENTER 15
#define BACK_UP_EDGE 16
#define RIGHT_FRONT_DOWN_CORNER 17
#define RIGHT_FRONT_EDGE 18
#define RIGHT_FRONT_UP_CORNER 19
#define RIGHT_DOWN_EDGE 20
#define RIGHT_CENTER 21
#define RIGHT_UP_EDGE 22
#define RIGHT_BACK_DOWN_CORNER 23
#define RIGHT_BACK_EDGE 24
#define RIGHT_BACK_UP_CORNER 25

struct Piece{
	int coord[3], color[3];
	bool operator==(const Piece &other) const;
	bool operator!=(const Piece &other) const;
};

enum Color{
	White = 0,
	Green,
	Red,
	Blue,
	Orange,
	Yellow
};

struct Cube{
	Cube();
	std::string toString(const std::string &strCube) const;
	const Piece &getPiece(int x, int y, int z) const;
	bool isSolved() const;
	bool isSolved2x2() const;
	bool isSolved(const Cube &ref) const;
	bool isPiecesSolved(const std::vector<int> &pieceIdcs, const Cube &ref) const;
	bool isOLL(const Cube &ref) const;
	std::vector<Piece> pieces;
};
