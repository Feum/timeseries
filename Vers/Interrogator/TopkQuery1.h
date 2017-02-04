#include <iostream>
#include <vector>
#include <queue>
#include <stdint.h>
#include <string>


using namespace std;

struct compare_V1  
{  
	bool operator()(const pair<int32_t, uint32_t> & l, const  pair<int32_t, uint32_t> & r)  
	{  
		return l.second > r.second;  
	}  
};

class TopkQuery_V1
{
	
	int32_t time1;
	int32_t time2;
	string page;
	uint32_t K;
	
public:
	
	
	TopkQuery_V1(int32_t t1, int32_t t2, const string & p, uint32_t K)
	{
		this->time1 = t1;
		this->time2 = t2;
		this->page = p;
		this->K = K; 
	}
	
	void performQuery(const vector<vector<pair<string, string>>> & compactStructure) 
	{
		uint32_t begin = time1 >= 0 ? time1 : 0;
		uint32_t size = compactStructure.size();
		uint32_t end = (time2 + 1) < size ? (time2 + 1) : size;
		vector<pair<string, string>> sameDateListOfPair;
		
		priority_queue<pair<int32_t, uint32_t>,vector<pair<int32_t, uint32_t>>,compare_V1> topK;
		
		ofstream out("result_001.txt", ofstream::app);
		out << ":::" << page << ":::" << endl;
		
		for (int i = begin; i < end; i++)
		{
			sameDateListOfPair = ref(compactStructure[i]);
			uint32_t j = 0;
			uint32_t dim = sameDateListOfPair.size();
			pair<int32_t, uint32_t> Kpair;
				
			while (j < dim)
			{
				if (sameDateListOfPair[j].first == page)
				{
					Kpair.first = i;
					Kpair.second = stoi(sameDateListOfPair[j].second);
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
				else j++;
			}
		}

		int32_t qsize = topK.size();		

		for(int i = 0; i  < qsize; i++) {
		
		pair<int32_t, uint32_t> p = topK.top();
		out << "Date: " << p.first << " Counter: " << p.second << endl;
		topK.pop();

	}

}

	~TopkQuery_V1(void)
	{     
	}
};

