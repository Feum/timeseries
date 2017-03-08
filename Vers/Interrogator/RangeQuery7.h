#include <cstdint>
#include <iostream>
#include <vector>
#include <fstream>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <cmph.h>

using namespace std;
using namespace sdsl;

class RangeQuery_V7
{
	
	uint32_t time1;
	uint32_t time2;
	uint32_t page;
	string pagestr;
	int_vector<32> results;

public:

	
	RangeQuery_V7(	uint32_t t1, 
					uint32_t t2, 
					uint32_t p,
					const string & pagestr)
	{
		this->time1 = t1;
		this->time2 = t2;
		this->page = p;
		this->pagestr = pagestr;
		results = int_vector<32>(t2-t1+1);
	}
	
	
	inline void performQuery(sd_vector<>::select_1_type & sel_structure, sd_vector<> & Structure, size_t dateSize)
	{
		
		size_t shift = page*dateSize;
		uint32_t prev = (shift + time1) == 0? 0 : sel_structure(shift + time1);
		uint32_t succ;
	
		for (int i = time1; i <= time2; i++)	
		{	succ = sel_structure(shift + i + 1);
			uint32_t counter = succ - prev;
			prev = succ; 
			results[i - time1] = counter;					
		}
		 
	}
	
	inline void printQuery() {
		ofstream out("result_007.txt", ofstream::app);
		out << ":::" << pagestr << ":::" << endl;
		size_t sz = results.size();
		
		for (size_t i=0; i < sz; i++) 
		{
			uint32_t d = i + time1;
			out << "Date: " << d << " Counter: " << results[i] <<endl;	
		}
	}

	~RangeQuery_V7(void)
	{     
	}


};