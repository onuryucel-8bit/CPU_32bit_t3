#pragma once

#include <cstdint>
#include <string>

namespace asmc
{


	class MemoryManager
	{
	public:
		MemoryManager();
		~MemoryManager();

		bool allocRegister();
		void releaseRegister(int index);
		void releaseRegister(std::string index);

		uint32_t allocVariable();
		void defineFunction();

		void push();
		void pop();

		int m_activeRegister;

	private:

		static const char MAX_REG = 8;
		bool m_registerPool[MAX_REG];

		size_t m_stackPointer;
		uint32_t m_varPointer;
		size_t m_funcPointer;
		
		

		/*
		+-------------------+
		|					|
		|		Code 		|
		|					|
		+-------------------+
		|					|
		|	  Variables		|
		|					|
		+-------------------+
		|					|
		|	 Functions		|
		|					|
		+-------------------+
		|					|
		|	   Stack		|
		|					|
		+-------------------+

		*/
	};

	

}