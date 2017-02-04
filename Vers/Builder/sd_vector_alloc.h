#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include <vector>
#include <sdsl/vectors.hpp>
#include <sdsl/bit_vectors.hpp>
#include <cmph.h>

using namespace std;
using namespace sdsl;
		
//alloca interi positivi in vettore binario e lo comprime
class sd_vector_alloc {
	
	bit_vector bv;
	sd_vector<> sd_bv;
	int64_t size;
	int64_t capacity;
	uint64_t idx = 0;
	bool closed = false;
	

public:
	
	sd_vector_alloc()

	{
			bit_vector b(96);
			bv = ref(b);
			size = 0;
			capacity = 96;
							
	}
	

	inline void push_back(uint32_t value) 
	{
		if (closed) return;
		if (idx >= capacity)
		{
			capacity = capacity*2;
			bv.resize(capacity);
			
		}
		
		bv.set_int(idx, value, 32);
		idx+=32;
		size++;
				
	}

	inline void close_and_compress() 
	{
		sd_vector<> sd(bv);
		sd_bv = ref(sd);
		idx = 0;
		closed = true;
	}
	//requires: close_and_compress() eseguita prima di nextInt()
	inline int32_t nextInt()
	
	{
		int32_t res = -1;
		if (idx < (size*32))		
			res = sd_bv.get_int(idx,32);
		idx+=32;
		return res;
		
	}
	
	inline void clear()
	{
		bit_vector().swap(bv);
		sd_vector<>().swap(sd_bv);
		
		
	}
	
	~sd_vector_alloc(void)
	{
		clear();
	}
	
	
	
};