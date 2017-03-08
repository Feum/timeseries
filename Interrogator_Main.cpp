#include <Interrogator1.h>
#include <Interrogator2.h>
#include <Interrogator3.h>
#include <Interrogator4.h>
#include <Interrogator5.h>
#include <Interrogator6.h>
#include <Interrogator7.h>
#include <Interrogator8.h>
#include <cstdint>
#include <chrono>
#include <ctime>

#define INTERROGATE \
  do { \
	i.deserialize(); \
	i.readQueryset(queryset); \
	std::chrono::time_point<std::chrono::system_clock> start, end; \
	start = std::chrono::system_clock::now(); \
	i.performQueries(); \
	end = std::chrono::system_clock::now(); \
    std::chrono::duration<double> elapsed_seconds = end-start; \
    cout << "total elapsed time: " << elapsed_seconds.count() << "s\n"; \
} while (false)

int main(int argv, char** argc)
{
	
	int8_t versionID;
	 
	
	if (argv != 3) return -1;
	versionID = stoi(argc[1]);
	string queryset = argc[2];
	
	switch (versionID) {
	case 1 : {Interrogator_V1 i (versionID); INTERROGATE; break;}
	case 2 : {Interrogator_V2 i (versionID); INTERROGATE; break;}
	case 3 : {Interrogator_V3 i (versionID); INTERROGATE; break;}
	case 4 : {Interrogator_V4 i (versionID); INTERROGATE; break;}
	case 5 : {Interrogator_V5 i (versionID); INTERROGATE; break;}
	case 6 : {Interrogator_V6 i (versionID); INTERROGATE; break;}
	case 7 : {Interrogator_V7 i (versionID); INTERROGATE; break;}
	case 8 : {Interrogator_V8 i (versionID); INTERROGATE; break;}
	default : {Interrogator_V8 i (versionID); INTERROGATE; break;}
	}
	

}