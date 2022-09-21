#include <CubeSolver.hpp>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <random>

Rotation::Rotation(std::string id, int dimension, int coordinate, int angle) : id(id),
																			   dimension(dimension),
																			   coordinate(coordinate),
																			   angle(angle){
	int sin[4] = {-1, 0, 1, 0};
	int cos[4] = {0, 1, 0, -1};
	m[dimension][dimension] = 1;
	for (int i = dimension + 1; i <= dimension + 2; i++){
		m[i % 3][dimension] = 0;
		m[dimension][i % 3] = 0;
		m[i % 3][i % 3] = cos[angle + 1];
	}
	m[(dimension + 1) % 3][(dimension + 2) % 3] = -sin[angle + 1];
	m[(dimension + 2) % 3][(dimension + 1) % 3] = sin[angle + 1];
}

void CubeSolver::Rotate(Cube &cube, const Rotation &rotation) const{
	for (auto &p : cube.pieces)
		if (p.coord[rotation.dimension] == rotation.coordinate){
			int ncoord[3];
			for (int i = 0; i < 3; i++){
				ncoord[i] = 0;
				for (int j = 0; j < 3; j++)
					ncoord[i] += rotation.m[i][j] * p.coord[j];
			}
			for (int i = 0; i < 3; i++)
				p.coord[i] = ncoord[i];
			if (rotation.angle % 2)
				std::swap(p.color[(rotation.dimension + 1) % 3],
						  p.color[(rotation.dimension + 2) % 3]);
		}
}

void CubeSolver::Rotate(Cube &cube, const std::string &rotationId) const{
	auto ri = std::find_if(rotations.begin(), rotations.end(),
						   [rotationId](const Rotation &r)
						   { return r.id == rotationId; });
	if (ri == rotations.end())
		throw std::runtime_error("Unknown rotation: " + rotationId);
	Rotate(cube, *ri);
}

void CubeSolver::Scramble(Cube &cube, const std::string &scramble) const{
	size_t pos = 0, pos1 = 0, cshpos = scramble.find('#');
	while ((pos = scramble.find_first_not_of(" ()[]{}", pos1)) != cshpos){
		pos1 = scramble.find_first_of(" ()[]{}", pos);
		std::string rotationId = scramble.substr(pos, pos1 - pos);
		Rotate(cube, rotationId);
	}
}

void CubeSolver::Scramble(Cube &cube, const std::vector<int> &scramble, const std::vector<std::string> &moves) const{
	for (auto moveId : scramble)
		Scramble(cube, moves[moveId]);
}

void next(std::vector<int> &a, int max, int lastMoveMax = -1){
	size_t i = 0;
	while (i < a.size() && ++a[i] == max)
		a[i++] = 0;
	if (i == a.size())
		a.push_back(lastMoveMax > 0 ? max : 0);
	if (a.back() == lastMoveMax)
		a.back() = 0;
}

bool find_first_rotation(std::vector<std::string> &moves, size_t &move_idx, size_t &pos){
	move_idx = 0;
	for (auto &move : moves){
		size_t cshpos = move.find('#');
		if ((pos = move.find_first_not_of(" ()[]{}", 0)) != cshpos)
			return true;
		move_idx++;
	}
	return false;
}

bool find_last_rotation(std::vector<std::string> &moves, size_t &move_idx, size_t &pos){
	if (moves.size() == 0)
		return false;
	for (move_idx = moves.size(); move_idx > 0; move_idx--){
		std::string &move = moves[move_idx - 1];
		size_t cshpos = move.find('#');
		if ((pos = move.find_last_not_of(" ()[]{}#", cshpos)) != std::string::npos){
			move_idx--;
			pos = move.find_last_of(" ()[]{}", pos - 1);
			if(pos==std::string::npos)
				pos = 0;
			else
				pos++;
			return true;
		}
	}
	return false;
}

