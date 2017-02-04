#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#define nqueries 1000

using namespace std;

map<string, vector<pair<string, string>>> structure;

void memo(const string &data, const string &pagina, const string &numero) {
		
		const pair <string, string> values(pagina, numero);
		
				
		if (structure.find(data) != structure.end())
		{
			vector<pair<string, string>> &sameDateListOfPair = structure[data];
			sameDateListOfPair.push_back(values);
			structure[data] = sameDateListOfPair;
		}
		else
		{
			vector<pair<string, string>> sameDateListOfPair;
			sameDateListOfPair.push_back(values);
			structure[data] = sameDateListOfPair;
		}
	}

int main (int argv, char** argc) {
	
	ifstream dataFile;
	dataFile.open("time_series.txt");
	
	if (argv != 2) return -1;
	
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
	
	
				int64_t dim = structure.size();
				srand(stoi(argc[1]));
				int64_t isx; int64_t idx; int8_t rnd; int64_t pag; int64_t record;
				vector <pair<string,vector<pair<string,string>>>> flat;
				string query;
				ofstream out("query_set_2.txt");
				
				
				for (auto ent : structure) 
				{
					flat.push_back(ent);
				}
				
				for (int64_t i = 0; i < nqueries; i++)
				{
					query = "";
					isx = rand() % (dim-1);
					idx = rand() % (dim-1);
					pag = rand() % (dim-1);
					rnd = rand() % 2;
					int8_t top = rand() % 30;
					
					if (idx < isx) {int64_t temp = idx; idx = isx; isx = temp;}
					
					if (rnd == 1)  
						query += "range\t";
					else query += "top_k\t";
					
					query += flat[isx].first +"\t";
					query += flat[idx].first +"\t";
					
					query += (flat[pag].second)[rand() % (flat[pag].second).size()].first;
					
					if (rnd == 0)
						query += "\t" + to_string(top);
					query+= "\n";
					
					out << query;
				}
	
	
	
	
}