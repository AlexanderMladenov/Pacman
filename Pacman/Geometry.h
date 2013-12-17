#ifndef __GEOMETRY_H
#define __GEOMETRY_H

#include <math.h>

namespace Pacman
{
	//simple point struct
	struct Point
	{
		Point(){};
		Point(float X, float Y) : x(X), y(Y){};
		float x;
		float y;
	};
	//simple rect struct
	struct Rect
	{
		Rect(){};
		Rect(float X, float Y, float w, float h) : x(X), y(Y), width(w), height(h){}; // construct a rect with the arguments
		Rect(const Point& p, float w, float h) : width(w), height(h){ x = p.x; y = p.y; }; // construct a rect from point
		float x;
		float y;
		float width;
		float height;

	};

	inline bool pointRectTest(const Point& p, const Rect& r) // test if a point is inside a rectangle 
	{
		return p.x >= r.x && p.x <= r.x + r.width && p.y >= r.y && p.y <= r.y + r.height;
	}

	inline bool rectRectTest(const Rect& a, const Rect& b) // test if the 2 rectangles overlap
	{
		return !(a.x > b.x + b.width || a.x + a.width < b.x || a.y > b.y + b.height || a.y + a.height < b.y);
	}

	inline float rectDistanceCheck(const Rect& a, const Rect& b) // return euclidian distance between two rects
	{
		return sqrtf(powf((b.x + b.width / 2.0f) - (a.x + a.width / 2.0f), 2) + powf((b.y + b.height / 2.0f) - (a.y + a.height / 2.0f), 2));
	}
	inline float pointDistanceCheck(const Point& a, const Point& b)//  return euclidian distance between two points;
	{
		return sqrtf(powf((b.x - a.x), 2) + powf((b.y - a.y), 2));
	}
	inline float pointPointDistSquare(float x1, float y1, float x2, float y2) // return the squred distance between two points
	{
		float dx = x2 - x1;
		float dy = y2 - y1;
		return dx * dx + dy * dy;
	}
	inline float clamp(float n, float min, float max) // clamps a target float number n between min and max values for it.
	{
		return (n < min) ? min : ((n > max) ? max : n);
	}
}

#endif