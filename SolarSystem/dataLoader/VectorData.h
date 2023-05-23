
using namespace cpr;
class VectorData {
	private:
		//Data from Horizon Api
		string date;
		array<double, 3> coordinates;
		array<double, 3> velocity;
		string LT;
		string RG;
		string RR;

		// Helper functions
		array<double, 3> convertStrCoordinates(array<string, 3> coordinatesStr) {
			array<double, 3> coordinatesNumerical = { 0.0, 0.0, 0.0 };

			double coord;

			for (int i = 0; i < 3; i++) {
				istringstream os(coordinatesStr[i]);
				os >> coord;
				coordinatesNumerical[i] = coord;
			}

			return coordinatesNumerical;
		}
	public:

		VectorData(){}
		//Getters and Settters 
		inline void setDate(string date) { this->date = date; }

		inline void setCoordinates(array<string,3> coordinates) { 
			this->coordinates = convertStrCoordinates(coordinates);
		}

		inline void setVelocity(array<string, 3> velocity) { 
			this->velocity = convertStrCoordinates(velocity); 
		}

		inline void setLT(string LT) { this->LT = LT; }
		inline void setRG(string RG) { this->RG = RG; }
		inline void setRR(string RR) { this->RR = RR; }

		inline string getDate() { return this->date; }
		inline array<double,3> getCoordinates() { return this->coordinates; }
		inline array<double,3> getVelocity() { return this->velocity; }
		inline string getLT(string LT) { this->LT = LT; }
		inline string getRG(string RG) { this->RG = RG; }
		inline string getRR(string RR) { this->RR = RR; }

		string toString() {
			array<string, 3> coordinatesStr = { "", "", "" };
			array<string, 3> velocityStr = { "", "", "" };

			for (int i = 0; i < 3; i++) {
				coordinatesStr[i] = coordinates[i];
				velocityStr[i] = velocity[i];
			}

			return "{ date: " + date +
				" x: " + coordinatesStr[0] + " y: " + coordinatesStr[1] + " z: " + coordinatesStr[2] +
				" VX: " + velocityStr[0] + " VY: " + velocityStr[1] + " VZ: " + velocityStr[2] +
				" LT: " + LT + " RG: " + RG + " RR: " + RR + "}";
		}
};