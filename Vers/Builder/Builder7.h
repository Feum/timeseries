#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <cmph.h>

#define MAX_SIZE_FRACTION 40
#define DATE_LENGTH 11



using namespace std;
using namespace sdsl;		

class Builder_V7 {
	
	typedef void (Builder_V7::*tripleFunction)(const string &d, const string &p, const string &n);
	
	int8_t versionID;
	string time_series;	
	
	unordered_map<string, uint8_t> pageCheck;
	unordered_map<string, uint32_t> dateCheck;
	bool ibk = false;
	vector<const char *> pages;
	vector<string> dates;
	
	cmph_t *hash;
	int_vector<32> Structure;
	sd_vector<> FanoStructure;
	
	
	inline void read(const string &data, const string &pagina, const string &numero) {
			
		if (pageCheck[pagina] != 1)
			{
				pageCheck[pagina] = 1;
				pages.push_back(pagina.c_str());
			}
		
		if (dateCheck[data] != 1)
			{
				dateCheck[data] = 1;
				dates.push_back(data);
			}
		
	
	}
	
	inline void memo(const string &data, const string &pagina, const string &numero) {
			
		  size_t dateSize = dates.size();
		  uint32_t Pid = cmph_search(hash, pagina.c_str(), (cmph_uint32)strlen(pagina.c_str()));
		  uint32_t Did = dateCheck[data]; 
		  
		  Structure[(Pid*dateSize) + Did] = stoi(numero);
		  
	}
	
	inline void readDataset(tripleFunction foo, Builder_V7 & obj)
		{
			ifstream dataFile;
			dataFile.open(time_series);
			
			
			if (dataFile.is_open())
			{	
				
				string line = "";
				string data = "";
				string * pagina;
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
							data = numero; 
						else   
						if (c == 1)
							pagina = new string(numero);
						else   
						if (c == 2)
							(obj.*foo)(data, *pagina, numero);			    
						else break;
						c++;
					}
				}
				 
				
				dataFile.close();

			    
		}
		
		
		
	}
		
	
public:
	
	Builder_V7(int8_t versionID, string file)

	{
			this->versionID = versionID;
			this->time_series = file;			
					
	}
	
	
	inline void readDataset() 
	{
		   readDataset(&Builder_V7::read, *this);
		   
			unordered_map<string, uint8_t>().swap(pageCheck); //freeing pageCheck
			sort(dates.begin(), dates.end());
			size_t sz = dates.size();
			for (size_t i = 0; i < sz; i++)
			{
				string &s = ref(dates[i]);
				dateCheck[s] = i;
			}
			
			//Creating hash function
			FILE* mphf_fd = fopen("hash.mph", "w");	
			cmph_io_adapter_t *source = cmph_io_vector_adapter((char **)pages.data(), pages.size());
			cmph_config_t *config = cmph_config_new(source);
		    cmph_config_set_algo(config, CMPH_BRZ);
		    cmph_config_set_mphf_fd(config, mphf_fd);
		    cmph_t *hash = cmph_new(config); 
		    cmph_config_destroy(config);
		    cmph_dump(hash, mphf_fd); 
		    cmph_destroy(hash);	
		    fclose(mphf_fd);
	}
	
	inline void compressStructure()
	{	
		  size_t pageSize = pages.size();
		  size_t dateSize = dates.size();
		  size_t totSize = pageSize*dateSize;
		  Structure = int_vector<32>(totSize, 0);
		  FILE* mphf_fd = fopen("hash.mph", "r");
		  hash = cmph_load(mphf_fd);
		  
		  readDataset(&Builder_V7::memo, *this);

		  
		  for (size_t i = 1; i < totSize; i++)
			  	  Structure[i] = Structure[i] + Structure[i-1];
			  		 	  
		  FanoStructure =  sd_vector<> (Structure.begin(), Structure.end());
		  
	}
		  
	
	inline void serialize() 
	{

			
			cout << "Structure: " << size_in_mega_bytes(Structure) << endl;			
			int_vector<32>().swap(Structure); //freeing Structure
			cout << "FanoStructure: " << size_in_mega_bytes(FanoStructure) << endl;

			//serializing
						
			string indexfile = string("index.") + to_string(versionID);
			ofstream ss(indexfile);
			
			sdsl::serialize(FanoStructure, ss, nullptr, string("strct"));

			
			
			vector<vector<char>> skey (dates.size());		
			int32_t i = 0;
			for (const auto str : dates)
			{
				vector<char> data (str.begin(), str.end());
				skey[i] = ref(data);
				i++;
			}
			
			sdsl::serialize(skey, ss, nullptr, string("dates"));

	}

	~Builder_V7()

	{
		sd_vector<>().swap(FanoStructure); //freeing FanoStructure
		vector<string>().swap(dates); //freeing dates
		unordered_map<string, uint32_t>().swap(dateCheck); //freeing dateCheck		
					
	}
		
	
	
};