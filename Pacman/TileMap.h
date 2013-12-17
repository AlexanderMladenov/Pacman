#ifndef __TILEMAP_H
#define __TILEMAP_H
#include <bitset>
#include <vector>
#include "SDLInput.h"
#include "Geometry.h"
namespace Pacman
{
	//Tile numbers
	enum TileType
	{

		Wall = 0,
		Coin,
		Empty,
		GhostBlock,
		GhostBaseMid,
		PowerUp,
	};

	//interserction data structure holding info for the tile we intersect
	struct Intersection
	{

		size_t x;
		size_t y;
		TileType type;

	};

	//directions
	enum class Direction
	{
		Left = 0,
			Right,
			Up,
			Down,
			None
	};


	class Tilemap // hold a tile map and a corresponding to it map of direction flags for each tile. Supports Set and get for both containers, intersect function returning a vector 
		// holding all intersecting tiles with a given rect and a CalculateFlags function go calculate the flags after the grid has been set
	{
	public:
		typedef std::vector<Intersection> IntersectionsList;

		// sets all tiles to be walls and makes all flags false;
		Tilemap(size_t width, size_t height) : m_Width(width), m_Height(height)
		{
			m_Grid.resize(width * height);
			m_DirectionFlags.resize(width * height);
			for (auto &tileFlags : m_DirectionFlags)
			{
				tileFlags.reset();
			}

			for (auto& tile : m_Grid)
			{
				tile = Wall;
			}
		}

		~Tilemap() {}

		//set type of tile at x,y
		inline void Set(size_t x, size_t y, TileType type)
		{
			m_Grid[x + y * m_Width] = type;
		}

		//get the tile type at x,y
		inline char Get(size_t x, size_t y) const
		{
			return m_Grid[x + y * m_Width];
		}

		//set direction flag for x,y
		inline void SetDirectionFlag(size_t x, size_t y, size_t flag)
		{
			m_DirectionFlags[x + y * m_Width].set(flag, true);
		}
		//get a if target direction flag at x,y is set
		inline bool GetDirectionFlag(size_t x, size_t y, size_t flag)
		{
			return m_DirectionFlags[x + y * m_Width][flag];
		}

		void CalculateFlags() //goes trough all grid suqres and checks all adjacent squres to a current one. 
		{					//if any of them are viable move directions sets the appropriate direction flag on the current tile. 
			m_DirectionFlags.clear(); // wipes the current flags
			m_DirectionFlags.reserve(m_Width * m_Height); //resize the vector

			for (auto i = 0u; i < m_Width * m_Height; i++)
			{
				m_DirectionFlags.emplace_back(0); //make all flags false;
			}

			for (auto x = 0u; x < m_Width; x++)
			{
				for (auto y = 0u; y < m_Height; y++)
				{
					if (Get(x, y) == TileType::Wall || Get(x, y) == GhostBlock) // if the flag is a wall or ghost block tile
					{
						continue;
					}
					if (Get(x, y) == GhostBaseMid) // tiletype that has only up direction. Used in middle of ghost base only.
					{
						SetDirectionFlag(x, y, (size_t)Direction::Up);
					}
					if (x >= 1 && Get(x - 1, y) != TileType::Wall && Get(x - 1, y) != GhostBlock && Get(x - 1, y) != GhostBaseMid)
					{
						SetDirectionFlag(x, y, (size_t)Direction::Left);
					}

					if (x < m_Width - 1 && Get(x + 1, y) != TileType::Wall && Get(x + 1, y) != GhostBlock && Get(x + 1, y) != GhostBaseMid)
					{
						SetDirectionFlag(x, y, (size_t)Direction::Right);
					}

					if (y >= 1 && Get(x, y - 1) != TileType::Wall && Get(x, y - 1) != GhostBlock && Get(x, y - 1) != GhostBaseMid)
					{
						SetDirectionFlag(x, y, (size_t)Direction::Up);
					}

					if (y < m_Height - 1 && Get(x, y + 1) != TileType::Wall && Get(x, y + 1) != GhostBlock && Get(x, y + 1) != GhostBaseMid)
					{
						SetDirectionFlag(x, y, (size_t)Direction::Down);
					}
				}
			}
		}

		//get the grid itself
		inline const std::vector<char>& GetGrid() const
		{
			return m_Grid;
		}

		//intersection function that returns a vector holding all intersection with other tiles that the argument rect has. 
		//Always has at least one tile in intersection, unless our start rect is out of bounds
		inline IntersectionsList Intersect(const Rect& rect) const
		{
			IntersectionsList intersections;

			size_t s_x = (size_t)rect.x; //start x
			if (s_x < 0) // if our start.x is out of bounds
			{
				s_x = 0;
			}
			else if (s_x >= m_Width)
			{
				return intersections;
			}

			size_t s_y = (size_t)rect.y; // start y
			if (s_y < 0)
			{
				s_y = 0;
			}
			else if (s_y >= m_Height)
			{
				return intersections;
			}

			size_t e_x = (size_t)(rect.x + rect.width); //end x
			if (e_x < 0)
			{
				return intersections;
			}
			else if (e_x >= m_Width)
			{
				e_x = m_Width - 1;
			}

			size_t e_y = (size_t)(rect.y + rect.height); // end y
			if (e_y < 0)
			{
				return intersections;
			}
			else if (e_y >= m_Height)
			{
				e_y = m_Height - 1;
			}

			for (auto x = s_x; x <= e_x; x++) // iterates trough the rect walls and adds the tiles they itnersect in the vector
			{
				for (auto y = s_y; y <= e_y; y++)
				{
					Intersection i;
					i.x = x;
					i.y = y;
					i.type = (TileType)m_Grid[x + y * m_Width];
					intersections.push_back(i);
				}
			}

			return intersections;
		}

	private:

		size_t m_Width; // grid width
		size_t m_Height; // grid height

		std::vector<std::bitset<4>> m_DirectionFlags; //direction flags for each tile

		std::vector<char> m_Grid; //tile grid
	};
}

#endif