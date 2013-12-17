#include "PowerUp.h"

namespace Pacman
{
	PowerObj::PowerObj(size_t x, size_t y)
	{
		m_Position.x = (float)x;
		m_Position.y = (float)y;

		m_Enabled = true;

		m_Type = Type::Power;
	}
	void PowerObj::OnCollision(const std::shared_ptr<ICollider>& collidedWith)
	{
		if (collidedWith->GetType() == Type::Player)
		{
			m_Enabled = false; //disables powerup
			m_Position.x = -2.f; //removes from screen
		}
		if (collidedWith->GetType() == Type::PowerPlayer)
		{
			m_Enabled = false; //disables powerup
			m_Position.x = -2.f; //removes from screen
		}
	}
}