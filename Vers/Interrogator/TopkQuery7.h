#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/rmq_support.hpp>
#include <cmph.h>

using namespace std;
using namespace sdsl;

struct compare_V7  
{  
	bool operator()(const pair<int32_t, uint32_t> & l, const  pair<int32_t, uint32_t> & r)  
	{  
		return l.second > r.second;  
	}  
};

class TopkQuery_V7
{
	
	uint32_t time1;
	uint32_t time2;
	uint32_t page;
	uint32_t K;
	string pagestr;
	priority_queue<pair<int32_t, uint32_t>,vector<pair<int32_t, uint32_t>>,compare_V7> topK;

public:
	
	
	TopkQuery_V7(	uint32_t t1, 
					uint32_t t2, 
					uint32_t p,
					uint32_t K,
					const string & pagestr) 
	{
		this->time1 = t1;
		this->time2 = t2;
		this->page = p;
		this->K = K;
		this->pagestr = pagestr;
	}
	
	inline void performQuery(sd_vector<>::select_1_type & sel_structure, sd_vector<> & Structure, size_t dateSize)
	{
		size_t shift = page*dateSize;
		uint32_t prev = (shift + time1) == 0? 0 : sel_structure(shift + time1);
		uint32_t succ;
		

		for (int i = time1+1; i <= time2; i++)
		{
				pair<int32_t, uint32_t> Kpair;
				succ = sel_structure(shift + i + 1);
				uint32_t counter = succ - prev;
				prev = succ; 
				Kpair.first = i;
				Kpair.second = counter;
				if (topK.size() >= K)
				{
					
					if (topK.top().second < Kpair.second)
					{
						topK.pop();						
						topK.push(Kpair);
					}
				}
				else topK.push(Kpair);
								
		}
	
}
	
	inline void printQuery() {
		
		ofstream out("result_007.txt", ofstream::app);
		out << ":::" << pagestr << ":::" << endl;
		
		for(int i = 0; i  < K; i++) {
		
		pair<int32_t, uint32_t> p = topK.top();
		out << "Date: " << p.first << " Counter: " << p.second << endl;
		topK.pop();

			}
	}

	~TopkQuery_V7(void)
	{ 
		priority_queue<pair<int32_t, uint32_t>,vector<pair<int32_t, uint32_t>>,compare_V7>().swap(topK); //freeing queue
	}
};

