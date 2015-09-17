#ifndef __BGM_FASTA_H__
#define __BGM_FASTA_H__

#include <vector>

namespace BGM
{
	class Fasta
	{
	private:
		std::vector<char> DNA;

	public:
		Fasta();
		~Fasta();
		void readFile(const char *fileName);
		const std::vector<char>& getDNA() const;
	};
}

#endif
