#include <math.h>
#include <algorithm>
#include <iostream>
#include <random>

#include "Geometry.h"
#include "Ghost.h"
#include "Timer.h"

namespace Pacman
{
	Ghost::Ghost(float x, float y, const std::shared_ptr<Pacman>& pacman) // initilialize  ghost 
	{
		m_SpawnPoint.x = x; // remebers the spawn position coordinates
		m_SpawnPoint.y = y;

		m_Position.x = x;
		m_Position.y = y;

		m_TargetPacman = pacman; //sets target pacman

		m_State = GhostState::Base; // state is at base until pacman moves and starts the chase

		m_CurrentDirection = Direction::Left;

		m_DistanceLeft = 1.0f; // set the distance to one tile
	}
	void Ghost::Reset(Direction dir)
	{
		m_Position.x = m_SpawnPoint.x; // makes position the spawn point
		m_Position.y = m_SpawnPoint.y;

		m_State = GhostState::Base;// state is at base until pacman moves and starts the chase

		m_CurrentDirection = dir; //assign the paramer direction to current dir.

		m_DistanceLeft = 1.0f; // set the distance to one tile

	}

	void Ghost::OnCollision(const std::shared_ptr<ICollider>& collidedWith) // playes already has defined what to do when he hits a ghost.
	{
		if (collidedWith->GetType() == Type::PowerPlayer) // if a powered up player hits us, he kills the ghost and returns sets it state to return to base.
			m_State = GhostState::ReturnToBase;
	}

