#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>


using namespace std;
using namespace sdsl;

struct compare_V3  
{  
	bool operator()(const pair<int32_t, uint32_t> & l, const  pair<int32_t, uint32_t> & r)  
	{  
		return l.second > r.second;  
	}  
};

class TopkQuery_V3
{
	
	int32_t time1;
	int32_t time2;
	string page;
	uint32_t K;
	
public:
	
	
	TopkQuery_V3(int32_t t1, int32_t t2, const string & p, uint32_t K)
	{
		this->time1 = t1;
		this->time2 = t2;
		this->page = p;
		this->K = K;
		
	}
	
	void performQuery(const vector<string> & compactStructure, const vector<rrr_vector<>> & compactPositions) 
	{
		uint32_t begin = time1 >= 0 ? time1 : 0;
		uint32_t size = compactStructure.size();
		uint32_t end = (time2 + 1) < size ? (time2 + 1) : size;
		string sameDateListOfPair;
		rrr_vector<> sameDatePositions;
		
		priority_queue<pair<int32_t, uint32_t>,vector<pair<int32_t, uint32_t>>,compare_V3> topK;
		
		ofstream out("result_003.txt", ofstream::app);
		out << ":::" << page << ":::" << endl;
		
		for (int i = begin; i < end; i++)
		{
			sameDateListOfPair = ref(compactStructure[i]);
			sameDatePositions = ref(compactPositions[i]);
			uint32_t j = 0;
			uint32_t dim = sameDatePositions.size();
			pair<int32_t, uint32_t> Kpair;
				
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
					
					Kpair.first = i;
					Kpair.second = stoi(sameDateListOfPair.substr(pagEndCountStart, countLength));
					if (topK.size() >= K)
					{
						if (topK.top().second < Kpair.second)
						{
							topK.pop();						
							topK.push(Kpair);
						}
					}
					else topK.push(Kpair);
						
					break;
				}
				else j+=64;
			}
		}

		int32_t qsize = topK.size();		

		for(int i = 0; i  < qsize; i++) {
		
		pair<int32_t, uint32_t> p = topK.top();
		out << "Date: " << p.first << " Counter: " << p.second << endl;
		topK.pop();

	}

}

	~TopkQuery_V3(void)
	{     
	}
};

