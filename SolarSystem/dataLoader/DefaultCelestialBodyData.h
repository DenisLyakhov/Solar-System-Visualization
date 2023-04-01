#pragma once

#include<iostream>
#include <sstream>
#include <map>
#include <string>


using namespace std;

class DefaultCelestialBodyData {
	public:
		static const map<string, string> MERCURY;
		static const map<string, string> VENUS;
		static const map<string, string> EARTH;
		static const map<string, string> MARS;
		static const map<string, string> JUPITER;
		static const map<string, string> SATURN;
		static const map<string, string> URANUS;
		static const map<string, string> NEPTUNE;
		static const map<string, string> SUN;


		static map<string, string> getDefaultDataBasedOnName(string name) {
			if (name == "Mercury" || name == "mercury" || name == "MERCURY") return MERCURY;
			else if (name == "Venus" || name == "venus" || name == "VENUS") return VENUS;
			else if (name == "Earth" || name == "earth" || name == "EARTH") return EARTH;
			else if (name == "Mars" || name == "mars" || name == "MARS") return MARS;
			else if (name == "Jupiter" || name == "jupiter" || name == "JUPITER") return JUPITER;
			else if (name == "Saturn" || name == "saturn" || name == "SATURN") return SATURN;
			else if (name == "Uranus" || name == "uranus" || name == "URANUS") return URANUS;
			else if (name == "Neptune" || name == "neptune" || name == "NEPTUNE") return NEPTUNE;
			else if (name == "Sun" || name == "sun" || name == "SUN") return SUN;
		}
};