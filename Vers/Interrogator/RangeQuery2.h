#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class RangeQuery_V2
{
	
	int32_t time1;
	int32_t time2;
	string page;
	
public:

	
	RangeQuery_V2(int32_t t1, int32_t t2, const string & p)
	{
		this->time1 = t1;
		this->time2 = t2;
		this->page = p;
	}
	
	void performQuery(const vector<string> & compactStructure, const vector<vector<int32_t>> compactPositions) const
	{
		uint32_t begin = time1 >= 0 ? time1 : 0;
		uint32_t size = compactStructure.size();
		uint32_t end = (time2 + 1) < size ? (time2 + 1) : size;
		string sameDateListOfPair;
		vector<int32_t> sameDatePositions;
		
		ofstream out("result_002.txt", ofstream::app);
		out << ":::" << page << ":::" << endl;
		for (int i = begin; i < end; i++)
		{	
			sameDateListOfPair = ref(compactStructure[i]);
			sameDatePositions = ref(compactPositions[i]);
			uint32_t j = 0;
			uint32_t dim = sameDatePositions.size();
			if (dim == 0 || sameDateListOfPair.length() == 0) continue;
			while (j < dim - 2)
			{
				int32_t pagStart = sameDatePositions[j];
				int32_t pagEndCountStart = sameDatePositions[j+1];
				int32_t pagLength = pagEndCountStart - pagStart;
				int32_t countEnd = sameDatePositions[j+2];
				int32_t countLength = countEnd - pagEndCountStart;
						if (sameDateListOfPair.length() == 0) cout << sameDateListOfPair.length() << " at j: " << j << " of dim: " << dim << endl;
				if (sameDateListOfPair.substr(pagStart, pagLength) == page)
				{
					out << "Date: " << i << " Counter: " << sameDateListOfPair.substr(pagEndCountStart, countLength) <<endl;
					break;
				}
				else j+=2;
			}
		}
			
	}

	~RangeQuery_V2(void)
	{     
	}


};