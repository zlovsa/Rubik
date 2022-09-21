#include <CubeSolver.hpp>
#include "Utils.hpp"

CubeSolver::CubeSolver(){
	initRotations();
	initF2Lmoves();
	initOLLmoves();
	initPLLmoves();
	initCLLmoves();
	initGraph();
}

CubeSolver::CubeSolver(const CubeSolver &other){
	if (this != &other)
		*this = other;
}

CubeSolver &CubeSolver::operator=(const CubeSolver &other){
	if (this != &other)
		rotations = other.rotations;
	return (*this);
}

CubeSolver::~CubeSolver() {}

void CubeSolver::initRotations(){
	rotations.clear();
	rotations.insert(rotations.end(), {Rotation("F", 1, -1, 1),
									   Rotation("R", 0, 1, -1),
									   Rotation("U", 2, 1, -1),
									   Rotation("L", 0, -1, 1),
									   Rotation("B", 1, 1, -1),
									   Rotation("D", 2, -1, 1),
									   Rotation("F2", 1, -1, 2),
									   Rotation("R2", 0, 1, 2),
									   Rotation("U2", 2, 1, 2),
									   Rotation("L2", 0, -1, 2),
									   Rotation("B2", 1, 1, 2),
									   Rotation("D2", 2, -1, 2),
									   Rotation("F'", 1, -1, -1),
									   Rotation("R'", 0, 1, 1),
									   Rotation("U'", 2, 1, 1),
									   Rotation("L'", 0, -1, -1),
									   Rotation("B'", 1, 1, 1),
									   Rotation("D'", 2, -1, -1)});
}

void CubeSolver::initF2Lmoves(){
	f2lmoves.clear();
	f2lmoves.insert(f2lmoves.end(), {"R U R' # Easy case",
									 "F' U' F # Easy case",
									 "U' F' U F # Easy case",
									 "U R U' R' # Easy case",
									 "(U R U' R') (U' F' U F) # Corner in bottom, edge in top layer",
									 "(U' F' U F) (U R U' R') # Corner in bottom, edge in top layer",
									 "(F' U F) (U' F' U F) # Corner in bottom, edge in top layer",
									 "(R U R') (U' R U R') # Corner in bottom, edge in top layer",
									 "(R U' R') (U R U' R') # Corner in bottom, edge in top layer",
									 "(F' U' F) (U F' U' F) # Corner in bottom, edge in top layer",
									 "(R U R' U') (R U R' U') (R U R') # Corner in top, edge in middle",
									 "(R U' R') (U F' U F) # Corner in top, edge in middle",
									 "(U F' U F) (U F' U2 F) # Corner in top, edge in middle",
									 "(U F' U' F) (U' R U R') # Corner in top, edge in middle",
									 "(U' R U' R') (U' R U2 R') # Corner in top, edge in middle",
									 "(U' R U R') (U F' U' F) # Corner in top, edge in middle",
									 "(R U' R' U) (U F' U' F) # Corner pointing outwards, edge in top layer",
									 "(F' U F U') (U' R U R') # Corner pointing outwards, edge in top layer",
									 "(U F' U2 F) (U F' U2 F) # Corner pointing outwards, edge in top layer",
									 "(U' R U2 R') (U' R U2 R') # Corner pointing outwards, edge in top layer",
									 "(U F' U' F) (U F' U2 F) # Corner pointing outwards, edge in top layer",
									 "(U' R U R') (U' R U2 R') # Corner pointing outwards, edge in top layer",
									 "(U' R U' R' U) (R U R') # Corner pointing outwards, edge in top layer",
									 "(U F' U F U') (F' U' F) # Corner pointing outwards, edge in top layer",
									 "(U' R U R' U) (R U R') # Corner pointing outwards, edge in top layer",
									 "(U F' U' F U') (F' U' F) # Corner pointing outwards, edge in top layer",
									 "(U F' U2 F U') (R U R') # Corner pointing outwards, edge in top layer",
									 "(U' R U2 R' U) (F' U' F) # Corner pointing outwards, edge in top layer",
									 "(R U R' U') U' (R U R' U') (R U R') # Corner pointing upwards, edge in top layer",
									 "(B' U' B U) U (B' U' B U) (B' U' B) # Corner pointing upwards, edge in top layer",
									 "(U2 R U R') (U R U' R') # Corner pointing upwards, edge in top layer",
									 "(U2 F' U' F) (U' F' U F) # Corner pointing upwards, edge in top layer",
									 "(U R U2 R') (U R U' R') # Corner pointing upwards, edge in top layer",
									 "(U' F' U2 F) (U' F' U F) # Corner pointing upwards, edge in top layer",
									 "(R U2 R') (U' R U R') # Corner pointing upwards, edge in top layer",
									 "(F' U2 F) (U F' U' F) # Corner pointing upwards, edge in top layer",
									 "(R U' R' U F' U2 F) (U F' U2 F) # Corner in bottom, edge in middle",
									 "(R U' R' U R U2 R') (U R U' R') # Corner in bottom, edge in middle",
									 "(R U' R' U' R U R') (U' R U2 R') # Corner in bottom, edge in middle",
									 "(R U R' U' R U' R') (U2 F' U' F) # Corner in bottom, edge in middle",
									 "(R U' R' U F' U' F) (U' F' U' F) # Corner in bottom, edge in middle"});
}

