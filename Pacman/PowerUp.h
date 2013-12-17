#ifndef __POWER_UP_H
#define __POWER_UP_H

#include "Geometry.h"
#include "CollisionDetection.h"

namespace Pacman
{
	class PowerObj :public ICollider
	{
	public:
		PowerObj(size_t x, size_t y); // constructs a powerup at x,y
		~PowerObj(){}

		Rect GetRect() const override { return Rect(m_Position.x + 0.05f, m_Position.y + 0.05f, 0.8f, 0.8f); }// get a collsion rect

		void OnCollision(const std::shared_ptr<ICollider>& collidedWith); //sets enabled to false moves object out of the grid 

		Type GetType() const { return m_Type; }; //gets the object type using the above enum

		inline bool is_enabled() const { return m_Enabled; }

		inline const Point& GetPosition() const{ return m_Position; }

	private:
		bool m_Enabled;

		Type m_Type;

		Point m_Position;
	};
}
#endif