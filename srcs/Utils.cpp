#include "Utils.hpp"
#include <sstream>
#include <algorithm>

Permutation::Permutation(const std::string &before,
						 const std::string &after) : before(before),
													 after(after) {}

Permutation::Permutation() : before("LRUDFB"),
							 after("LRUDFB") {}

Permutation Permutation::operator*(const Permutation &other){
	auto nafter = after;
	for (auto &c : nafter){
		size_t i = other.before.find(c);
		if (i != std::string::npos)
			c = other.after[i];
	}
	return Permutation(before, nafter);
}

Replacement::Replacement(const std::string &rotationId,
						 const std::string &replacement,
						 const Permutation &perm) : rotationId(rotationId),
													replacement(replacement),
													perm(perm) {}

char Permutation::Permute(const char c) const{
	size_t i = before.find(c);
	if (i != std::string::npos)
		return after[i];
	return c;
}

std::string toFMC(const std::string &move){
	Permutation perm;
	std::vector<Replacement> repls;
	Permutation xPerm("LRUFDB",
					  "LRFDBU");
	Permutation xPrimePerm("LRFDBU",
						   "LRUFDB");
	Permutation yPerm("UDRBLF",
					  "UDBLFR");
	Permutation yPrimePerm("UDBLFR",
						   "UDRBLF");
	Permutation zPerm("FBRULD",
					  "FBULDR");
	Permutation zPrimePerm("FBULDR",
						   "FBRULD");
	repls.insert(repls.end(), {Replacement("L2'", "L2", perm),
							   Replacement("U2'", "U2", perm),
							   Replacement("F2'", "F2", perm),
							   Replacement("R2'", "R2", perm),
							   Replacement("E'", "U' D", perm),
							   Replacement("Fw'", "B'", zPrimePerm),
							   Replacement("Fw", "B", zPerm),
							   Replacement("Rw'", "L'", xPrimePerm),
							   Replacement("Rw", "L", xPerm),
							   Replacement("Lw'", "R'", xPerm),
							   Replacement("Lw", "R", xPrimePerm),
							   Replacement("x", "", xPerm),
							   Replacement("x'", "", xPrimePerm),
							   Replacement("y", "", yPerm),
							   Replacement("y2'", "", yPerm * yPerm),
							   Replacement("y2", "", yPerm * yPerm),
							   Replacement("y'", "", yPrimePerm),
							   Replacement("z", "", zPerm),
							   Replacement("z'", "", zPrimePerm),
							   Replacement("r", "L", xPerm),
							   Replacement("r2", "L2", xPerm * xPerm),
							   Replacement("r'", "L'", xPrimePerm),
							   Replacement("l", "R", xPrimePerm),
							   Replacement("l2'", "R2", xPerm * xPerm),
							   Replacement("l'", "R'", xPerm),
							   Replacement("d", "U", yPrimePerm),
							   Replacement("d'", "U'", yPerm),
							   Replacement("u", "D", yPerm),
							   Replacement("u'", "D'", yPrimePerm),
							   Replacement("u2", "D2", yPerm * yPerm),
							   Replacement("S", "F' B", zPerm),
							   Replacement("S'", "F B'", zPrimePerm),
							   Replacement("M", "L' R", xPrimePerm),
							   Replacement("M'", "L R'", xPerm),
							   Replacement("M2", "L2 R2", xPerm * xPerm)});
	std::stringstream ss;
	size_t pos = 0, pos1 = 0, cshpos = move.find('#');
	while ((pos = move.find_first_not_of(" ()[]{}", pos1)) != cshpos){
		ss << move.substr(pos1, pos - pos1);
		pos1 = move.find_first_of(" ()[]{}", pos);
		auto rId = move.substr(pos, pos1 - pos);
		auto ri = std::find_if(repls.begin(), repls.end(),
							   [rId](const Replacement &r)
							   { return r.rotationId == rId; });
		if (ri == repls.end())
			ss << perm.Permute(rId.front()) << rId.substr(1);
		else{
			ss << ri->replacement;
			perm = perm * ri->perm;
		}
	}
	if (pos1 != std::string::npos)
		ss << move.substr(pos1);
	return ss.str();
}
