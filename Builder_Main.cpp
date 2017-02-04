#include <Builder1.h>
#include <Builder2.h>
#include <Builder3.h>
#include <Builder4.h>
#include <Builder5.h>
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
	    cout << "elapsed time: " << elapsed_seconds.count() << "s\n"; \
} while (false)

int main(int argv, char** argc)
{
	
	int8_t versionID;
	 
	
	if (argv != 2) return -1;
	versionID = stoi(argc[1]);
	
	switch (versionID) {
	case 1 : {Builder_V1 b (versionID); BUILD; break;}
	case 2 : {Builder_V2 b (versionID); BUILD; break;}
	case 3 : {Builder_V3 b (versionID); BUILD; break;}
	case 4 : {Builder_V4 b (versionID); BUILD; break;}
	case 5 : {Builder_V5 b (versionID); BUILD; break;}
	}
	

}