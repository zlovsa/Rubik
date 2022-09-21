#include <Cube.hpp>
#include <sstream>
#include <algorithm>

bool Piece::operator==(const Piece &other) const{
	return coord[0] == other.coord[0] &&
		   coord[1] == other.coord[1] &&
		   coord[2] == other.coord[2] &&
		   (!coord[0] || color[0] == other.color[0]) &&
		   (!coord[1] || color[1] == other.color[1]) &&
		   (!coord[2] || color[2] == other.color[2]);
}

bool Piece::operator!=(const Piece &other) const{
	return !(*this == other);
}

Cube::Cube(){
	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
			for (int z = -1; z <= 1; z++){
				if (x == 0 && y == 0 && z == 0)
					continue;
				Piece p;
				p.coord[0] = x;
				p.coord[1] = y;
				p.coord[2] = z;
				p.color[0] = x == 1 ? Red : Orange; // Red : Orange;
				p.color[1] = y == 1 ? Green : Blue; // Blue : Green;
				p.color[2] = z == 1 ? Yellow : White; // White : Yellow;
				pieces.push_back(p);
			}
}

const Piece &Cube::getPiece(int x, int y, int z) const{
	for (auto &p : pieces)
		if (p.coord[0] == x && p.coord[1] == y && p.coord[2] == z)
			return p;
	throw std::runtime_error("Unkown piece coordinates");
}

std::string Cube::toString(const std::string &strCube) const{
	struct pieceFace{
		char fId;
		int x, y, z, face;
		pieceFace(char fId, int x, int y, int z, int face) : fId(fId), x(x), y(y), z(z), face(face) {}
	};
	std::vector<pieceFace> faces = {
		pieceFace('a', -1, 1, 1, 2),
		pieceFace('b', 0, 1, 1, 2),
		pieceFace('c', 1, 1, 1, 2),
		pieceFace('d', -1, 0, 1, 2),
		pieceFace('e', 0, 0, 1, 2),
		pieceFace('f', 1, 0, 1, 2),
		pieceFace('g', -1, -1, 1, 2),
		pieceFace('h', 0, -1, 1, 2),
		pieceFace('i', 1, -1, 1, 2),
		pieceFace('j', -1, 1, 1, 0),
		pieceFace('k', -1, 0, 1, 0),
		pieceFace('l', -1, -1, 1, 0),
		pieceFace('m', -1, 1, 0, 0),
		pieceFace('n', -1, 0, 0, 0),
		pieceFace('o', -1, -1, 0, 0),
		pieceFace('p', -1, 1, -1, 0),
		pieceFace('q', -1, 0, -1, 0),
		pieceFace('r', -1, -1, -1, 0),
		pieceFace('s', -1, -1, 1, 1),
		pieceFace('t', 0, -1, 1, 1),
		pieceFace('u', 1, -1, 1, 1),
		pieceFace('v', -1, -1, 0, 1),
		pieceFace('w', 0, -1, 0, 1),
		pieceFace('x', 1, -1, 0, 1),
		pieceFace('y', -1, -1, -1, 1),
		pieceFace('z', 0, -1, -1, 1),
		pieceFace('A', 1, -1, -1, 1),
		pieceFace('B', 1, -1, 1, 0),
		pieceFace('C', 1, 0, 1, 0),
		pieceFace('D', 1, 1, 1, 0),
		pieceFace('E', 1, -1, 0, 0),
		pieceFace('F', 1, 0, 0, 0),
		pieceFace('G', 1, 1, 0, 0),
		pieceFace('H', 1, -1, -1, 0),
		pieceFace('I', 1, 0, -1, 0),
		pieceFace('J', 1, 1, -1, 0),
		pieceFace('K', -1, 1, 1, 1),
		pieceFace('L', 0, 1, 1, 1),
		pieceFace('M', 1, 1, 1, 1),
		pieceFace('N', -1, 1, 0, 1),
		pieceFace('O', 0, 1, 0, 1),
		pieceFace('P', 1, 1, 0, 1),
		pieceFace('Q', -1, 1, -1, 1),
		pieceFace('R', 0, 1, -1, 1),
		pieceFace('S', 1, 1, -1, 1),
		pieceFace('T', -1, 1, -1, 2),
		pieceFace('U', 0, 1, -1, 2),
		pieceFace('V', 1, 1, -1, 2),
		pieceFace('W', -1, 0, -1, 2),
		pieceFace('X', 0, 0, -1, 2),
		pieceFace('Y', 1, 0, -1, 2),
		pieceFace('Z', -1, -1, -1, 2),
		pieceFace('1', 0, -1, -1, 2),
		pieceFace('2', 1, -1, -1, 2)};
	std::string r;
	std::string strColors[] = {"\033[1;37m█\033[0m",  // White
							   "\033[1;32m█\033[0m",  // Green
							   "\033[1;31m█\033[0m",  // Red
							   "\033[1;34m█\033[0m",  // Blue
							   "\033[1;35m█\033[0m",  // Orange
							   "\033[1;33m█\033[0m"}; // Yellow
	for (auto faceId : strCube){
		auto ri = std::find_if(faces.begin(), faces.end(),
							   [faceId](const pieceFace &pf)
							   { return pf.fId == faceId; });
		if (ri == faces.end())
			r += faceId;
		else
			r += strColors[getPiece(ri->x, ri->y, ri->z).color[ri->face]];
	}
	return r;
}

bool Cube::isSolved(const Cube &ref) const{
	return pieces == ref.pieces;
}

bool Cube::isSolved() const{
	return isSolved(Cube());
}

bool Cube::isPiecesSolved(const std::vector<int> &pieceIdcs, const Cube &ref) const{
	for (auto pidx : pieceIdcs)
		if (pieces[pidx] != ref.pieces[pidx])
			return false;
	return true;
}

bool Cube::isOLL(const Cube &ref) const{
	int refColor = ref.pieces[UP_CENTER].color[2];
	for (size_t i = 0; i < pieces.size(); i++)
		if (pieces[i].coord[2] == 1 && pieces[i].color[2] != refColor)
			return false;
	return true;
}

bool Cube::isSolved2x2() const{
	std::vector<int> corners = {LEFT_FRONT_DOWN_CORNER,
								LEFT_BACK_DOWN_CORNER,
								RIGHT_FRONT_DOWN_CORNER,
								RIGHT_BACK_DOWN_CORNER,
								LEFT_FRONT_UP_CORNER,
								LEFT_BACK_UP_CORNER,
								RIGHT_FRONT_UP_CORNER,
								RIGHT_BACK_UP_CORNER};
	return isPiecesSolved(corners, Cube());
}