void CubeSolver::initOLLmoves(){
	ollmoves.clear();
	ollmoves.insert(ollmoves.end(), {"R U2 R2 F R F' U2 R' F R F' # Dot",
									 "r U r' U2 r U2 R' U2 R U' r' # Dot",
									 "r' R2 U R' U r U2 r' U M' # Dot",
									 "M U' r U2 r' U' R U' R' M' # Dot",
									 "l' U2 L U L' U l # Square Shape",
									 "r U2 R' U' R U' r' # Square Shape",
									 "r U R' U R U2 r' # Small Lightning Bolt",
									 "l' U' L U' L' U2 l # Small Lightning Bolt",
									 "R U R' U' R' F R2 U R' U' F' # Fish Shape",
									 "R U R' U R' F R F' R U2 R' # Fish Shape",
									 "r U R' U R' F R F' R U2 r' # Small Lightning Bolt",
									 "M' R' U' R U' R' U2 R U' R r' # Small Lightning Bolt",
									 "F U R U' R2 F' R U R U' R' # Knight Move Shape",
									 "R' F R U R' F' R F U' F' # Knight Move Shape",
									 "l' U' l L' U' L U l' U l # Knight Move Shape",
									 "r U r' R U R' U' r U' r' # Knight Move Shape",
									 "F R' F' R2 r' U R U' R' U' M' # Dot",
									 "r U R' U R U2 r2 U' R U' R' U2 r # Dot",
									 "r' R U R U R' U' M' R' F R F' # Dot",
									 "r U R' U' M2 U R U' R' U' M' # Dot",
									 "R U2 R' U' R U R' U' R U' R' # Cross",
									 "R U2 R2 U' R2 U' R2 U2 R # Cross",
									 "R2 D' R U2 R' D R U2 R # Cross",
									 "r U R' U' r' F R F' # Cross",
									 "F' r U R' U' r' F R # Cross",
									 "R U2 R' U' R U' R' # Cross",
									 "R U R' U R U2 R' # Cross",
									 "r U R' U' r' R U R U' R' # Corners Oriented",
									 "R U R' U' R U' R' F' U' F R U R' # Awkward Shape",
									 "F R' F R2 U' R' U' R U R' F2 # Awkward Shape",
									 "R' U' F U R U' R' F' R # P Shape",
									 "L U F' U' L' U L F L' # P Shape",
									 "R U R' U' R' F R F' # T Shape",
									 "R U R2 U' R' F R U R U' F' # C Shape",
									 "R U2 R2 F R F' R U2 R' # Fish Shape",
									 "L' U' L U' L' U L U L F' L' F # W Shape",
									 "F R' F' R U R U' R' # Fish Shape",
									 "R U R' U R U' R' U' R' F R F' # W Shape",
									 "L F' L' U' L U F U' L' # Big Lightning Bolt",
									 "R' F R U R' U' F' U R # Big Lightning Bolt",
									 "R U R' U R U2 R' F R U R' U' F' # Awkward Shape",
									 "R' U' R U' R' U2 R F R U R' U' F' # Awkward Shape",
									 "F' U' L' U L F # P Shape",
									 "F U R U' R' F' # P Shape",
									 "F R U R' U' F' # T Shape",
									 "R' U' R' F R F' U R # C Shape",
									 "R' U' R' F R F' R' F R F' U R # Small L Shape",
									 "F R U R' U' R U R' U' F' # Small L Shape",
									 "r U' r2 U r2 U r2 U' r # Small L Shape",
									 "r' U r2 U' r2 U' r2 U r' # Small L Shape",
									 "F U R U' R' U R U' R' F' # I Shape",
									 "R U R' U R U' B U' B' R' # I Shape",
									 "l' U2 L U L' U' L U L' U l # Small L Shape",
									 "r U2 R' U' R U R' U' R U' r' # Small L Shape",
									 "R' F R U R U' R2 F' R2 U' R' U R U R' # I Shape",
									 "r' U' r U' R' U R U' R' U R r' U r # I Shape",
									 "R U R' U' M' U R U' r' # Corners Oriented"});
	// Cube cube;
	for (auto &move : ollmoves)
	{
		// std::cout << move << "  ==>  ";
		move = toFMC(move);
		// std::cout << move << std::endl;
		// Scramble(cube, move);
	}
}