void replace_rotation(std::string &move, size_t pos, const std::string &rotation){
	size_t pos1 = move.find_first_of(" ()[]{}", pos);
	while (!rotation.length() && pos > 0 && move[pos - 1] == ' ')
		pos--;
	move.erase(pos, pos1 - pos);
	move.insert(pos, rotation);
}

int rotation_angle(const std::string &move, size_t pos){
	int angle = 1;
	if (move.find_first_of("'2", pos) == pos + 1)
		angle = move.at(pos + 1) == '2' ? 2 : -1;
	return angle;
}

void append(std::vector<std::string> &dest, std::vector<std::string> &src){
	size_t first_idx, first_pos, last_idx, last_pos;
	int first_angle, last_angle;
	while (find_first_rotation(src, first_idx, first_pos) &&
		   find_last_rotation(dest, last_idx, last_pos) &&
		   dest[last_idx].at(last_pos) == src[first_idx].at(first_pos)){
		last_angle = rotation_angle(dest[last_idx], last_pos);
		first_angle = rotation_angle(src[first_idx], first_pos);
		int angle = (last_angle + first_angle + 4) % 4;
		std::string nmove;
		if(angle){
			nmove += src[first_idx].at(first_pos);
			if(angle>1)
				nmove += angle == 2 ? "2" : "'";
		}
		replace_rotation(dest[last_idx], last_pos, nmove);
		replace_rotation(src[first_idx], first_pos, "");
	}
	dest.insert(dest.end(), src.begin(), src.end());
}

void append(std::vector<std::string> &dest, const std::vector<std::string> &src){
	dest.insert(dest.end(), src.begin(), src.end());
}

std::string join(std::vector<std::string> move, std::string comment){
	std::string str;
	for (auto m : move)
		str += m + " ";
	str += comment;
	return str;
}

std::vector<std::string> CubeSolver::Solve(const std::string &scramble, bool cube2x2) const{
	Cube cube;
	Scramble(cube, scramble);
	return Solve(cube, cube2x2);
}

std::vector<std::string> CubeSolver::WhiteCross(Cube &cube, std::vector<int> &solvedPiecesIdcs) const{
	std::vector<std::string> moves;
	for (auto r : rotations)
		moves.push_back(r.id);
	std::vector<std::string> sol;
	solvedPiecesIdcs.push_back(FRONT_DOWN_EDGE);
	auto wgsol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, -1);
	solvedPiecesIdcs.push_back(RIGHT_DOWN_EDGE);
	auto wrsol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, -1);
	solvedPiecesIdcs.push_back(BACK_DOWN_EDGE);
	auto wbsol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, -1);
	solvedPiecesIdcs.push_back(LEFT_DOWN_EDGE);
	auto wosol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, -1);
	if (wgsol.size() > 0 || wrsol.size() > 0 || wbsol.size() > 0 || wosol.size() > 0){
		sol.push_back("#\nWHITE CROSS:");
		if (wgsol.size() > 0)
			sol.push_back(join(wgsol, "# White-Blue edge"));
		if (wrsol.size() > 0)
			sol.push_back(join(wrsol, "# White-Red edge"));
		if (wbsol.size() > 0)
			sol.push_back(join(wbsol, "# White-Green edge"));
		if (wosol.size() > 0)
			sol.push_back(join(wosol, "# White-Orange edge"));
	}else
		sol.push_back("#\nWHITE CROSS already solved!!!");
	return sol;
}

std::string &rotY(std::string &move){
	std::string from = "FRBL", to = "RBLF";
	for (auto &c : move){
		if (c == '#')
			break;
		size_t i = from.find(c);
		if (i < 4)
			c = to[i];
	}
	return move;
}

