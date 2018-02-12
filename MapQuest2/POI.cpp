#include "POI.h"
#include "Coordinate.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

const static double EarthRadiusKm = 6367;

class POI::POIImpl
{
public:
	void load(const std::string& inputFile)
	{
		std::ifstream in(inputFile);

		if (!in)
		{
			throw std::exception("Error: Unable to open input file");
		}

		if (in.peek() == std::ifstream::traits_type::eof())
		{
			throw std::exception("Error: Input file is empty.");
		}
		
		std::string data;
		std::vector<std::string> tokens;
		std::string token;

		double latitude, longitude;

		while (!in.eof())
		{
			std::getline(in, data, '\n');

			std::istringstream tokenStream(data);
			tokens.clear();

			latitude = longitude = 0.0;

			while (std::getline(tokenStream, token, ','))
			{
				tokens.push_back(token);
			}

			bool validateOK = true;
			
			if (tokens.size() == 3)
			{
				if (str_to_double(tokens[0], latitude))
				{
					if (str_to_double(tokens[1], longitude))
					{
						// Here using C++11 unique_ptr
						auto coordinate = std::make_unique<Coordinate>(latitude, longitude, tokens[2]);

						// storing all POI into vector.
						_coordinates.push_back(std::move(coordinate));
					}
					else
					{
						validateOK = false;
					}
				}
				else
				{
					validateOK = false;
				}
			}
			else
			{
				validateOK = false;
			}

			if ( !validateOK )
			{
				std::cout << "Warning: Invalid POI data. Skipping Haversine distance calculation: " << data << "\n";
			}
		}
	}

	void calculate()
	{
		for (auto const& coordinate : _coordinates)
		{
			std::vector<std::pair<double, std::string> > haversineDistances;

			for (auto const& loopCoordinate : _coordinates)
			{
				// if this is POI is question, then skip it.
				if (loopCoordinate->Name() == coordinate->Name())
				{
					// store first entry as POI is question. Storing -1.0 and POI name. Assuming result of haversine is never negative.
					haversineDistances.push_back(std::make_pair(-1.0, coordinate->Name()));
					continue;
				}

				// calculate Haversine (round off) between POI ins question and others.
				double distance = std::ceil(HaversineDistance(coordinate, loopCoordinate));

				// And save it in vector as pair value ( distance and POI Name)
				haversineDistances.push_back(std::make_pair(distance, loopCoordinate->Name()));
			}

			// Sort it by ascending order
			std::sort(haversineDistances.begin(), haversineDistances.end());

			// store it into vector
			_distances.push_back(haversineDistances);
		}
	}

	void display()
	{
		for (auto& row_value : _distances)
		{
			for (auto& col_value : row_value)
			{
				if (col_value.first < 0)
				{
					std::cout << "Distance from " << col_value.second << ":" << "\n";
					continue;
				}

				std::cout << col_value.second << ": " << col_value.first << " km" << "\n";
			}
			std::cout << "\n";
		}
	}

	/******  Helper function *****/
	inline double DegreeToRadian(double angle)
	{
		return M_PI * angle / 180.0;
	}

	double HaversineDistance(std::unique_ptr<Coordinate> const& p1, std::unique_ptr<Coordinate> const& p2)
	{
		double latRad1 = DegreeToRadian(p1->Latitude());
		double latRad2 = DegreeToRadian(p2->Latitude());
		double lonRad1 = DegreeToRadian(p1->Longitude());
		double lonRad2 = DegreeToRadian(p2->Longitude());

		double diffLa = latRad2 - latRad1;
		double doffLo = lonRad2 - lonRad1;

		double computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
		return 2 * EarthRadiusKm * computation;
	}

	bool str_to_double(const std::string& str, double& d)
	{
		try 
		{
			d = std::stod(str);
		}
		catch (const std::invalid_argument&) 
		{
			std::cout << "Argument is invalid to convert to double: " << str << "\n";
			return false;
		}
		catch (const std::out_of_range&) {
			std::cout << "Argument is out of range for a double: " << str << "\n";
			return false;
		}

		return true;
	}

	/****  Variables ****/
	// This store instance of Coordinates (POIs) into vector as smart pointer
	std::vector<std::unique_ptr<Coordinate>> _coordinates;

	// This store 2D vector with value as pair (haversine, POI Name)
	std::vector<std::vector<std::pair<double, std::string>>> _distances;
};


POI::POI() : _impl(new POIImpl) {}

POI::~POI()
{
	delete _impl;
}

// This is if use unique_ptr
//POI::~POI() = default;


void POI::load(const std::string& inputFile)
{
	_impl->load(inputFile);
}

void POI::calculate()
{
	_impl->calculate();
}

void POI::display()
{
	_impl->display();
}