#pragma once

#pragma once
#define CURL_STATICLIB

#include<iostream>
#include <sstream>
#include<DataParser.h>
#include<DataLoader.h>
#include<map>
#include<DefaultCelestialBodyData.h>

using namespace std;

class Planet {


	public:
		string name;

		Planet(string name) {
			this->name = name;
			this->loader = createLoader(DefaultCelestialBodyData::getDefaultDataBasedOnName(name));
		}
		Planet(string name, map<string,string> arguments) {
			this->name = name;
			this->loader = createLoader(arguments);
		}


		list<VectorData> getVectorData() {
			string data = loader.getData().text;
			return DataParser::parseDataVector(data);
		}


		string getName() { return name;}
		DataLoader getLoader() { return this->loader;}
	private:
		DataLoader loader;
		DataLoader createLoader(map<string,string> arguments) {
			DataLoader loader = DataLoader();
			map<string, string>::iterator it;

			for (it = arguments.begin(); it != arguments.end(); it++) {
				if (it->first == "format") loader.setFormat(it->second);
				else if (it->first == "COMMAND") loader.setCommand(it->second);
				else if (it->first == "OBJ_DATA") loader.setObjData(it->second);
				else if (it->first == "CENTER") loader.setCenter(it->second);
				else if (it->first == "START_TIME") loader.setStartTime(it->second);
				else if (it->first == "STOP_TIME") loader.setStopTime(it->second);
				else if (it->first == "STEP_SIZE") loader.setStepSize(it->second);
				else if (it->first == "MAKE_EPHEM") loader.setMakeEphem(it->second);
				else if (it->first == "EPHEM_TYPE") loader.setEphemType(it->second);
				else if (it->first == "QUANTITIES") loader.setQuantities(it->second);
			}
			return loader;
		}
};