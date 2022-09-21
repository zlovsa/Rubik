#pragma once
#include <vector>
#include <string>
#include "Cube.hpp"

struct Rotation{
	Rotation(std::string id, int dimension, int coordinate, int angle);
	std::string id;
	int dimension, coordinate, angle;
	int m[3][3];
};

class CubeSolver
{
public:
	class UnknownRotation : public std::exception{
	public:
		virtual const char *what() const throw(){
			return "Unknown rotation";
		}
	};
	CubeSolver();
	CubeSolver(const CubeSolver &other);
	CubeSolver &operator=(const CubeSolver &other);
	~CubeSolver();
	std::string GenerateScramble(int ScrambleLength) const;
	void Rotate(Cube &cube, const Rotation &rotation) const;
	void Rotate(Cube &cube, const std::string &rotationId) const;
	void Scramble(Cube &cube, const std::string &scramble) const;
	void Scramble(Cube &cube, const std::vector<int> &scramble, const std::vector<std::string> &moves) const;
	std::vector<std::string> Solve(const std::string &scramble, bool cube2x2 = false) const;
	std::vector<std::string> Solve(Cube &cube, bool cube2x2 = false) const;
	std::vector<std::string> Solve2x2(Cube &cube) const;
	std::vector<std::string> SolvePieces(Cube &cube,
										 const std::vector<int> &pieceIdcs,
										 const std::vector<std::string> &moves,
										 size_t maxMoves,
										 int lastMoveFrom,
										 bool (Cube::*cond)(const Cube &) const = NULL) const;
	std::vector<std::string> WhiteCross(Cube &cube, std::vector<int> &solvedPiecesIdcs) const;
	std::vector<std::string> FirstTwoLayers(Cube &cube, std::vector<int> &solvedPiecesIdcs) const;
	std::vector<std::string> OrientLastLayer(Cube &cube, std::vector<int> &solvedPiecesIdcs) const;
	std::vector<std::string> PermutateLastLayer(Cube &cube, std::vector<int> &solvedPiecesIdcs) const;
	std::vector<std::string> CornersFirstLayer(Cube &cube, std::vector<int> &solvedPiecesIdcs) const;
	std::vector<std::string> CornersLastLayer(Cube &cube, std::vector<int> &solvedPiecesIdcs) const;
	std::string cubeToString(const Cube &cube, bool cube2x2 = false) const;

private:
	std::vector<std::string> VerboseSolution(const std::vector<int> &solution, const std::vector<std::string> &moves) const;
	void initRotations();
	void initF2Lmoves();
	void initOLLmoves();
	void initPLLmoves();
	void initCLLmoves();
	void initGraph();
	std::vector<Rotation> rotations;
	Cube reference;
	std::vector<std::string> f2lmoves;
	std::vector<std::string> ollmoves;
	std::vector<std::string> pllmoves;
	std::vector<std::string> cllmoves;
	std::string str3x3, str2x2;
};
