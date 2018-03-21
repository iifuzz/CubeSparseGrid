#pragma once
#include <limits>
#include "Point3D.h"

class Range3D
{
public:
	Point3D Min;
	Point3D Max;

	Range3D(Point3D const& min, Point3D const& max) : Min{ min }, Max{ max }{}

	bool IsNull() 
	{
		return Min.X > Max.X || Min.Y > Max.Y || Min.Z > Max.Z;
	}

	void Extend(Point3D const& p) {
		if (Min.X > p.X)
			Min.X = p.X;
		if (Min.Y > p.Y)
			Min.Y = p.Y;
		if (Min.Z > p.Z)
			Min.Z = p.Z;

		if (Max.X < p.X)
			Max.X = p.X;
		if (Max.Y < p.Y)
			Max.Y = p.Y;
		if (Max.Z < p.Z)
			Max.Z = p.Z;
	}

	double Distance() {
		return Min.DistanceTo(Max);
	}

	bool Contains(Point3D const& p) {
		return p.X >= Min.X && p.X <= Max.X && p.Y >= Min.Y && p.Y <= Max.Y && p.Z >= Min.Z && p.Z <= Max.Z;
	}

	static Range3D From(Point3D const& min, Point3D const& max) { return Range3D(min, max); }
	static Range3D FromNullRange() { 
		double lowest = std::numeric_limits<double>::lowest();
		double highest = std::numeric_limits<double>::max();

		return Range3D(Point3D::From(highest, highest, highest), Point3D::From(lowest, lowest, lowest));
	}

};