	void Ghost::Update(Tilemap& tilemap, float deltaTime)
	{
		auto targetPos = UpdateTarget(); // update the target tile corresponding to each ghosts

		if (m_State == GhostState::Chase || m_State == GhostState::ReturnToBase) // if we are chasing
		{
			size_t t_x = (size_t)(targetPos.x + 0.5f); //target x
			size_t t_y = (size_t)(targetPos.y + 0.5f); //target y

			size_t c_x = (size_t)(m_Position.x + 0.5f); //ghost x 
			size_t c_y = (size_t)(m_Position.y + 0.5f); //ghost y

			if (m_State == GhostState::ReturnToBase && m_Position.x == m_SpawnPoint.x && m_Position.y == m_SpawnPoint.y) //we have reached base;
			{
				Reset(Direction::Up);
				m_State = GhostState::Chase;
			}
			float speed = 5.f * deltaTime; //our speed. Speed is frame rate independant thanks to deltaTime

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

			m_DistanceLeft -= speed; // reduce the distance we have left

			if (m_Position.x < 0.f) // handles the middle tunnel. Ghosts cant go trough it so we force a direction turn.
			{
				m_CurrentDirection = Direction::Right;
			}

			if (m_Position.x > 18.f)
			{
				m_CurrentDirection = Direction::Left;
			}

			if (m_DistanceLeft <= 0.f) // if we have used all of the travel distance we are at choice making situation
			{
				m_DistanceLeft += 1.0f; // sets the new move distance

				Direction last = m_CurrentDirection; // last chosen direction is our current direction

				float closestDist = std::numeric_limits<float>::max(); // set the closest distance to infinity

				float dist = pointPointDistSquare(c_x - 1, c_y, t_x, t_y); // returns the squared distance between the future left position and the target position. Since the distances are only comapred, 
				// it is not needed to take the heavy sqrt operation.

				if (last != Direction::Right && dist < closestDist && tilemap.GetDirectionFlag(c_x, c_y, (size_t)Direction::Left))
				{ // if the ghost last direction was not right and the distance measured is better than the current and the tile under us says we can go left
					m_CurrentDirection = Direction::Left;//go left
					closestDist = dist; // set the closest dist to this dist
				}

				dist = pointPointDistSquare(c_x + 1, c_y, t_x, t_y); // same as above. Future for right pos

				if (last != Direction::Left && dist < closestDist && tilemap.GetDirectionFlag(c_x, c_y, (size_t)Direction::Right))
				{// if the ghost last direction was not left and the distance measured is better than the current and the tile under us says we can go right
					m_CurrentDirection = Direction::Right;//go right
					closestDist = dist;// set the closest dist to this dist
				}

				dist = pointPointDistSquare(c_x, c_y - 1, t_x, t_y);

				if (last != Direction::Down && dist < closestDist && tilemap.GetDirectionFlag(c_x, c_y, (size_t)Direction::Up))
				{// if the ghost last direction was not down and the distance measured is better than the current and the tile under us says we can go up
					m_CurrentDirection = Direction::Up;// go up
					closestDist = dist;// set the closest dist to this dist
				}

				dist = pointPointDistSquare(c_x, c_y + 1, t_x, t_y);

				if (last != Direction::Up && dist < closestDist && tilemap.GetDirectionFlag(c_x, c_y, (size_t)Direction::Down))
				{// if the ghost last direction was not up and the distance measured is better than the current and the tile under us says we can go down
					m_CurrentDirection = Direction::Down;// go down
					closestDist = dist;// set the closest dist to this dist
				}
			}
		}

		if (m_State == GhostState::Frightened) // if we are frightened the ghosts generete random target tiles to move to. The rest of the function is the same as above
		{
			std::random_device randomDevice; // creates random devide to be used by our random generator engine
			std::mt19937 randomEngine(randomDevice()); // engine
			std::uniform_real_distribution<> randomGen(1, 22); //random generator and range

			Point target;

			target.x = (float)randomGen(randomEngine); // generate random points
			target.y = (float)randomGen(randomEngine);

			size_t t_x = (size_t)(target.x + 0.5f); //target x
			size_t t_y = (size_t)(target.y + 0.5f); //target y

			size_t c_x = (size_t)(m_Position.x + 0.5f); //ghost x 
			size_t c_y = (size_t)(m_Position.y + 0.5f); //ghost y

			float speed = 3.f * deltaTime; //our speed. Speed is frame rate independant thanks to deltaTime

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

			m_DistanceLeft -= speed; // reduce the distance we have left

			if (m_Position.x < 0.f)
			{
				m_CurrentDirection = Direction::Right;
			}

			if (m_Position.x > 18.f)
			{
				m_CurrentDirection = Direction::Left;
			}

			if (m_DistanceLeft <= 0.f) // if we have used all of the travel distance we are at choice making situation
			{
				m_DistanceLeft += 1.0f; // sets the new move distance

				Direction last = m_CurrentDirection; // last chosen direction is our current direction

				float closestDist = std::numeric_limits<float>::max(); // set the closest distance to infinity

				float dist = pointPointDistSquare(c_x - 1, c_y, t_x, t_y);// returns the squared distance between the future left position and the target position. Since the distances are only comapred, 
				// it is not needed to take the heavy sqrt operation.

				if (last != Direction::Right && dist < closestDist && tilemap.GetDirectionFlag(c_x, c_y, (size_t)Direction::Left))
				{ // if the ghost last direction was not right and the distance measured is better than the current and the tile under us says we can go left
					m_CurrentDirection = Direction::Left;//go left
					closestDist = dist; // set the closest dist to this dist
				}

				dist = pointPointDistSquare(c_x + 1, c_y, t_x, t_y); // same as above. Future for right pos

				if (last != Direction::Left && dist < closestDist && tilemap.GetDirectionFlag(c_x, c_y, (size_t)Direction::Right))
				{// if the ghost last direction was not left and the distance measured is better than the current and the tile under us says we can go right
					m_CurrentDirection = Direction::Right;//go right
					closestDist = dist;// set the closest dist to this dist
				}

				dist = pointPointDistSquare(c_x, c_y - 1, t_x, t_y);

				if (last != Direction::Down && dist < closestDist && tilemap.GetDirectionFlag(c_x, c_y, (size_t)Direction::Up))
				{// if the ghost last direction was not down and the distance measured is better than the current and the tile under us says we can go up
					m_CurrentDirection = Direction::Up;// go up
					closestDist = dist;// set the closest dist to this dist
				}

				dist = pointPointDistSquare(c_x, c_y + 1, t_x, t_y);

				if (last != Direction::Up && dist < closestDist && tilemap.GetDirectionFlag(c_x, c_y, (size_t)Direction::Down))
				{// if the ghost last direction was not up and the distance measured is better than the current and the tile under us says we can go down
					m_CurrentDirection = Direction::Down;// go down
					closestDist = dist;// set the closest dist to this dist
				}
			}
		}
	}

	Point RedGhost::UpdateTarget()
	{
		Point target;

		if (m_TargetPacman->GetDirection() != Direction::None && m_TargetPacman->GetState() == PacState::Normal) // if the player has moved and is in normal state
		{
			m_State = GhostState::Chase;// chase him
			target = m_TargetPacman->GetPosition();
			return target;
		}
		if (m_State == GhostState::ReturnToBase) // if we are to return to base
		{
			target.x = m_SpawnPoint.x;
			target.y = m_SpawnPoint.y;
			return target;
		}
		if (m_TargetPacman->GetState() == PacState::PowerUp) // if he ate a powerup
		{
			m_State = GhostState::Frightened; // scare
		}
	}

