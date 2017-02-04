#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class RangeQuery_V1
{
	
	int32_t time1;
	int32_t time2;
	string page;
	
public:

	
	RangeQuery_V1(int32_t t1, int32_t t2, const string & p)
	{
		this->time1 = t1;
		this->time2 = t2;
		this->page = p;
	}
	
	void performQuery(const vector<vector<pair<string, string>>> & compactStructure) const
	{
		uint32_t begin = time1 >= 0 ? time1 : 0;
		uint32_t size = compactStructure.size();
		uint32_t end = (time2 + 1) < size ? (time2 + 1) : size;
		vector<pair<string, string>> sameDateListOfPair;
		ofstream out("result_001.txt", ofstream::app);
		out << ":::" << page << ":::" << endl;
		for (int i = begin; i < end; i++)
		{
			sameDateListOfPair = ref(compactStructure[i]);
			uint32_t j = 0;
			uint32_t dim = sameDateListOfPair.size();
			while (j < dim)
			{
				if (sameDateListOfPair[j].first == page)
				{
					out << "Date: " << i << " Counter: " << sameDateListOfPair[j].second <<endl;
					break;
				}
				else j++;
			}
		}
			
	}

	~RangeQuery_V1(void)
	{     
	}


};