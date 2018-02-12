#pragma once

// This include is needed if using smart ptr
// #include <memory>
#include <string>

class POI
{
public:
	POI();
	~POI();

	// This load function will read input csv file, parse it and create instance of Coordinates.
	void load(const std::string& input);

	/* This function loops through list of Coordinates (POIs). And for each Coordinate prints out the other four Coordinates in 
	order of increasing Haversine distance from the Coordinate in question */
	void calculate();

	// This function display the result.
	void display();

private:
	class POIImpl;
	// Showing old way to use raw pointer
	POIImpl* _impl;
	
	// New way to use pointer C++11 (smart ptr)
	//std::unique_ptr<POIImpl> _impl;
};