	Point OrangeGhost::UpdateTarget()
	{
		Point target(0, 24);

		if (m_TargetPacman->GetDirection() != Direction::None && m_TargetPacman->GetState() == PacState::Normal) // if the player has moved and is in normal state
		{
			m_State = GhostState::Chase; //chase him

			if (pointDistanceCheck(GetPosition(), m_TargetPacman->GetPosition()) > 7.f) // if we are too far from pacman, clyde behaves like blinky
				target = m_TargetPacman->GetPosition();

			return target;
		}
		

		if (m_State == GhostState::ReturnToBase)// if we are to return to base
		{
			target.x = m_SpawnPoint.x;
			target.y = m_SpawnPoint.y;
			return target;
		}
		if (m_TargetPacman->GetState() == PacState::PowerUp) // if he ate a powerup
			m_State = GhostState::Frightened; // scare

		else // else we set his target to be just below lower left corner.
			return target;


	}
	Point PinkGhost::UpdateTarget()
	{
		Direction pacDir = m_TargetPacman->GetDirection();
		Point pacPos = m_TargetPacman->GetPosition();

		if (m_TargetPacman->GetDirection() != Direction::None && m_TargetPacman->GetState() == PacState::Normal)// if the player has moved and is in normal state
			m_State = GhostState::Chase;//chase him

		if (m_TargetPacman->GetState() == PacState::PowerUp) // if he ate a powerup
			m_State = GhostState::Frightened; //scare

		if (m_State == GhostState::ReturnToBase)
		{
			Point target;
			target.x = 9.f;
			target.y = 8.f;
			return target;
		}
		// check player direction and get a target 4 tiles ahead of him
		if (pacDir == Direction::Up)
		{
			pacPos.y -= 4.f;
			if (pacPos.y < 0.f)
				pacPos.y = 0.f;
			return pacPos;
		}
		if (pacDir == Direction::Down)
		{
			pacPos.y += 4.f;
			if (pacPos.y > 23.f)
				pacPos.y = 23.f;
			return pacPos;
		}
		if (pacDir == Direction::Left)
		{
			pacPos.x -= 4.f;
			if (pacPos.x < 0.f)
				pacPos.x = 0.f;
			return pacPos;
		}
		if (pacDir == Direction::Right)
		{
			pacPos.x += 4.f;
			if (pacPos.x > 19.f)
				pacPos.x = 19.f;
			return pacPos;
		}
	}
	Point BlueGhost::UpdateTarget()
	{
		if (m_TargetPacman->GetDirection() != Direction::None && m_TargetPacman->GetState() == PacState::Normal)// if the player has moved and is in normal state
			m_State = GhostState::Chase; // chase him

		Direction pacDir = m_TargetPacman->GetDirection();

		Point result;
		Point blinkyPos = m_RedGhost->GetPosition();
		Point pacPos = m_TargetPacman->GetPosition();

		if (m_TargetPacman->GetState() == PacState::PowerUp) // if player ate a powerup 
			m_State = GhostState::Frightened; //scare

		if (m_State == GhostState::ReturnToBase)
		{
			result.x = 9.f;
			result.y = 8.f;
			return result;
		}

		//uses blinky pos and 2 tiles ahead of pacman to determine target
		if (pacDir == Direction::Up)
		{
			pacPos.y -= 2.f;
			if (pacPos.y < 0.f)
				pacPos.y = 0.f;

			result.x = clamp((pacPos.x - blinkyPos.x), 0.f, 19.f);
			result.y = clamp((pacPos.y - blinkyPos.y), 0.f, 23.f);

			return result;
		}
		if (pacDir == Direction::Down)
		{
			pacPos.y += 2.f;
			if (pacPos.y > 23.f)
				pacPos.y = 23.f;

			result.x = clamp((pacPos.x - blinkyPos.x), 0.f, 19.f);
			result.y = clamp((pacPos.y - blinkyPos.y), 0.f, 23.f);

			return result;
		}
		if (pacDir == Direction::Left)
		{
			pacPos.x -= 2.f;
			if (pacPos.x < 0.f)
				pacPos.x = 0.f;

			result.x = clamp((pacPos.x - blinkyPos.x), 0.f, 19.f);
			result.y = clamp((pacPos.y - blinkyPos.y), 0.f, 23.f);

			return result;
		}
		if (pacDir == Direction::Right)
		{
			pacPos.x += 2.f;
			if (pacPos.x > 19.f)
				pacPos.x = 19.f;

			result.x = clamp((pacPos.x - blinkyPos.x), 0.f, 19.f);
			result.y = clamp((pacPos.y - blinkyPos.y), 0.f, 23.f);

			return result;
		}
	}
}

