#pragma once
#define CURL_STATICLIB

#include<iostream>
#include <cpr/cpr.h>
#include <sstream>

using namespace std;
using namespace cpr;
//$$SOE $$EOE
class DataParser {

	public:
		static string parseData(string urlReturn) {
			return cutUpAndDown(urlReturn);
		}
	private:
		static string cutUpAndDown(string text) {
			string delimeter = "$$SOE";
			string delimeter2 = "$$EOE";
			string returnString = text.substr(text.find(delimeter),text.length());
			returnString = returnString.substr(6, returnString.find(delimeter2) -6);
			return returnString;
		}
};