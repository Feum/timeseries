#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include <vector>
#include <cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

using namespace std;
		

class Builder_V1 {
	
	int8_t versionID;
	string time_series;
	map<int32_t, vector<pair<string, string>>> structure;
		
	int32_t refYear = 0;
	int32_t refMonth = 0;
	int32_t refDay = 0;
	int32_t refHour = 0;
	string refDate;
	int32_t min = 0;
	int32_t max = 0;
	int32_t shift;
	vector<vector<pair<string, string>>> compactStructure;
	
	
	
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
		
		const pair <string, string> values(pagina, numero);
		referenceDate(data);
		int32_t dateAsNumber = compressDate(data);
		min = dateAsNumber < min ? dateAsNumber : min;
		max = dateAsNumber > max ? dateAsNumber : max;
		
		if (structure.find(dateAsNumber) != structure.end())
		{
			vector<pair<string, string>> &sameDateListOfPair = ref(structure[dateAsNumber]);
			sameDateListOfPair.push_back(values);
			structure[dateAsNumber] = sameDateListOfPair;
		}
		else
		{
			vector<pair<string, string>> sameDateListOfPair;
			sameDateListOfPair.push_back(values);
			structure[dateAsNumber] = sameDateListOfPair;
		}
	}
	
	
public:
	
	Builder_V1(int8_t versionID, string file)

	{
			this->versionID = versionID;
			this->time_series = file;
					
	}
	
	inline void readDataset()
	{
		ifstream dataFile;
		dataFile.open(time_series);
		
		
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
			compactStructure.reserve((max + shift)*sizeof(vector<pair<string, string>>));
			for (int i = 0; i < (max+shift); i++)
			{	
				
				vector<pair<string, string>> init2;
				compactStructure.push_back(init2);
			}
				
			
			for (auto const ent1 : structure) 
			{
				compactStructure[ent1.first + shift] = ref(ent1.second);
			}
		
			
	}
	
	inline void serialize() 
	{
		
			string indexfile = string("index.") + to_string(versionID);
			ofstream ss(indexfile, ios::binary);
			cereal::BinaryOutputArchive out(ss);
			out(compactStructure);
			out(refDate);
			out(shift);
		
	}




		
	
	
};