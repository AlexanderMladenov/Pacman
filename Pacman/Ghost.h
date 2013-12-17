#ifndef __GHOST_H
#define __GHOST_H

#include "CollisionDetection.h"
#include "Tilemap.h"
#include "Pacman.h"

namespace Pacman
{
	enum class GhostState
	{
		Chase = 0,
			Scatter,
			Frightened,
			Base,
			ReturnToBase,
	};

	class Ghost : public ICollider 	// Base ghost class. Handles operations wich will be the same for all ghosts. 
	{
	public:

		Ghost(float x, float y, const std::shared_ptr<Pacman>& pacman); //makes a ghost at x,y with default target pacman

		virtual ~Ghost() { m_TargetPacman = nullptr; }//destructor sets the target pointer to nullptr.

		void Reset(Direction dir); // resets the ghost as if just initilised. Parameter dir sets the start direction for the ghost (Used mainly on Blue, Orange and Pink ghosts).

		void Update(Tilemap& tilemap, float deltaTime); //updates the ghost position to make it go follow a target tile. Makes the ghost behavior almost like the original


		Rect GetRect() const override { return Rect(m_Position.x, m_Position.y, 0.9f, 0.9f); } 		//get collision rect for ghost current position

		void OnCollision(const std::shared_ptr<ICollider>& collidedWith) override; // if a powered pacman hits the ghost, return ghost to base.

		//returns the ICollider type depending on current state
		Type GetType() const override
		{
			if (m_State == GhostState::ReturnToBase)
				return Type::DeadGhost;

			if (m_State == GhostState::Frightened )
				return Type::FrghtGhost;

			else
				return Type::Ghost;
		}

		GhostState GetState() const { return m_State; } //get current state

		inline const Point& GetPosition() const { return m_Position; } //returns current ghost position

	protected: // properties that the different ghosts need to use in order to change targets
	
		std::shared_ptr<Pacman> m_TargetPacman; //target pacman

		Direction m_CurrentDirection;

		Point m_SpawnPoint;

		GhostState m_State; 

		virtual Point UpdateTarget() = 0; //pure virtual function that each ghost realises specifically for their target method

	private: //properties thst the different ghost dont need to have acces to

		Point m_Position;

		Point m_Target; // target tile to move to

		float m_DistanceLeft; //distace left to move
	};

	class RedGhost : public Ghost // Blinky.
	{
	protected:
		Point UpdateTarget() override; // sets the target to pacmans tile in chase mode
	public:
		RedGhost(float x, float y, const std::shared_ptr<Pacman>& pacman) : Ghost(x, y, pacman) {}
	};

	class PinkGhost : public Ghost // Pinky
	{
	protected:
		Point UpdateTarget() override; // sets target tile 4 tiles ahed in pacmans direction
	public:
		PinkGhost(float x, float y, const std::shared_ptr<Pacman>& pacman) : Ghost(x, y, pacman) { m_CurrentDirection = Direction::Up; }
	};

	class BlueGhost : public Ghost // Inky
	{
	private:
		std::shared_ptr<RedGhost> m_RedGhost;
	protected:
		Point UpdateTarget() override; // sets target tile in a weird way
	public:
		~BlueGhost(){ m_RedGhost = nullptr; } // set the additional pointer to null

		BlueGhost(float x, float y, const std::shared_ptr<Pacman>& pacman, const std::shared_ptr<RedGhost>& redghost) : Ghost(x, y, pacman), m_RedGhost(redghost){ m_CurrentDirection = Direction::Up; }
	};

	class OrangeGhost : public Ghost // Clyde
	{
	protected:
		Point UpdateTarget() override; // chase pacman if far away, go to lower left corner if far away.
	public:
		OrangeGhost(float x, float y, const std::shared_ptr<Pacman>& pacman) : Ghost(x, y, pacman){ m_CurrentDirection = Direction::Up; }
	};
}
#endif