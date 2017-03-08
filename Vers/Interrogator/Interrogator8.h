#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include <vector>
#include <numeric>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/rmq_support.hpp>
#include <RangeQuery8.h>
#include <TopkQuery8.h>
#include <cmph.h>
#include <algorithm>


using namespace std;
using namespace sdsl;


class Interrogator_V8 {
	
	int8_t versionID;
	vector<RangeQuery_V8> range_set;
	vector<TopkQuery_V8> topk_set;
	cmph_t *hash;
	unordered_map<string, uint32_t> dateCheck;
	sd_vector<> Structure;
	

	inline void saveOp(const string & operation,
						const string & time1,
	                    const string & time2,
	                    const string & page,
	                    const string & K)
	{

		if (operation == "range")
		{
			RangeQuery_V8 query(dateCheck[time1],dateCheck[time2], cmph_search(hash, page.c_str(), (cmph_uint32)strlen(page.c_str())), page);
			range_set.push_back(query);
		}
			
		if (operation == "top_k")
		{	
			int64_t k = stoi(K);
			if (k <= 0) return;
			TopkQuery_V8 query(dateCheck[time1],dateCheck[time2], cmph_search(hash, page.c_str(), (cmph_uint32)strlen(page.c_str())), k, page);
			topk_set.push_back(query);
		}	
		
	}
	
	
public:
	
	Interrogator_V8 (int8_t versionID) {
		
		this->versionID = versionID;
		ofstream out("result_008.txt");
	}
	
	inline void deserialize() {
		
	    FILE* mphf_fd = fopen("hash.mph", "r");
	    hash = cmph_load(mphf_fd);
	    fclose(mphf_fd);
	
		string fileindex = string("index.") + to_string(versionID-1);
		ifstream ss(fileindex,ios::binary);
        vector<vector<char>> skey;
		
				
		if (ss.is_open())
		{
				load (Structure, ss);
				load (skey, ss);
						
				ss.close();	
		} 
		
		int32_t i = 0;
		
		
		for (const auto vec : skey)
		{
			string data (vec.begin(), vec.end());
			dateCheck[data] = i;
			i++;
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
		sd_vector<>::select_1_type sel_structure (&Structure); 
		vector<double> rangeTimes; rangeTimes.reserve(range_set.size());
		vector<double> topkTimes; topkTimes.reserve(topk_set.size());
		
		for (RangeQuery_V8 query : range_set)
		{
			chrono::time_point<chrono::system_clock> start, end; 
			start = chrono::system_clock::now(); 
			
				query.performQuery(sel_structure, Structure, dateCheck.size());
			
			end = chrono::system_clock::now();
		    chrono::duration<double> elapsed_seconds = end-start;
		    rangeTimes.push_back(elapsed_seconds.count());
		    
		    	query.printQuery();
			
		}

		sd_vector<>::rank_1_type structure_ran(&Structure);
		size_t sz = structure_ran(Structure.size());
		int_vector<32> m_uncomp (sz);
		uint32_t prev = 0;
		uint32_t succ;
		
		//preproc start
		for (size_t i = 0; i < sz; i++)
			{
				succ = sel_structure(i+1);
				m_uncomp[i] = succ - prev;
				prev = succ;
			}
		
		rmq_succinct_sct<false> rmq (&m_uncomp);
		//preproc stop
		
		for (TopkQuery_V8 query : topk_set)
		{
			chrono::time_point<chrono::system_clock> start, end; 
			start = chrono::system_clock::now(); 
			
				query.performQuery(m_uncomp, rmq, dateCheck.size());
				
			end = chrono::system_clock::now();
			chrono::duration<double> elapsed_seconds = end-start;
			topkTimes.push_back(elapsed_seconds.count());
			
				query.printQuery();
		}
		if (!rangeTimes.empty())
		{
			double averageRange = accumulate( rangeTimes.begin(), rangeTimes.end(), 0.0)/rangeTimes.size();
			cout << "Average Range queries time: " << averageRange*1000 << " ms" << endl;
		}
		if (!topkTimes.empty())
		{
			double averageTopk = accumulate( topkTimes.begin(), topkTimes.end(), 0.0)/topkTimes.size();
			cout << "Average Top K queries time: " << averageTopk*1000 << " ms" << endl;
		}
		sd_vector<>::select_1_type().swap(sel_structure);
		int_vector<32>().swap(m_uncomp);
	}
	
	
	~Interrogator_V8 () {
		vector<RangeQuery_V8>().swap(range_set);
		vector<TopkQuery_V8>().swap(topk_set);
		sd_vector<>().swap(Structure);
	    cmph_destroy(hash);	
		unordered_map<string, uint32_t>().swap(dateCheck);

	}
	
	
	
	
};