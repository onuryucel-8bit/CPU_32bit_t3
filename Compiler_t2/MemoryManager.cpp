#include "MemoryManager.h"

namespace asmc
{
	MemoryManager::MemoryManager()
	{
		m_varPointer = 1000;
		m_activeRegister = -1;

		m_funcPointer = 0;
		m_stackPointer = 0;

		for (size_t i = 0; i < MAX_REG; i++)
		{
			m_registerPool[i] = false;
		}
	}

	MemoryManager::~MemoryManager()
	{
	}

	bool MemoryManager::allocRegister()
	{
		for (size_t i = 0; i < MAX_REG; i++)
		{
			if (m_registerPool[i] == false)
			{
				m_registerPool[i] = true;
				m_activeRegister = i;

				return true;
			}
		}

		return false;
	}

	void MemoryManager::releaseRegister(int index)
	{
		m_registerPool[index] = false;
		m_activeRegister = -1;
	}

	void MemoryManager::releaseRegister(std::string index)
	{

		m_registerPool[std::stoi(index)] = false;
		m_activeRegister = -1;
	}

	uint32_t MemoryManager::allocVariable()
	{
		return m_varPointer++;
	}

	void MemoryManager::defineFunction()
	{
	}

	void MemoryManager::push()
	{
	}

	void MemoryManager::pop()
	{
	}
}