std::vector<std::string> CubeSolver::FirstTwoLayers(Cube &cube, std::vector<int> &solvedPiecesIdcs) const{
	std::vector<std::string> moves;
	std::string upMove = "R U R' # Getting corner/edge to top layer";
	moves.push_back(upMove);
	moves.push_back(rotY(upMove));
	moves.push_back(rotY(upMove));
	moves.push_back(rotY(upMove));
	moves.push_back("U # Rotate top layer to fit F2L algorithm");
	moves.push_back("U' # Rotate top layer to fit F2L algorithm");
	moves.push_back("U2 # Rotate top layer to fit F2L algorithm");
	append(moves, f2lmoves);
	std::vector<std::string> sol;
	solvedPiecesIdcs.push_back(RIGHT_FRONT_DOWN_CORNER);
	solvedPiecesIdcs.push_back(RIGHT_FRONT_EDGE);
	auto grsol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, 7);
	for (auto &move : moves)
		rotY(move);
	solvedPiecesIdcs.push_back(RIGHT_BACK_DOWN_CORNER);
	solvedPiecesIdcs.push_back(RIGHT_BACK_EDGE);
	auto brsol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, 7);
	for (auto &move : moves)
		rotY(move);
	solvedPiecesIdcs.push_back(LEFT_BACK_DOWN_CORNER);
	solvedPiecesIdcs.push_back(LEFT_BACK_EDGE);
	auto bosol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, 7);
	for (auto &move : moves)
		rotY(move);
	solvedPiecesIdcs.push_back(LEFT_FRONT_DOWN_CORNER);
	solvedPiecesIdcs.push_back(LEFT_FRONT_EDGE);
	auto gosol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, 7);
	if (grsol.size() > 0 || brsol.size() > 0 || bosol.size() > 0 || gosol.size() > 0){
		sol.push_back("#\nF2L FIRST TWO LAYERS:");
		grsol.size() > 0 ? sol.push_back(" # Blue-Red slot:") : (void)0;
		grsol.size() > 0 ? append(sol, grsol) : (void)0;
		brsol.size() > 0 ? sol.push_back(" # Green-Red slot:") : (void)0;
		brsol.size() > 0 ? append(sol, brsol) : (void)0;
		bosol.size() > 0 ? sol.push_back(" # Green-Orange slot:") : (void)0;
		bosol.size() > 0 ? append(sol, bosol) :(void)0;
		gosol.size() > 0 ? sol.push_back(" # Blue-Orange slot:") : (void)0;
		gosol.size() > 0 ? append(sol, gosol) : (void)0;
	}else
		sol.push_back("#\nF2L FIRST TWO LAYERS alredy solved!!!:");
	return sol;
}

std::vector<std::string> CubeSolver::OrientLastLayer(Cube &cube, std::vector<int> &solvedPiecesIdcs) const{
	std::vector<std::string> moves;
	moves.push_back("U # Rotate top layer to fit OLL algorithm");
	moves.push_back("U' # Rotate top layer to fit OLL algorithm");
	moves.push_back("U2 # Rotate top layer to fit OLL algorithm");
	append(moves, ollmoves);
	auto sol = SolvePieces(cube, solvedPiecesIdcs, moves, 2, 3, &Cube::isOLL);
	if (sol.empty())
		sol.push_back("#\nOLL ORIENT LAST LAYER - already oriented!!!");
	else
		sol.insert(sol.begin(), "#\nOLL ORIENT LAST LAYER:");
	return sol;
}

