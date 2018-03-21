#pragma once
class Point3D 
{
public:
	double X, Y, Z;

	Point3D(double x, double y, double z) :X{ x }, Y{ y }, Z{ z } {}
	~Point3D() = default;

	double DistanceTo(Point3D const& p) {
		return sqrt(pow(X - p.X, 2) + pow(Y - p.Y, 2) + pow(Z - p.Z, 2));
	}
	static Point3D From(double x, double y, double z) { return Point3D(x, y, z); }
	
private:
	

};