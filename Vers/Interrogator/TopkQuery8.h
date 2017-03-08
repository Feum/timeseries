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

struct compare_V8  
{  
	bool operator()(const tuple<int32_t, uint32_t, uint32_t, uint32_t> & l, const  tuple<int32_t, uint32_t, uint32_t, uint32_t> & r)  
	{  
		return (get<1>(l) <= get<1>(r));  
	}  
};

class TopkQuery_V8
{
	
	uint32_t time1;
	uint32_t time2;
	uint32_t page;
	uint32_t K;
	string pagestr;
	int_vector<32> topK;
		
	inline void out_topk(const rmq_succinct_sct<false> & rmq, const int_vector<32> & m_uncomp, size_t shift)
		{
			uint32_t l, r; l = shift + time1; r = shift + time2;
			size_t idx = rmq (l, r);
			topK[0] = idx; //top1 date
			topK[1] = m_uncomp[idx]; //top1 counter
			
			priority_queue<tuple<int32_t, uint32_t, uint32_t, uint32_t>, vector<tuple<int32_t, uint32_t, uint32_t, uint32_t>>,compare_V8> heap;
			
			for (size_t i = 0; i < K - 1; i++) {
			
			if (l < idx) {
				size_t m_left = rmq(l, idx-1);
				heap.push(make_tuple(m_left,m_uncomp[m_left], l, idx-1));
			}
		
			if (idx < r) {
			
			size_t m_right = rmq(idx+1, r);
			heap.push(make_tuple(m_right,m_uncomp[m_right], idx+1, r));
			}
			
			if (heap.empty()) break;
			tuple<int32_t, uint32_t, uint32_t, uint32_t> actual = heap.top(); heap.pop();
			topK[2*i + 2] = get<0>(actual); //top(i+2) date
			topK[2*i + 3] = get<1>(actual); //top(i+2) counter	
			l = get<2>(actual);
			r = get<3>(actual);
			idx = get<0>(actual);
		
			
		
					

			

				}
		}
	
	
public:
	
	
	TopkQuery_V8(	uint32_t t1, 
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
		this->topK = int_vector<32>(2*K);
	}
	
	inline void performQuery(const int_vector<32> & m_uncomp, const rmq_succinct_sct<false> & rmq, size_t dateSize)
	{
		size_t shift = page*dateSize;
		
		out_topk(rmq, m_uncomp, shift);
	
}
	
	inline void printQuery() {
		
		ofstream out("result_008.txt", ofstream::app);
		out << ":::" << pagestr << ":::" << endl;
		
		for(int i = 0; i < K; i++) {
		
		out << "Date: " << topK[2*i] << " Counter: " << topK[2*i + 1] << endl;

			}
	}

	~TopkQuery_V8(void)
	{ 
		int_vector<32>().swap(topK); //freeing topK
	}
};