std::vector<std::string> CubeSolver::PermutateLastLayer(Cube &cube, std::vector<int> &solvedPiecesIdcs) const{
	std::vector<std::string> moves;
	moves.push_back("U # Rotate top layer to better fit sides");
	moves.push_back("U' # Rotate top layer to better fit sides");
	moves.push_back("U2 # Rotate top layer to better fit sides");
	append(moves, pllmoves);
	for (int i = 0; i < 3;i++){
		for (auto &move : moves)
			rotY(move);
		append(moves, pllmoves);
	}
	solvedPiecesIdcs.push_back(RIGHT_FRONT_UP_CORNER);
	solvedPiecesIdcs.push_back(RIGHT_UP_EDGE);
	solvedPiecesIdcs.push_back(RIGHT_BACK_UP_CORNER);
	solvedPiecesIdcs.push_back(BACK_UP_EDGE);
	solvedPiecesIdcs.push_back(LEFT_BACK_UP_CORNER);
	solvedPiecesIdcs.push_back(LEFT_UP_EDGE);
	solvedPiecesIdcs.push_back(LEFT_FRONT_UP_CORNER);
	solvedPiecesIdcs.push_back(FRONT_UP_EDGE);
	auto sol = SolvePieces(cube, solvedPiecesIdcs, moves, 2, 3);
	if (sol.empty())
		sol.push_back("#\nPLL PERMUTATE LAST LAYER - already permutated!!!");
	else
		sol.insert(sol.begin(), "#\nPLL PERMUTATE LAST LAYER:");
	return sol;
}

std::vector<std::string> CubeSolver::SolvePieces(Cube &cube,
												 const std::vector<int> &pieceIdcs,
												 const std::vector<std::string> &moves,
												 size_t maxMoves,
												 int lastMoveFrom,
												 bool (Cube::*cond)(const Cube &) const) const{
	std::vector<int> solution;
	Cube t = cube;
	while (!(t.isPiecesSolved(pieceIdcs, reference) && (!cond || (t.*cond)(reference))) &&
		   solution.size() <= maxMoves){
		t = cube;
		next(solution, lastMoveFrom > -1 ? lastMoveFrom : moves.size(), moves.size());
		Scramble(t, solution, moves);
	}
	cube = t;
	return VerboseSolution(solution, moves);
}

std::vector<std::string> CubeSolver::Solve(Cube &cube, bool cube2x2) const{
	if (cube2x2)
		return Solve2x2(cube);
	std::vector<int> solvedPiecesIdcs;
	auto sol = WhiteCross(cube, solvedPiecesIdcs);
	auto tsol = FirstTwoLayers(cube, solvedPiecesIdcs);
	append(sol, tsol);
	tsol = OrientLastLayer(cube, solvedPiecesIdcs);
	append(sol, tsol);
	tsol = PermutateLastLayer(cube, solvedPiecesIdcs);
	append(sol, tsol);
	return sol;
}

std::vector<std::string> CubeSolver::CornersFirstLayer(Cube &cube, std::vector<int> &solvedPiecesIdcs) const{
	std::vector<std::string> moves;
	std::string upMove = "R U R' # White facing right / getting corner up";
	moves.push_back(upMove);
	moves.push_back(rotY(upMove));
	moves.push_back(rotY(upMove));
	moves.push_back(rotY(upMove));
	upMove = "R U2 R' # White facing right / getting corner up";
	moves.push_back(upMove);
	moves.push_back(rotY(upMove));
	moves.push_back(rotY(upMove));
	moves.push_back(rotY(upMove));
	moves.push_back("U # Rotate top layer to fit algorithm");
	moves.push_back("U' # Rotate top layer to fit algorithm");
	moves.push_back("U2 # Rotate top layer to fit algorithm");
	moves.push_back("U R U' R' # White facing front");
	moves.push_back("U R2 U' R2 # White facing upward");
	std::vector<std::string> sol;
	solvedPiecesIdcs.push_back(RIGHT_FRONT_DOWN_CORNER);
	auto rfsol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, -1);
	for (auto &move : moves)
		rotY(move);
	solvedPiecesIdcs.push_back(RIGHT_BACK_DOWN_CORNER);
	auto rbsol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, -1);
	for (auto &move : moves)
		rotY(move);
	solvedPiecesIdcs.push_back(LEFT_BACK_DOWN_CORNER);
	auto lbsol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, -1);
	for (auto &move : moves)
		rotY(move);
	solvedPiecesIdcs.push_back(LEFT_FRONT_DOWN_CORNER);
	auto lfsol = SolvePieces(cube, solvedPiecesIdcs, moves, 4, -1);
	if (rfsol.size() > 0 || rbsol.size() > 0 || lbsol.size() > 0 || lfsol.size() > 0){
		sol.push_back("#\nCFL CORNERS FIRST LAYER:");
		rfsol.size() > 0 ? sol.push_back(" # Blue-Red slot:") : (void)0;
		rfsol.size() > 0 ? append(sol, rfsol) : (void)0;
		rbsol.size() > 0 ? sol.push_back(" # Green-Red slot:") : (void)0;
		rbsol.size() > 0 ? append(sol, rbsol) : (void)0;
		lbsol.size() > 0 ? sol.push_back(" # Green-Orange slot:") : (void)0;
		lbsol.size() > 0 ? append(sol, lbsol) :(void)0;
		lfsol.size() > 0 ? sol.push_back(" # Blue-Orange slot:") : (void)0;
		lfsol.size() > 0 ? append(sol, lfsol) : (void)0;
	}else
		sol.push_back("#\nCFL CORNERS FIRST LAYER alredy solved!!!:");
	return sol;
}

