#ifndef __COLLISION_DETECTION_H
#define __COLLISION_DETECTION_H

#include <memory>
#include <set>

#include "Geometry.h"

namespace Pacman
{
	enum class Type
	{
		Power = 0,
			Player = 1,
			Ghost = 2,
			PowerPlayer,
			FrghtGhost,
			DeadGhost,
			None,
	};

	//Abstract class for creating a collider interface
	class ICollider
	{
	public:
		virtual ~ICollider(){};

		virtual Rect GetRect() const = 0; // to return a collsion rect

		virtual void OnCollision(const std::shared_ptr<ICollider>& collidedWith) = 0; //OnCollision method, handling what shall the object do if it hits another object.

		virtual Type GetType() const = 0;//gets the object type using the above enum
	};

	//Collison Manager, holdig a set of pointers to all objects using the above interface.
	class CollisionDetection
	{
	public:
		typedef std::shared_ptr<ICollider> ColliderPtr;

		void Insert(const ColliderPtr& collider) // inserts a new pointer to the array
		{
			m_Colliders.insert(collider);
		}

		void ResolveCollisions() // resolves colision between obejcts using callback to them.
		{
			for (auto& a : m_Colliders)
			{
				for (auto& b : m_Colliders)
				{
					if (a == b) // if both objects are the same, skip a turn 
					{
						continue;
					}

					if (rectRectTest(a->GetRect(), b->GetRect())) // if they are not the same and have collided
					{
						//resolve collisioin betweeen them
						a->OnCollision(b);
						b->OnCollision(a);
					}
				}
			}
		}

	private:
		std::set<ColliderPtr> m_Colliders; // set of pointer to objects

	};
}

#endif