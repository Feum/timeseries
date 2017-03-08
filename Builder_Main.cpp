#include <Builder1.h>
#include <Builder2.h>
#include <Builder3.h>
#include <Builder4.h>
#include <Builder5.h>
#include <Builder6.h>
#include <Builder7.h>
#include <cstdint>
#include <chrono>
#include <ctime>

#define BUILD \
  do { \
	  	b.readDataset(); \
		std::chrono::time_point<std::chrono::system_clock> start, end; \
		start = std::chrono::system_clock::now(); \
		b.compressStructure(); \
		b.serialize(); \
	    end = std::chrono::system_clock::now(); \
	    std::chrono::duration<double> elapsed_seconds = end-start; \
	    cout << "total elapsed time: " << elapsed_seconds.count() << "s\n"; \
} while (false)

int main(int argv, char** argc)
{
	
	int8_t versionID;
	string time_series;
	 
	
	if (argv != 3) return -1;
	versionID = stoi(argc[1]);
	time_series = argc[2];
	
	switch (versionID) {
	case 1 : {Builder_V1 b (versionID, time_series); BUILD; break;}
	case 2 : {Builder_V2 b (versionID, time_series); BUILD; break;}
	case 3 : {Builder_V3 b (versionID, time_series); BUILD; break;}
	case 4 : {Builder_V4 b (versionID, time_series); BUILD; break;}
	case 5 : {Builder_V5 b (versionID, time_series); BUILD; break;}
	case 6 : {Builder_V6 b (versionID, time_series); BUILD; break;}
	case 7 : {Builder_V7 b (versionID, time_series); BUILD; break;}
	default : {Builder_V7 b (versionID, time_series); BUILD; break;}
	}
	

}