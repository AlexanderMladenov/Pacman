#ifndef __PACMAN_H
#define __PACMAN_H

#include "CollisionDetection.h"
#include "SDLRenderer.h"

namespace Pacman
{
	enum class PacState
	{
		Normal = 0,
			Base,
			PowerUp,
			Dead,
	};

	class Pacman :public ICollider
	{
	public:

		Pacman(float x, float y); // constructs a pacman at x,y

		~Pacman(){};

		void Reset(); //resets pacman to starting state

		inline const Point& GetPosition() const{ return m_Position; }//get the pacman position

		Direction GetDirection() const { return m_CurrentDirection; } // get the pacman's direction. mostly used in Pinky

		PacState GetState() const  { return m_State; }; //return the state


		Rect GetRect() const override { return Rect(m_Position.x + 0.05f, m_Position.y + 0.05f, 0.9f, 0.9f); } 	//returns a collision rect

		void OnCollision(const std::shared_ptr<ICollider>& collidedWith) override; // if we hit a ghost, makes state dead and resets everything. if we hit a powerup, eat it, if we hit a frightened ghost, eat it

		//return the type of the ICollider depending on current state
		Type GetType() const override
		{
			if (m_State == PacState::PowerUp)
				return Type::PowerPlayer;
			else if (m_State == PacState::Normal)
				return Type::Player;
		};


		//moves the pacman according to input on tilemap. last 2 arguments are for redrawing the coins we eat on the grid. Uses a call to RenderTileset.
		void Update(const SDLInput& input, Tilemap& tilemap, float deltaTime, const std::shared_ptr<SDLRenderTarget>& tileSurface, const std::vector<SDLSurface>& gridSurf);

	private:
		int m_Points; // accumulated points. Currently unused.

		int m_Lives;

		Point m_Position; // current position

		Direction m_CurrentDirection;

		Direction m_NextDirection;

		float m_DistanceLeft; // distance left to move 

		PacState m_State;

		float m_powerDuration; // time elapsed since eating a powerup
	};
}

#endif