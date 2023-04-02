
using namespace cpr;
class VectorData {
	private:
		//Data from Horizon Api
		string date;
		array<string, 3> coordinates;
		array<string, 3> velocity;
		string LT;
		string RG;
		string RR;
	public:

		VectorData(){}
		//Getters and Settters 
		void setDate(string date) { this->date = date; }
		void setCoordinates(array<string,3> coordinates) { this->coordinates = coordinates; }
		void setVelocity(array<string, 3> velocity) { this->velocity = velocity; }
		void setLT(string LT) { this->LT = LT; }
		void setRG(string RG) { this->RG = RG; }
		void setRR(string RR) { this->RR = RR; }

		string getDate() { return this->date; }
		array<string,3> getCoordinates() { return this->coordinates; }
		array<string,3> getVelocity() { return this->velocity; }
		string getLT(string LT) { this->LT = LT; }
		string getRG(string RG) { this->RG = RG; }
		string getRR(string RR) { this->RR = RR; }

		array<double, 3> getCoordinatesNumerical() {
			array<double, 3> coordinatesNumerical = { 0.0, 0.0, 0.0 };
			double coord;

			for (int i = 0; i < 3; i++) {
				istringstream os(this->coordinates[i]);
				os >> coord;
				coordinatesNumerical[i] = coord;
			}

			return coordinatesNumerical;
		}

		string toString() {
			return "{ date: " + date +
				" x: " + coordinates[0] + " y: " + coordinates[1] + " z: " + coordinates[2] +
				" VX: " + velocity[0] + " VY: " + velocity[1] + " VZ: " + velocity[2] +
				" LT: " + LT + " RG: " + RG + " RR: " + RR + "}";
		}
};