void CubeSolver::initPLLmoves(){
	pllmoves.clear();
	pllmoves.insert(pllmoves.end(), {"x L2 D2 L' U' L D2 L' U L' # Adjacent Corner Swap",
									 "x' L2 D2 L U L' D2 L U' L # Adjacent Corner Swap",
									 "R' U' F' R U R' U' R' F R2 U' R' U' R U R' U R # Adjacent Corner Swap",
									 "R2 U R' U R' U' R U' R2 U' D R' U R D' # Adjacent Corner Swap",
									 "R' U' R U D' R2 U R' U R U' R U' R2 D # Adjacent Corner Swap",
									 "R2 U' R U' R U R' U R2 U D' R U' R' D # Adjacent Corner Swap",
									 "R U R' U' D R2 U' R U' R' U R' U R2 D' # Adjacent Corner Swap",
									 "x R2 F R F' R U2 r' U r U2 # Adjacent Corner Swap",
									 "R U R' F' R U R' U' R' F R2 U' R' # Adjacent Corner Swap",
									 "R U' R' U' R U R D R' U' R D' R' U2 R' # Adjacent Corner Swap",
									 "R2 F R U R U' R' F' R U2 R' U2 R # Adjacent Corner Swap",
									 "R U R' U' R' F R2 U' R' U' R U R' F' # Adjacent Corner Swap",
									 "x' L' U L D' L' U' L D L' U' L D' L' U L D # Diagonal Corner Swap",
									 "R U R' U R U R' F' R U R' U' R' F R2 U' R' U2 R U' R' # Diagonal Corner Swap",
									 "R' U R U' R' F' U' F R U R' F R' F' R U' R # Diagonal Corner Swap",
									 "R' U R' U' y R' F' R2 U' R' U R' F R F # Diagonal Corner Swap",
									 "F R U' R' U' R U R' F' R U R' U' R' F R F' # Diagonal Corner Swap",
									 "M2 U M2 U2 M2 U M2 # Edges Only",
									 "M2 U M U2 M' U M2 # Edges Only",
									 "M2 U' M U2 M' U' M2 # Edges Only",
									 "M' U M2 U M2 U M' U2 M2 # Edges Only"});
	// Cube t;
	for (auto &move : pllmoves)
	{
		// std::cout << move << "  ==>  ";
		move = toFMC(move);
		// std::cout << move << std::endl;
		// Scramble(t, move);
	}
}

