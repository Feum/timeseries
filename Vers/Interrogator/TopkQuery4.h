#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>
#include <string>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <cmph.h>


using namespace std;
using namespace sdsl;

struct compare_V4  
{  
	bool operator()(const pair<int32_t, uint32_t> & l, const  pair<int32_t, uint32_t> & r)  
	{  
		return l.second > r.second;  
	}  
};

class TopkQuery_V4
{
	
	uint32_t time1;
	uint32_t time2;
	string page;
	uint32_t K;
	vector<uint32_t> keys_Chrono_ID;
	vector<uint32_t> keys_ID_Chrono;
	
public:
	
	
	TopkQuery_V4(	const string & t1, 
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
		uint32_t size = listPerDate.size();
		string list;
		vector<uint32_t> delims;
		
		priority_queue<pair<int32_t, uint32_t>,vector<pair<int32_t, uint32_t>>,compare_V4> topK;
		
		ofstream out("result_004.txt", ofstream::app);
		out << ":::" << page << ":::" << endl;
		for (int i = time1; i != time2; i = keys_Chrono_ID[keys_ID_Chrono[i]+1])
		{	
			list = ref(listPerDate[i]);
			delims = ref(delimPerDate[i]);

			uint32_t dim = delims.size();
					
			pair<int32_t, uint32_t> Kpair;
				
			if (dim == 0 || list.length() == 0) continue;
			for (uint32_t j = 0; j < dim - 1; j+=2)
			{
				int32_t pagStart = delims[j];
				int32_t pagEndCountStart = delims[j+1];
				int32_t pagLength = pagEndCountStart - pagStart;
				int32_t countEnd = j == dim - 2 ? list.length() : delims[j+2];
				int32_t countLength = countEnd - pagEndCountStart;
				
				
				if (list.substr(pagStart, pagLength) == page)
				{
					
					Kpair.first = i;
					Kpair.second = stoi(list.substr(pagEndCountStart, countLength));
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
				
			}
		}

		int32_t qsize = topK.size();		

		for(int i = 0; i  < qsize; i++) {
		
		pair<int32_t, uint32_t> p = topK.top();
		out << "Date: " << p.first << " Counter: " << p.second << endl;
		topK.pop();

			}

}

	~TopkQuery_V4(void)
	{     
	}
};

