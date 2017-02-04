#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include <vector>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <RangeQuery4.h>
#include <TopkQuery4.h>
#include <cmph.h>
#include <algorithm>


using namespace std;
using namespace sdsl;

inline bool comp_V4(const vector <char> & c1, const vector<char> & c2)
{
    return strcmp(c1.data(), c2.data()) < 0;
}

class Interrogator_V4 {
	
	int8_t versionID;
	vector<string> listPerDate;
	vector<vector<uint32_t>> delimPerDate;
	vector<uint32_t> keys_ID_Chrono;
	vector<uint32_t> keys_Chrono_ID;
	vector<RangeQuery_V4> range_set;
	vector<TopkQuery_V4> topk_set;
	cmph_t *hash;
	



	inline void saveOp(const string & operation,
						const string & time1,
	                    const string & time2,
	                    const string & page,
	                    const string & K)
	{

		if (operation == "range")
		{
			RangeQuery_V4 query(time1,time2, page, hash, keys_Chrono_ID, keys_ID_Chrono);
			range_set.push_back(query);
		}
			
		if (operation == "top_k")
		{	
			int64_t k = stoi(K);
			if (k <= 0) return;
			TopkQuery_V4 query(time1,time2, page, k, hash, keys_Chrono_ID, keys_ID_Chrono);
			topk_set.push_back(query);
		}	
		
	}
	
	
public:
	
	Interrogator_V4 (int8_t versionID) {
		
		this->versionID = versionID;
	}
	
	inline void deserialize() {
		
	    FILE* mphf_fd = fopen("hash.mph", "r");
	    hash = cmph_load(mphf_fd);
	
		string fileindex = string("index.") + to_string(versionID);
		ifstream ss(fileindex,ios::binary);
		vector<vector<char>> serialList;
		vector<vector<char>> cKeys;
		
				
		if (ss.is_open())
		{
				load(delimPerDate, ss);
				load(serialList, ss);
				load (cKeys, ss);
						
				ss.close();	
		} 
		
		
		vector<string> tmp (serialList.size());
		int32_t i = 0;
		
		for (const auto vec : serialList)
		{
			string data (vec.begin(), vec.end());
			tmp[i] = ref(data);
			i++;
		}
		
		listPerDate = ref(tmp);
		sort(cKeys.begin(), cKeys.end(), comp_V4);
		keys_Chrono_ID.reserve(cKeys.size());
		vector<uint32_t> kid (cKeys.size());
		keys_ID_Chrono = ref (kid);
		uint32_t chrono = 0;
		
		for (auto const k : cKeys)
		{
			uint32_t id = cmph_search(hash, k.data(), (cmph_uint32)strlen(k.data()));
			keys_Chrono_ID.push_back(id);
			keys_ID_Chrono[id] = chrono;
			chrono++;
			
		}
		

		

		
	}
	
	inline void readQueryset(string queryset) {
	
		ifstream qry(queryset);
		
		if (qry.is_open())
		{
			string line = "";
			string operation = "";
			string time1 = "";
			string time2 = "";
			string page = "";
			string K = "";
			
			uint8_t c;
			istringstream iss(line);
				
				
			
			while (getline(qry, line))
			{
				iss.clear();
				iss.str(line + "\t0");
				c = 0;

				while (getline(iss, K, '\t')) 
					
				{  
				
					if (c == 0) 
						operation = K; 
					else   
					if (c == 1)
						time1 = K;
					else   
					if (c == 2)
						time2 = K;
					else
					if (c == 3)
						page = K;
					else
					if (c == 4)
						saveOp(operation, time1, time2, page, K);				
					else break;
					c++;
				}
			}
			qry.close();
		}
				
		
	}
	
	inline void performQueries() 
	{
		
		for (RangeQuery_V4 query : range_set)
		{
			query.performQuery(listPerDate, delimPerDate);
		}

		for (TopkQuery_V4 query : topk_set)
		{
			query.performQuery(listPerDate, delimPerDate);
		}
		
		
	}
	
	
	
	
	
	
};