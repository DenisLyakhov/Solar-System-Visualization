#pragma once
#define CURL_STATICLIB

#include<iostream>
#include <cpr/cpr.h>
#include <sstream>
#include <VectorData.h>

using namespace std;
using namespace cpr;
//$$SOE $$EOE
class DataParser {

	public:
		//Just wanted to use static method here
		static string parseData(string urlReturn) {
			string parsedText = cutUpAndDown(urlReturn);
			return "";
		}
		static list<VectorData>parseDataVector(string text) {
			string parsedText = cutUpAndDown(text);
			return generateVectorList(parsedText);
		}
	private:
		static string cutUpAndDown(string text) {
			string delimeter = "$$SOE";
			string delimeter2 = "$$EOE";
			string returnString = text.substr(text.find(delimeter),text.length());
			returnString = returnString.substr(6, returnString.find(delimeter2) -6);
			return returnString;
		}
		static list<VectorData> generateVectorList(string text) {

			list<VectorData> vectorList = {};
			while (text.length() > 0) {

				VectorData vector = VectorData();

				string line = removeOneLine(text);
				line = removeOneLine(text);
				vector.setCoordinates(getCordinatesArray(line));

				line = removeOneLine(text);
				vector.setVelocity(getVelocityArray(line));

				line = removeOneLine(text);
				vector.setLT(getLT(line));
				vector.setRR(getRR(line));
				vector.setRG(getRG(line));
				vectorList.push_back(vector);
				

			} 
			return vectorList;
		
		}
		//Parse date from data
		static string getDate(string line) {
			string data = line.substr(nthOccurrence(line," ",3)+1, nthOccurrence(line, " ", 4));
			return data;
		}
		//Parse coordinates from data
		static array<string, 3> getCordinatesArray(string line) {
			// Find X --> first parameter, 4 -> |X = |, second argument is difference between for example Y start and start of X so we calculate length of number
			string xData = line.substr(nthOccurrence(line, "X", 1)+4, nthOccurrence(line, "Y", 1) - (nthOccurrence(line, "X", 1) + 5) );
			string yData = line.substr(nthOccurrence(line, "Y", 1)+4, nthOccurrence(line, "Z", 1) - (nthOccurrence(line, "Y", 1) + 5));
			string zData = line.substr(nthOccurrence(line, "Z", 1)+4, line.length());

			return { xData,yData,zData };
		}
		static array<string, 3> getVelocityArray(string line) {
			
			string xData = line.substr(nthOccurrence(line, "VX", 1) + 4, nthOccurrence(line, "VY", 1) - (nthOccurrence(line, "VX", 1) + 5));
			string yData = line.substr(nthOccurrence(line, "VY", 1) + 4, nthOccurrence(line, "VZ", 1) - (nthOccurrence(line, "VY", 1) + 5));
			string zData = line.substr(nthOccurrence(line, "VZ", 1) + 4, line.length());

			return { xData,yData,zData };
		}
		static string getLT(string line) {
			return line.substr(nthOccurrence(line, "LT", 1) + 4, nthOccurrence(line, "RG", 1) - (nthOccurrence(line, "LT", 1) + 5));
		}
		static string getRG(string line) {
			return line.substr(nthOccurrence(line, "RG", 1) + 4, nthOccurrence(line, "RR", 1) - (nthOccurrence(line, "RG", 1) + 5));
		}
		static string getRR(string line) {
			return line.substr(nthOccurrence(line, "RR", 1) + 4, line.length());
		}

		static int nthOccurrence(const std::string& str, const std::string& findMe, int nth){
			size_t  pos = 0;
			int     cnt = 0;

			while (cnt != nth){
				pos += 1;
				pos = str.find(findMe, pos);
				if (pos == std::string::npos)
					return -1;
				cnt++;
			}
			return pos;
		}

		static string removeOneLine(string & text) {
			string line = text.substr(0, text.find("\n"));
			text = text.substr(text.find("\n")+1, text.length());
			return line;
		}
};