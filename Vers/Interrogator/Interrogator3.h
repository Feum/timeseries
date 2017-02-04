#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include <vector>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <RangeQuery3.h>
#include <TopkQuery3.h>


using namespace std;
using namespace sdsl;

class Interrogator_V3 {
	
	int8_t versionID;
	vector<string> compactStructure;
	vector<rrr_vector<>> compactPositions;
	vector<RangeQuery_V3> range_set;
	vector<TopkQuery_V3> topk_set;
	string refDate;
	int32_t normalize;
	int32_t refYear = 0;
	int32_t refMonth = 0;
	int32_t refDay = 0;
	int32_t refHour = 0;
	
	
	inline void referenceDate()

	{
			refYear = stoi(refDate.substr(0, 4));
			refMonth = stoi(refDate.substr(4, 2));
			refDay = stoi(refDate.substr(6, 2));
			refHour = stoi(refDate.substr(9, 2));
		
	}
	
	inline int32_t compressDate(const string & time)
	{
		string year = time.substr(0, 4);
		string month = time.substr(4, 2);
		string day = time.substr(6, 2);
		string hour = time.substr(9, 2);

		int32_t ID = 0;
		ID += stoi(hour) - refHour; 
		ID += (stoi(day) - refDay) * 24;
		ID += (stoi(month) - refMonth) * 744;
		ID += (stoi(year) - refYear) * 8928;

		return ID;
	}
	
	inline void saveOp(const string & operation,
						const string & time1,
	                    const string & time2,
	                    const string & page,
	                    const string & K)
	{

		if (operation == "range")
		{
			RangeQuery_V3 query(compressDate(time1) + normalize, compressDate(time2) + normalize, page);
			range_set.push_back(query);
		}
			
		if (operation == "top_k")
		{	
			int64_t k = stoi(K);
			if (k <= 0) return;
			TopkQuery_V3 query(compressDate(time1) + normalize, compressDate(time2) + normalize, page, k);
			topk_set.push_back(query);
		}	
		
	}
	
	
public:
	
	Interrogator_V3 (int8_t versionID) {
		
		this->versionID = versionID;
	}
	
	inline void deserialize() {
	
		string fileindex = string("index.") + to_string(versionID);
		ifstream ss(fileindex,ios::binary);
		vector<vector<char>> serialStructure;
		vector<char> data;
		
		if (ss.is_open())
		{
				load(compactPositions, ss);
				load(normalize, ss);
				load(serialStructure, ss);
				load(data, ss);
			
				ss.close();	
		} 
		
		refDate = string(data.begin(), data.end());
		
		vector<string> structure (serialStructure.size());
		int32_t i = 0;
		
		for (const auto vec : serialStructure)
		{
			string data (vec.begin(), vec.end());
			structure[i] = ref(data);
			i++;
		}
		
		compactStructure = ref(structure);
		
		referenceDate();
		
		
		
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
		
		for (RangeQuery_V3 query : range_set)
		{
			query.performQuery(compactStructure, compactPositions);
		}

		for (TopkQuery_V3 query : topk_set)
		{
			query.performQuery(compactStructure, compactPositions);
		}
		
		
	}
	
	
	
	
	
	
};