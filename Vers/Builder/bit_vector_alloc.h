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
		
//implementa un vettore caratteristico delle date
class bit_vector_alloc {
	
	bit_vector bv_p;
	bit_vector bv_n;

	
	int64_t REF_YEAR;
	int64_t REF_MONTH;
	int64_t REF_DAY;
	int64_t REF_HOUR;
	
	
	
	inline void add(int64_t pos, int8_t value) 
	{	
		size_t size;
		
		if (pos >= 0)
		{
			size = bv_p.size();
			if (pos >= size)
			{
				resize(size, bv_p); 
				this->add(pos,value);
				
			}
			else bv_p[pos] = value;
		}
		
		if (pos < 0)
		{
			size = bv_n.size();
			if (-pos >= size)
			{
				resize(size, bv_n); 
				this->add(pos,value);
				
			}
			else bv_n[-pos] = value;
		}
		
				
	}
	
	inline int64_t toInt(const string & value)
	{	
		string year = value.substr(0, 4);
		string month = value.substr(4, 2);
		string day = value.substr(6, 2);
		string hour = value.substr(9, 2);

		int64_t ID = 0;

		ID += stoi(hour) - REF_HOUR; 
		ID += (stoi(day) - REF_DAY) * 24;
		ID += (stoi(month) - REF_MONTH) * 744;
		ID += (stoi(year) - REF_YEAR) * 8928;

		return ID;
	}
	
	int8_t get (int64_t ID) {
		
		if (ID >= 0) 
			{
				if (ID < bv_p.size())
					return bv_p[ID];
				else
					return 0;
			}
		else 
			{
				if (-ID < bv_n.size())
					return bv_n[-ID];
				else				
					return 0;
			}
			
			
			
		
	}
	
	inline void resize (size_t oldSize, bit_vector & bv)
	{
		
		size_t newSize = oldSize*2;
		bv.resize(newSize);
		for (size_t i = oldSize; i < newSize; i++)
		{
			bv[i] = 0;
		}
	}
	
public:
	
	bit_vector_alloc() 
	{
		
	}
	
	bit_vector_alloc(const string & REF_DATE)

	{
			bit_vector b(10);
			bv_p = ref(b);
			
			bit_vector n(10);
			bv_n = ref(n);
			
			REF_YEAR =  stoi(REF_DATE.substr(0, 4));
			REF_MONTH = stoi(REF_DATE.substr(4, 2));
			REF_DAY =   stoi(REF_DATE.substr(6, 2));
			REF_HOUR =  stoi(REF_DATE.substr(9, 2));

	}
	

	inline void insert(const string & value)
	{
		int64_t ID = toInt(value);
		this->add(ID, 1);
		
	
			
			
		

	}
	
	inline int8_t check (const string & value) 
	{
		int64_t ID = toInt(value);  
		return get(ID);
	}
	
	
	inline void clear()
	{
		bit_vector().swap(bv_n);
		bit_vector().swap(bv_p);
				
	}
	
	~bit_vector_alloc(void)
	{
		clear();
	}
	
};