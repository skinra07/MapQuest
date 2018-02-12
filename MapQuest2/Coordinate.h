#pragma once

#include <string>

class Coordinate
{
public:
	Coordinate(double latitude, double longitude, std::string name) : _latitude(latitude), _longitude(longitude), _name(name)
	{}

	~Coordinate() {}

	double Latitude() const
	{
		return _latitude;
	}

	double Longitude() const
	{
		return _longitude;
	}

	std::string Name() const
	{
		return _name;
	}

private:
	double _latitude;
	double _longitude;
	std::string _name;
};