void CubeSolver::initCLLmoves(){
	cllmoves.clear();
	cllmoves.insert(cllmoves.end(), {"L' U' L U' L' U2 L # Antisune AS1",
									 "L' U' L U L F' L' F L' U' L U' L' U2 L # Antisune AS2",
									 "F' L F L' U2 L' U2 L # Antisune AS3",
									 "R U2 R' U2 R' F R F' # Antisune AS4",
									 "R' F R F' R U R' # Antisune AS5",
									 "R' U R U' R2 F R F' R U R' U' R # Antisune AS6",
									 "R2 U2 R' U2 R2 # H H1",
									 "F R2 U' R2 U' R2 U R2 F' # H H2",
									 "R U R' U R U L' U R' U' L # H H3",
									 "x' U2 R U2 R2 F2 R U2 x # H H4",
									 "R' U R' U2 R U' R' U R U' R2 # L L1",
									 "R U2 R2 F2 R U R' F2 R F' # L L2",
									 "R U2 R' F' R U2 R' U R' F2 R # L L3",
									 "F R' F' R U R U' R' # L L4",
									 "F R U' R' U' R U R' F' # L L5",
									 "R U2 R2 F R F' R U2 R' # L L6",
									 "R U R' U' R' F R2 U' R' U' R U R' F' # PLL Adjacent",
									 "F R U' R' U' R U R' F' R U R' U' R' F R F' # PLL Diagonal",
									 "R U' R2 U R2 U R2 U' R # Pi P1",
									 "R' F2 R F' U2 R U' R' U' F # Pi P2",
									 "F R2 U' R2 U R2 U R2 F' # Pi P3",
									 "R U2 R' U' R U R' U2 R' F R F' # Pi P4",
									 "R U' R' F R' F R U R' F R # Pi P5",
									 "R' U' R' F R F' R U' R' U2 R # Pi P6",
									 "R U R' U R U2 R' # Sune S1",
									 "L' U2 L U2 L F' L' F # Sune S2",
									 "F R' F' R U2 R U2 R' # Sune S3",
									 "R U R' U' R' F R F' R U R' U R U2 R' # Sune S4",
									 "L F' L' F L' U' L # Sune S5",
									 "R U R' U R' F R F' R U2 R' # Sune S6",
									 "R' U R U2 R2 F' R U' R' F2 R2 # T T1",
									 "R' U R U2 R2 F R F' R # T T2",
									 "L' U' L U L F' L' F # T T3",
									 "F R U R' U' R U' R' U' R U R' F' # T T4",
									 "R U R' U' R' F R F' # T T5",
									 "F U' R U2 R' U' F2 R U R' # T T6",
									 "R2 F2 R U R' F R2 U2 R' U' R # U U1",
									 "R U2 R U' R' F R' F2 U' F # U U2",
									 "R' U R' F R F' R U2 R' U R # U U3",
									 "F R' F' R U' R U' R' U2 R U' R' # U U4",
									 "R U' R2 F R F' R U R' U' R U R' # U U5",
									 "F R U R' U' F' # U U6"});
	// Cube t;
	for (auto &move : cllmoves)
	{
		// std::cout << move << "  ==>  ";
		move = toFMC(move);
		// std::cout << move << std::endl;
		// Scramble(t, move);
	}
}

void CubeSolver::initGraph(){
	str3x3 =
		"        jjj                               KKK        \n"
		"    kkk                                       LLL    \n"
		"lll     mmm              aaa              NNN     MMM\n"
		"    nnn              ddd     bbb              OOO    \n"
		"ooo     ppp      ggg     eee     ccc      QQQ     PPP\n"
		"    qqq              hhh     fff              RRR    \n"
		"rrr           sss        iii        DDD           SSS\n"
		"                  ttt           CCC                  \n"
		"              vvv     uuu   BBB     GGG              \n"
		"                  www           FFF                  \n"
		"              yyy     xxx   EEE     JJJ              \n"
		"                  zzz           III                  \n"
		"                      AAA   HHH                      \n"
		"                                                     \n"
		"                         TTT                         \n"
		"                     WWW     UUU                     \n"
		"                 ZZZ     XXX     VVV                 \n"
		"                     111     YYY                     \n"
		"                         222                         \n";
	str2x2 =
		"    jjj                    KKK    \n"
		"lll            aaa             MMM\n"
		"    ppp    ggg     ccc     QQQ    \n"
		"rrr            iii             SSS\n"
		"        sss           DDD         \n"
		"            uuu   BBB             \n"
		"        yyy           JJJ         \n"
		"            AAA   HHH             \n"
		"                                  \n"
		"               TTT                \n"
		"           ZZZ     VVV            \n"
		"               222                \n";
}
