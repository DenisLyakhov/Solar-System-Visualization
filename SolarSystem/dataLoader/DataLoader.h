#pragma once
#define CURL_STATICLIB

#include<iostream>
#include <cpr/cpr.h>
#include <sstream>

using namespace std;
using namespace cpr;

class DataLoader {
	
	private:
		//Czêœci stringu
		string URL_BASE;
		string FORMAT;
		string COMMAND;
		string OBJ_DATA;
		string MAKE_EPHEM;
		string EPHEM_TYPE;
		string VECTOR;
		string CENTER;
		string START_TIME;
		string STOP_TIME;
		string STEP_SIZE;
		string QUANTITIES;

	public:
		virtual Response getData() {
			return Get(Url{ buildUrl() });
		}
		DataLoader() {
			this->URL_BASE = "https://ssd.jpl.nasa.gov/api/horizons.api?";
			setFormat("text");
			setCommand("1");
			setMakeEphem("'YES'");
			setEphemType("'VECTOR'");
			setStartTime("'2006-01-01'");
			setStopTime("'2006-01-20'");
			setStepSize("'1%20d'");
			setObjData("'YES'");
			setCenter("");

		}
		DataLoader(	string FORMAT,string COMMAND,string OBJ_DATA,string MAKE_EPHEM,string EPHEM_TYPE,string VECTOR,
					string CENTER,string START_TIME,string STOP_TIME,string STEP_SIZE,string QUANTITIES) {
			this->URL_BASE = "https://ssd.jpl.nasa.gov/api/horizons.api?";
			this->FORMAT = FORMAT;
			this->COMMAND = COMMAND;
			this->OBJ_DATA = OBJ_DATA;
			this->MAKE_EPHEM = MAKE_EPHEM;
			this->EPHEM_TYPE = EPHEM_TYPE;
			this->VECTOR = VECTOR;
			this->CENTER = CENTER;
			this->START_TIME = START_TIME;
			this->STOP_TIME = STOP_TIME;
			this->STEP_SIZE = STEP_SIZE;
			this->QUANTITIES = QUANTITIES;
		}
		virtual string buildUrl() {
			string returnString;

			returnString = this->URL_BASE;
			returnString = addPart(returnString, this->FORMAT);
			returnString = addPart(returnString, this->COMMAND);
			returnString = addPart(returnString, this->OBJ_DATA);
			returnString = addPart(returnString, this->MAKE_EPHEM);
			returnString = addPart(returnString, this->EPHEM_TYPE);
			returnString = addPart(returnString, this->VECTOR);
			returnString = addPart(returnString, this->CENTER);
			returnString = addPart(returnString, this->START_TIME);
			returnString = addPart(returnString, this->STOP_TIME);
			returnString = addPart(returnString, this->STEP_SIZE);
			returnString = addPart(returnString, this->QUANTITIES);

			return returnString;
		}

		//Settery

		void setFormat(string format) {this->FORMAT = "format=" + format; }
		void setCommand(string command) {this->COMMAND = "COMMAND=" + command; }
		void setObjData(string objData) {this->OBJ_DATA = "OBJ_DATA=" + objData; }
		void setMakeEphem(string makeEphem) {this->MAKE_EPHEM = "MAKE_EPHEM=" + makeEphem; }
		void setEphemType(string ephemType) {this->EPHEM_TYPE = "EPHEM_TYPE=" + ephemType; }
		void setVector(string vector) {this->VECTOR = vector;}
		void setCenter(string center) {this->CENTER= "CENTER=" + center; }
		void setStartTime(string startTime) {this->START_TIME = "START_TIME=" + startTime; }
		void setStopTime(string stopTime) {this->STOP_TIME = "STOP_TIME=" + stopTime; }
		void setStepSize(string stepSize) {this->STEP_SIZE = "STEP_SIZE=" + stepSize; }
		void setQuantities(string quantities) {this->QUANTITIES = "QUANTITIES=" + quantities; }
		/*
		loader.setFormat("format=text");
		loader.setCommand("COMMAND='499'");
		loader.setObjData("OBJ_DATA='YES'");
		loader.setCenter("CENTER='500@399'");
		loader.setStartTime("START_TIME='2006-01-01'");
		loader.setStopTime("STOP_TIME='2006-01-20'");
		loader.setStepSize("STEP_SIZE='1%20d'");
		loader.setQuantities("QUANTITIES='1,9,20,23,24,29'");
		loader.setMakeEphem("MAKE_EPHEM='YES'");
		loader.setEphemType("EPHEM_TYPE='VECTOR'");
		*/
	private:
		string addPart(string url,string urlPart) {
			if (urlPart == "") return url;
			else if (urlPart == this->FORMAT) return url + urlPart;
			else return url + "&" + urlPart;
		}

};

