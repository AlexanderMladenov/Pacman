#include <vector>
#include <iostream>

#include "SDLInput.h"

#include "Geometry.h"
#include "Tilemap.h"
#include "Pacman.h"
#include "Timer.h"

namespace Pacman
{
	//intializes pacman at point x,y
	Pacman::Pacman(float x, float y)
	{
		m_Position.x = x;
		m_Position.y = y;

		m_Points = 0;
		m_Lives = 2;
		m_powerDuration = 0.f;

		m_CurrentDirection = Direction::None;
		m_NextDirection = Direction::None;

		m_State = PacState::Normal;

		m_DistanceLeft = 1.0f; // set the distance to one tile
	}
	void Pacman::Reset()
	{
		m_Position.x = 9.f;
		m_Position.y = 13.f;

		m_powerDuration = 0.f;

		m_CurrentDirection = Direction::None;
		m_NextDirection = Direction::None;

		m_State = PacState::Normal;

		m_DistanceLeft = 1.0f; // set the distance to one tile

		m_Lives -= 1;
		if (m_Lives < 0)
		{
			exit(0);
		}
	}

	void Pacman::OnCollision(const std::shared_ptr<ICollider>& collidedWith)
	{	
		if (collidedWith->GetType() == Type::Ghost) // or we hit a normal ghost
			m_State = PacState::Dead;

		else if (collidedWith->GetType() == Type::Power && m_State != PacState::PowerUp)// or we hit a powerup
		{
			m_State = PacState::PowerUp;
			m_powerDuration = Timer::GetTime();//start duration
		}
	}

	void Pacman::Update(const SDLInput& input, Tilemap& tilemap, float deltaTime, const std::shared_ptr<SDLRenderTarget>& tileSurface, const std::vector<SDLSurface>& gridSurf)
	{
		if (m_State == PacState::PowerUp) // if powered up
		{
			float currentTime = Timer::GetTime();
			if ((currentTime - m_powerDuration) > 5.f) // if powered up for more tha 5 seconds
			{
				m_State = PacState::Normal;// return to normal
			}
		}

		//set our speed
		float speed = 5.5f * deltaTime;

		// clamp position
		size_t cX = (size_t)(clamp(m_Position.x, 0.f, 18.f) + 0.5f);
		size_t cY = (size_t)(clamp(m_Position.y, 0.f, 22.f) + 0.5f);

		switch (m_CurrentDirection) // move in direction
		{
		case Direction::Left:
			m_Position.x -= speed;
			break;
		case Direction::Right:
			m_Position.x += speed;
			break;
		case Direction::Up:
			m_Position.y -= speed;
			break;
		case Direction::Down:
			m_Position.y += speed;
			break;
		}

		//make the side wall let us trough
		if (m_Position.x < 0.f)
		{
			m_Position.x = 19.f;
		}

		if (m_Position.x > 19.0f)
		{
			m_Position.x = 0.f;
		}

		if (m_CurrentDirection == Direction::None && m_NextDirection != Direction::None && tilemap.GetDirectionFlag(cX, cY, (size_t)m_NextDirection))
		{// if the current direction is None and the next input direction is not none and we can go in that direction
			m_CurrentDirection = m_NextDirection; // switch directions
			m_NextDirection = Direction::None; // set next to none
		}

		if (m_CurrentDirection != Direction::None) // if our  current direction is not None ( we are not at base)
		{
			m_DistanceLeft -= speed; // reduce the distance of travel

			if (m_DistanceLeft <= 0.f) //if we have traveled 1 tile
			{
				if (!tilemap.GetDirectionFlag(cX, cY, (size_t)m_CurrentDirection)) // if the tile we are on dosent have the current direction direction flag set
				{
					m_CurrentDirection = Direction::None;// make curent dir none
				}

				if (m_NextDirection != Direction::None) // if we have a pending direction command
				{
					if (tilemap.GetDirectionFlag(cX, cY, (size_t)m_NextDirection)) // and if the tile we are one has the next direction tag set
					{
						m_CurrentDirection = m_NextDirection;// go in that direction
					}
				}
				m_DistanceLeft += 1.0f;// add distace to travel
			}
		}

		Rect rect(m_Position.x + 0.05f, m_Position.y + 0.05f, 0.9f, 0.9f);//construct a collision rect;

		//get all intersections
		auto intersections = tilemap.Intersect(rect);

		for (auto &i : intersections) // go trough them
		{
			if (i.type == Coin) // if its a coin
			{
				++m_Points; // increase points
				tilemap.Set(i.x, i.y, Empty); //remove coin 
				tileSurface->Clear(); // clear tilemap render
				tileSurface->RenderTileset(gridSurf, tilemap.GetGrid(), 19, 32.0f);// re - render the grid without the coin.
			}
		}

		//set directions accortind to input keys
		if (input.GetKey(Left))
		{
			m_NextDirection = Direction::Left;
		}

		if (input.GetKey(Right))
		{
			m_NextDirection = Direction::Right;
		}

		if (input.GetKey(Up))
		{
			m_NextDirection = Direction::Up;
		}

		if (input.GetKey(Down))
		{
			m_NextDirection = Direction::Down;
		}
	}
}