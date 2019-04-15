class Landmark {
public:
	Landmark(string name) {
		m_name = name;
	}
	string name() const {
		return m_name;
	}
	virtual string icon() const = 0;
	virtual string color() const {
		return "yellow";
	}
	virtual ~Landmark() {};
private:
	string m_name;
};

class Restaurant : public Landmark {
public:
	Restaurant(string name, int seatingCapacity):Landmark(name){
		m_seatingCapacity = seatingCapacity;
	}
	~Restaurant() {
		cout << "Destroying the restaurant " + name() + "." << endl;
	}
	string icon() const {
		if (m_seatingCapacity < 40) {
			return "small knife/fork";
		}
		else {
			return "large knife/fork";
		}
	}
private:
	int m_seatingCapacity;
};

class Hotel : public Landmark {
public:
	Hotel(string name) : Landmark(name) {}
	~Hotel() {
		cout << "Destroying the hotel " << name() << "." << endl;
	}
	string icon() const {
		return "bed";
	}
};

class Hospital : public Landmark {
public:
	Hospital(string name) : Landmark(name) {}
	~Hospital() {
		cout << "Destroying the hospital " << name() << "." << endl;
	}
	string icon() const {
		return "H";
	}
	string color() const {
		return "blue";
	}
};