std::vector<std::string> CubeSolver::CornersLastLayer(Cube &cube, std::vector<int> &solvedPiecesIdcs) const{
	std::vector<std::string> moves;
	moves.push_back("U # Rotate top layer to fit sides");
	moves.push_back("U' # Rotate top layer to fit sides");
	moves.push_back("U2 # Rotate top layer to fit sides");
	append(moves, cllmoves);
	for (int i = 0; i < 3; i++){
		for (auto &move : moves)
			rotY(move);
		append(moves, cllmoves);
	}
	moves.push_back("U # Rotate top layer to fit sides");
	moves.push_back("U' # Rotate top layer to fit sides");
	moves.push_back("U2 # Rotate top layer to fit sides");
	solvedPiecesIdcs.push_back(RIGHT_FRONT_UP_CORNER);
	solvedPiecesIdcs.push_back(RIGHT_BACK_UP_CORNER);
	solvedPiecesIdcs.push_back(LEFT_BACK_UP_CORNER);
	solvedPiecesIdcs.push_back(LEFT_FRONT_UP_CORNER);
	auto sol = SolvePieces(cube, solvedPiecesIdcs, moves, 3, 3);
	if (sol.empty())
		sol.push_back("#\nCLL CORNERS OF THE LAST LAYER - already done!!!");
	else
		sol.insert(sol.begin(), "#\nCLL CORNERS OF THE LAST LAYER:");
	return sol;
}

std::vector<std::string> CubeSolver::Solve2x2(Cube &cube) const{
	std::vector<int> solvedPiecesIdcs;
	auto sol = CornersFirstLayer(cube, solvedPiecesIdcs);
	auto tsol = CornersLastLayer(cube, solvedPiecesIdcs);
	append(sol, tsol);
	return sol;
}

std::vector<std::string> CubeSolver::VerboseSolution(const std::vector<int> &solution, const std::vector<std::string> &moves) const{
	std::vector<std::string> solutionStr;
	for (auto moveId : solution)
		solutionStr.push_back(moves[moveId]);
	return solutionStr;
}

std::string CubeSolver::GenerateScramble(int ScrambleLength) const{
	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_int_distribution<int> distr(0, rotations.size() - 1);
	std::string scramble;
	char lastFace = ' ';
	while (ScrambleLength--){
		std::string next;
		do
			next = rotations[distr(generator)].id;
		while (next.front() == lastFace);
		lastFace = next.front();
		scramble += (scramble.empty() ? "" : " ") + next;
	}
	return scramble;
}

std::string CubeSolver::cubeToString(const Cube &cube, bool cube2x2) const{
	return cube.toString(cube2x2 ? str2x2 : str3x3);
}
