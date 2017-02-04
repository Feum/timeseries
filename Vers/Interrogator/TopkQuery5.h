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

struct compare_V5  
{  
	bool operator()(const tuple<int32_t, uint32_t, uint32_t, uint32_t> & l, const  tuple<int32_t, uint32_t, uint32_t, uint32_t> & r)  
	{  
		return (get<1>(l) <= get<1>(r));  
	}  
};

class TopkQuery_V5
{
	
	uint32_t time1;
	uint32_t time2;
	string page;
	uint32_t K;
	vector<uint32_t> keys_Chrono_ID;
	vector<uint32_t> keys_ID_Chrono;
	
	inline bool substr_cmp (const char * str1, const char * str2, size_t n) const
	{
		bool equal = true;
		size_t i = 0;
		
		while (equal && i<n)
		{
			if (str1[i] != str2[i]) equal = false;
			i++;
		}
		
		return equal;
	}
	
	inline void out_topk(const rmq_succinct_sct<false> & rmq, const int_vector<> & topK, const size_t size, ofstream & out) const
	{
		uint32_t l, r; l = 0; r = size - 1;
		size_t idx = rmq (l, r);
		out << "Date: " << idx << " Counter: " << topK[idx] << endl;
		
		priority_queue<tuple<int32_t, uint32_t, uint32_t, uint32_t>, vector<tuple<int32_t, uint32_t, uint32_t, uint32_t>>,compare_V5> heap;
		
		for (size_t i = 0; i < K - 1; i++) {
		
		if (l < idx) {
			size_t m_left = rmq(l, idx-1);
			heap.push(make_tuple(m_left,topK[m_left], l, idx-1));
		}
	
		if (idx < r) {
		
		size_t m_right = rmq(idx+1, r);
		heap.push(make_tuple(m_right,topK[m_right], idx+1, r));
		}
		
		if (heap.empty()) break;
		tuple<int32_t, uint32_t, uint32_t, uint32_t> actual = heap.top(); heap.pop();
		out << "Date: " << get<0>(actual) << " Counter: " << get<1>(actual) << endl;		
		l = get<2>(actual);
		r = get<3>(actual);
		idx = get<0>(actual);
	
		
	
				

		

			}
	}
	
public:
	
	
	TopkQuery_V5(	const string & t1, 
				const string & t2, 
				const string & p, 
				uint32_t K, 
				cmph_t  * hash, 
				const vector<uint32_t> & keys_Chrono_ID,
				const vector<uint32_t> & keys_ID_Chrono)
	{
		this->time1 = cmph_search(hash, t1.c_str(), (cmph_uint32)strlen(t1.c_str()));
		this->time2 = cmph_search(hash, t2.c_str(), (cmph_uint32)strlen(t2.c_str()));
		this->page = p;
		this->K = K;
		this->keys_Chrono_ID = keys_Chrono_ID;
		this->keys_ID_Chrono = keys_ID_Chrono;
		
	}
	
	void performQuery(const vector<string> & listPerDate, const vector<vector<uint32_t>> & delimPerDate) const
	{
		size_t size = listPerDate.size();
		string list;
		vector<uint32_t> delims;
		
		int_vector<> topK (size);
		
		ofstream out("result_005.txt", ofstream::app);
		out << ":::" << page << ":::" << endl;
		for (int i = time1; i != time2; i = keys_Chrono_ID[keys_ID_Chrono[i]+1])
		{	
			list = ref(listPerDate[i]);
			delims = ref(delimPerDate[i]);

			uint32_t dim = delims.size();
				
			if (dim == 0 || list.length() == 0) continue;
			for (uint32_t j = 0; j < dim - 1; j+=2)
			{
				int32_t pagStart = delims[j];
				int32_t pagEndCountStart = delims[j+1];
				int32_t pagLength = pagEndCountStart - pagStart;
				int32_t countEnd = j == dim - 2 ? list.length() : delims[j+2];
				int32_t countLength = countEnd - pagEndCountStart;
				size_t n = pagLength < page.length()? pagLength : page.length();
				
				if (substr_cmp (list.data() + pagStart, page.data(), n))
				{
					
					topK[i] = stoi(list.substr(pagEndCountStart, countLength));
					
					break;
				}
				
			}
			
		
		}	
		
		
		rmq_succinct_sct<false> rmq(&topK);
		
		out_topk(rmq, topK, size, out);


}

	~TopkQuery_V5(void)
	{     
	}
};

