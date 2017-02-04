#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <vector>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <cmph.h>
#include "bit_vector_alloc.h"
#include "sd_vector_alloc.h"

#define MAX_SIZE_FRACTION 40
#define DATE_LENGTH 11

using namespace std;
using namespace sdsl;		

class Builder_V4 {
	
	int8_t versionID;
		
	bit_vector_alloc bitKeys;
	bool ibk = false;
	vector<const char *> keys;
	
	vector<string> pairs;
	int32_t pidx = 0; // used to cut string in pieces for alloc problems
	sd_vector_alloc pairDelim;
	
	vector<string> listPerDate;
	vector<vector<uint32_t>> delimPerDate;
	
	
	void memo(const string &data, const string &pagina, const string &numero) {
		
		initBitKeys(data);
		
		if (bitKeys.check(data) == 0) {
			bitKeys.insert(data);
			keys.push_back(data.c_str());
		}
			int32_t l = pairs[pidx].length();
			int32_t d = data.length();
			int32_t p = pagina.length();
			pairDelim.push_back(l);
			pairDelim.push_back(l+d);
			pairDelim.push_back(l+d+p);
			pairs[pidx] += data+pagina+numero; 
			if (pairs[pidx].length() > pairs[pidx].max_size()/MAX_SIZE_FRACTION) //cut the string to reduce contiguous alloc problems
			{
				pidx++;
				pairs.push_back("");
			
			}
	
	}
	
	__inline void initBitKeys(const string & date) 
	{
		if (ibk == false)
			{	
				bit_vector_alloc k (date);
				bitKeys = ref(k);
				ibk = true;
			}
	}
	
public:
	
	Builder_V4(int8_t versionID)

	{
			this->versionID = versionID;
			pairs.push_back("");
			
			
			
					
	}
	
	inline void readDataset()
	{
		ifstream dataFile;
		dataFile.open("time_series.txt");
		
		
		if (dataFile.is_open())
		{	
			
			string line = "";
			string * data;
			string pagina = "";
			string numero = "";
			
			uint8_t c;
			istringstream iss(line);
			
			while (getline(dataFile, line))
			{
				
				iss.clear();
				iss.str(line);
				c = 0;

				while (getline(iss, numero, '\t')) 
					
				{  
				
					if (c == 0) 
						data = new string(numero); 
					else   
					if (c == 1)
						pagina = numero;
					else   
					if (c == 2)
						memo(*data, pagina, numero);			    
					else break;
					c++;
				}
			}
			 
			//Creating hash function
			pairDelim.close_and_compress();
			FILE* mphf_fd = fopen("hash.mph", "w");	
			cmph_io_adapter_t *source = cmph_io_vector_adapter((char **)keys.data(), keys.size());
			cmph_config_t *config = cmph_config_new(source);
		    cmph_config_set_algo(config, CMPH_BRZ);
		    cmph_config_set_mphf_fd(config, mphf_fd);
		    cmph_t *hash = cmph_new(config); 
		    cmph_config_destroy(config);
		    cmph_dump(hash, mphf_fd); 
		    cmph_destroy(hash);	
		    fclose(mphf_fd);
	}
	
	
	
}
	
	inline void compressStructure()
	{	
	      FILE* mphf_fd = fopen("hash.mph", "r");
		  cmph_t *hash = cmph_load(mphf_fd);
		  int64_t d2, d3, end;
		  string date;
		  string page;
		  string count;
		  end = pairDelim.nextInt();
		  pidx = 0;
		  
		  vector<string> l (keys.size());
		  listPerDate = ref(l);
		  
		  vector<vector<uint32_t>> p (keys.size());
		  delimPerDate = ref(p); 
		  
		  while (end != -1)
		  {
			  d2 = pairDelim.nextInt();
			  d3 = pairDelim.nextInt();
			  date = pairs[pidx].substr(end,(d2-end));
			  page = pairs[pidx].substr(d2,(d3-d2));
			  end = pairDelim.nextInt(); 
			  	  	  if (d3 < 0)
			  	  		  cout << pidx;
			  
			  if (end > 0)
				   count = pairs[pidx].substr(d3, (end-d3)); 
			  else {count = pairs[pidx].substr(d3, (pairs[pidx].length()-d3)); pidx++;}
			  	  
			  unsigned int id = cmph_search(hash, date.c_str(), (cmph_uint32)strlen(date.c_str())); 

			  
			  
			  delimPerDate[id].push_back(listPerDate[id].length());
			  delimPerDate[id].push_back(listPerDate[id].length() + page.length());
			 
			  listPerDate[id]+= page + count;
		  }
		  
		  
		  
		  
		  	  		
		
			
	}
	
	inline void serialize() 
	{
		
			//freeing
		
			pairs.clear();
			pairs.shrink_to_fit();
			pairDelim.clear();
			bitKeys.clear();
			
			//serializing
			
			
		
			string indexfile = string("index.") + to_string(versionID);
			ofstream ss(indexfile);
			
			sdsl::serialize(delimPerDate, ss, nullptr, string("pstn"));
			int32_t i = 0;
			vector<vector<char>> serialList (listPerDate.size());
			
			for (const auto str : listPerDate)
			{
				vector<char> data (str.begin(), str.end());
				serialList[i] = ref(data);
				i++;
			}
			
			sdsl::serialize(serialList, ss, nullptr, string("strct"));
			vector<vector<char>> skey (keys.size());
			i = 0;
			
			for (const auto cstr : keys)
			{
				vector<char> data (cstr, cstr + DATE_LENGTH);
				skey[i] = ref(data);
				i++;
			}
			
			sdsl::serialize(skey, ss, nullptr, string("keys"));
			
	}


		
	
	
};