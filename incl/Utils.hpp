#include <string>
#include <vector>

struct Permutation{
	std::string before, after;
	Permutation();
	Permutation(const std::string &before, const std::string &after);
	Permutation operator*(const Permutation &other);
	char Permute(const char c) const;
};

struct Replacement{
	Replacement(const std::string &rotationId,
				const std::string &replacement,
				const Permutation &perm);
	std::string rotationId, replacement;
	Permutation perm;
};

std::string toFMC(const std::string &move);
