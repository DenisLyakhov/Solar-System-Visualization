class DateTime {
public:
	int year;
	int month;
	int day;

	DateTime() {
		this->year = 2006;
		this->month = 1;
		this->day = 1;
	}

	void increaseDay() {
		day++;
		if (day > 31) {
			day = 1;
			month++;
			if (month > 12) {
				month = 1;
				year++;
			}
		}
			
		if (checkLeapDay()) {
			day = 1;
			month++;
		} else if (month == 4 || month == 6 || month == 9 || month == 11) {
			if (day > 30) {
				day = 1;
				month++;
			}
		}
	}

	void decreaseDay() {
		day--;
		if (day < 1) {
			day = 31;
			decreaseMonth();
		}
	}

	void increaseMonth() {
		month++;
		if (month > 12) {
			month = 1;
			year++;
		}
		while(!isValidDate()) {
			decreaseDay();
		}
	}

	void decreaseMonth() {
		month--;
		if (month < 1) {
			month = 12;
			day = 31;
			year--;
		}

		while(!isValidDate()) {
			decreaseDay();
		}
	}

	void increaseYear() {
		year++;
		while(!isValidDate()) {
			decreaseDay();
		}
	}

	void decreaseYear() {
		year--;
		while(!isValidDate()) {
			decreaseDay();
		}
	}

	std::string toString() {
		return std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
	}

private:
	bool isValidDate() {
		if (checkLeapDay()) {
			return false;
		}

		if (month == 4 || month == 6 || month == 9 || month == 11) {
			if (day > 30) {
				return false;
			}
		}
		if (day > 31) {
			return false;
		}
		return true;
	}

	bool checkLeapDay() {
		if (month == 2) {
			if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
				if (day > 29) {
					return true;
				}
			} else if (day > 28) {
				return true;
			}
		}
		return false;
	}
};
