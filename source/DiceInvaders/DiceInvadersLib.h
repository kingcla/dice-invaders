#include <windows.h>
#include <cassert>
#include "DiceInvaders.h"

#define LIBRARY_PATH "DiceInvaders.dll"
#define SPRITE_SIZE 32


class DiceInvadersLib
{
public:

	// Check if the window is initialized
	bool isInititialized()
	{
		return isInit;
	}

	// Init the main window of the game
	bool init(int width, int height)
	{
		// Init the window once
		if (!isInit)
		{		
			isInit = m_interface->init(width, height);
		}
		
		return isInit;
	}

	// Get the singleton instance of the library
	static DiceInvadersLib& getInstance()
    {
		// Guaranteed to be destroyed.
        static DiceInvadersLib instance;
		// Instantiated on first use.
		return instance;
    }

	// Get the reference to the system
	IDiceInvaders* get() const 
	{
		return m_interface;
	}

private: 

	// Private contructor to the sigleton instance
	DiceInvadersLib()
	{
		// load the DICE library
		m_lib = LoadLibrary(LIBRARY_PATH);
		assert(m_lib);
		
		DiceInvadersFactoryType* factory = (DiceInvadersFactoryType*)GetProcAddress(m_lib, "DiceInvadersFactory");

		m_interface = factory();
		assert(m_interface);

		isInit = false;
	}

	~DiceInvadersLib()
	{
		m_interface->destroy();
		FreeLibrary(m_lib);
	}

	DiceInvadersLib(const DiceInvadersLib&);
	DiceInvadersLib& operator=(const DiceInvadersLib&);

	bool isInit;

private: 
	IDiceInvaders* m_interface;
	HMODULE m_lib;
};