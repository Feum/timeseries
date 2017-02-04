#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include <vector>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;
		

class Builder_V3 {
	
	int8_t versionID;
	
	map<int32_t, string> structure;
	map<int32_t, vector<int32_t>> positions;
		
	int32_t refYear = 0;
	int32_t refMonth = 0;
	int32_t refDay = 0;
	int32_t refHour = 0;
	string refDate;
	int32_t min = 0;
	int32_t max = 0;
	int32_t shift;
	vector<string> compactStructure;
	vector<rrr_vector<>> compactPositions;
	
	
	
	// >0 dopo la data di riferimento
	inline int32_t compressDate(const string & data) {

		string year = data.substr(0, 4);
		string month = data.substr(4, 2);
		string day = data.substr(6, 2);
		string hour = data.substr(9, 2);

		int32_t ID = 0;

		ID += stoi(hour) - refHour; 
		ID += (stoi(day) - refDay) * 24;
		ID += (stoi(month) - refMonth) * 744;
		ID += (stoi(year) - refYear) * 8928;

		return ID;



	}
	
	inline void referenceDate(const string & data)

	{
		if (refYear == 0) {
			refDate = data;
			refYear = stoi(data.substr(0, 4));
			refMonth = stoi(data.substr(4, 2));
			refDay = stoi(data.substr(6, 2));
			refHour = stoi(data.substr(9, 2));
		}
	}
	
	void memo(const string &data, const string &pagina, const string &numero) {
		
		string values = pagina + numero;
		referenceDate(data);
		int32_t dateAsNumber = compressDate(data);
		min = dateAsNumber < min ? dateAsNumber : min;
		max = dateAsNumber > max ? dateAsNumber : max;
		
			
		if (structure.find(dateAsNumber) != structure.end())
		{
			string &sameDateListOfPair = ref(structure[dateAsNumber]);
			int32_t sameDateListOfPair_length = sameDateListOfPair.length();
			int32_t last = positions[dateAsNumber].size();
			positions[dateAsNumber].push_back (sameDateListOfPair_length);
			positions[dateAsNumber].push_back(sameDateListOfPair_length + pagina.length());
			sameDateListOfPair += values;
			structure[dateAsNumber] = sameDateListOfPair;
		}
		else
		{
			positions[dateAsNumber].push_back(0);
			positions[dateAsNumber].push_back(pagina.length());
			structure[dateAsNumber] = values;
		}
	}
	
	rrr_vector<> compress(vector<int32_t> & large) {
		
		bit_vector small (large.size()*(1<<5),0);
		int32_t i = 0; 
		
//		cout << "large: " << size_in_mega_bytes(large) << endl; m/n troppo basso: vettore caratteristico meglio di elias-fano (534 mb vs 450 mb)
		
		for (const int32_t number : large) 
		{		
			small.set_int(i,number,32);
			i+=32;
		}
		rrr_vector<> rrr_small(small); 
		
//		cout << "small: " << size_in_mega_bytes(rrr_small) << endl;
		
		return rrr_small;
		
	}
	
public:
	
	Builder_V3(int8_t versionID)

	{
			this->versionID = versionID;
					
	}
	
	inline void readDataset()
	{
		ifstream dataFile;
		dataFile.open("time_series.txt");
		
		
		if (dataFile.is_open())
		{	
			
			string line = "";
			string data = "";
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
						data = numero; 
					else   
					if (c == 1)
						pagina = numero;
					else   
					if (c == 2)
						memo(data, pagina, numero);			    
					else break;
					c++;
				}
			}
	}
	
	
	
}
	
	inline void compressStructure()
	{
			if (structure.empty()) return;
			shift = -(min);
			max = max + 1;
			compactStructure.reserve((max + shift)*sizeof(string));
			
			for (int i = 0; i < (max+shift); i++)
			{	
				string init2;
				rrr_vector<> init1;
				compactStructure.push_back(init2);
				compactPositions.push_back(init1);
			}

					
			for (auto const ent1 : structure) 
			{
				compactStructure[ent1.first + shift] = ref(ent1.second);
				compactPositions[ent1.first + shift] = compress(positions[ent1.first]);
			}
		
			
	}
	
	inline void serialize() 
	{
		
		
			string indexfile = string("index.") + to_string(versionID);
			ofstream ss(indexfile);
			
			sdsl::serialize(compactPositions, ss, nullptr, string("pstn"));
			sdsl::serialize(shift, ss, nullptr, string("shift"));
			int32_t i = 0;
			vector<vector<char>> serialStructure (compactStructure.size());
			
			for (const auto str : compactStructure)
			{
				vector<char> data (str.begin(), str.end());
				serialStructure[i] = ref(data);
				i++;
			}
			
			vector<char> data (refDate.begin(), refDate.end());
			sdsl::serialize(serialStructure, ss, nullptr, string("strct"));
			sdsl::serialize(data, ss, nullptr, string("strct"));
			
	}




		
	
	
};