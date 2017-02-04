#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

class RangeQuery_V3
{
	
	int32_t time1;
	int32_t time2;
	string page;
	
public:

	
	RangeQuery_V3(int32_t t1, int32_t t2, const string & p)
	{
		this->time1 = t1;
		this->time2 = t2;
		this->page = p;
	}
	
	void performQuery(const vector<string> & compactStructure, const vector<rrr_vector<>> compactPositions) const
	{
		uint32_t begin = time1 >= 0 ? time1 : 0;
		uint32_t size = compactStructure.size();
		uint32_t end = (time2 + 1) < size ? (time2 + 1) : size;
		string sameDateListOfPair;
		rrr_vector<> sameDatePositions;
		
		ofstream out("result_003.txt", ofstream::app);
		out << ":::" << page << ":::" << endl;
		for (int i = begin; i < end; i++)
		{	
			sameDateListOfPair = ref(compactStructure[i]);
			sameDatePositions = ref(compactPositions[i]);
			uint32_t j = 0;
			uint32_t dim = sameDatePositions.size();
			if (dim == 0 || sameDateListOfPair.length() == 0) continue;
			while (j < dim - 64)
			{
				int32_t pagStart = sameDatePositions.get_int(j,32);
				int32_t pagEndCountStart = sameDatePositions.get_int(j+32,32);
				int32_t pagLength = pagEndCountStart - pagStart;
				int32_t countEnd = sameDatePositions.get_int(j+64,32);
				int32_t countLength = countEnd - pagEndCountStart;
										
				
				if (sameDateListOfPair.substr(pagStart, pagLength) == page)
				{
					string counter = sameDateListOfPair.substr(pagEndCountStart, countLength);
					out << "Date: " << i << " Counter: " << counter <<endl;
					break;
				}
				else j+=64;
			}
		}
			
	}

	~RangeQuery_V3(void)
	{     
	}


};