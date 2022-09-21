#include <iostream>
#include <unistd.h>
#include "CubeSolver.hpp"

bool error(const std::string &errmsg){
	std::cerr << errmsg << '\n';
	return false;
}

bool getScramble(int ac, char **av, bool &explain, bool &graph, bool &cube2x2, int &scrlen, std::string &scramble){
	explain = false;
	graph = false;
	bool scrambleInArgs = false;
	int i = 0;
	bool terminal = isatty(STDIN_FILENO);
	while (++i < ac){
		std::string pstr(av[i]);
		if (pstr == "-g")
			graph = true;
		else if (pstr == "-e")
			explain = true;
		else if (pstr == "-2x2")
			cube2x2 = true;
		else if (pstr == "-s"){
			i++;
			if (i == ac)
				return error("Missed length of scramble");
			char *end;
			int d = std::strtol(av[i], &end, 10);
			if (end == av[i] || errno == ERANGE || d < 1 || d>100)
				return error("Incorrect length of scramble");
			scrlen = d;
			return true;
		}
		else
		{
			scrambleInArgs = true;
			scramble += (scramble.empty() ? "" : " ");
			scramble += av[i];
		}
	}
	if (!scrambleInArgs && terminal){
		std::cout << "Rubik's cube solver. Usage:\n\n"
				  << "./rubik [options] \"[scramble]\"\n\n"
				  << "   -e          explain moves\n"
				  << "   -g          graph display\n"
				  << "   -s LENGTH   generate scramble only, 0 < LENGTH <= 100\n"
				  << "   -2x2        solve scramble for 2x2 cube\n"
				  << "   scramble    [F|R|U|B|L|D|F'|R'|U'|B'|L'|D'|F2|R2|U2|B2|L2|D2]*\n\n"
				  << "Only face rotations, no slices. Scramble can be provided by input redirection.\n";
		return false;
	}
	if (!scrambleInArgs)
		std::getline(std::cin, scramble);
	if (!terminal && explain)
		std::cout << "Scramble passed: " << scramble << "\n";
	return true;
}

int main(int ac, char **av){
	bool explain = false, graph = false, cube2x2 = false;
	int scrlen = 0;
	std::string scramble;
	if (!getScramble(ac, av, explain, graph, cube2x2, scrlen, scramble))
		return 0;
	CubeSolver cs;
	if(scrlen>0){
		std::cout << cs.GenerateScramble(scrlen) << std::endl;
		return 0;
	}
	Cube cube, ref;
	try{
		cs.Scramble(cube, scramble);
	}catch (const std::exception &e){
		std::cerr << e.what() << std::endl;
		return 1;
	}
	auto solution = cs.Solve(cube, cube2x2);
	if(!(cube.isSolved() || (cube2x2 && cube.isSolved2x2()))){
		std::cerr << "Not solved!\n";
		return 1;
	}
	
	cube = ref;
	cs.Scramble(cube, scramble);
	if (explain && graph)
		std::cout << cs.cubeToString(cube, cube2x2);
	bool emptysol = true;
	for (auto move : solution){
		auto commentPos = move.find('#');
		std::string comment;
		if (commentPos != std::string::npos)
			comment = move.substr(commentPos);
		size_t start = move.find_first_not_of(' ');
		std::cout << move.substr(start, commentPos - start);
		cs.Scramble(cube, move);
		if (commentPos - start > 0)
			emptysol = false;
		if (explain)
			std::cout << (commentPos ? "\033[1;32m" : "\033[1;34m") << comment << "\033[0m\n";
		else if(commentPos==std::string::npos)
			std::cout << " ";
		if (explain && graph && commentPos > start)
			std::cout << cs.cubeToString(cube, cube2x2);
	}
	if (!explain && !emptysol)
		std::cout << std::endl;
}
