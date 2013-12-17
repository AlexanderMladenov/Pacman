#ifndef __SDL_INPUT_H
#define __SDL_INPUT_H

#include <bitset>

namespace Pacman
{
	//key numbers
	enum Key
	{
		Up,
		Left,
		Down,
		Right,
	};

	class SDLInput
	{
	public:

		SDLInput() //sets all keys false;
		{
			m_Keys.reset();
		}

		bool Poll(); // sets flags in bitset to true or false depending on if we hit or relesed a key

		inline bool GetKey(Key key) const { return m_Keys[key]; } // get if a given key is pressed

	private:

		std::bitset<4> m_Keys; //set ot keys
	};
}

#endif