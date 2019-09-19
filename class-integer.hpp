#include <iostream>

using namespace std;
class Integer {
private:
public:
	int data;
	Integer() {}
	Integer(const int &value) : data(value) {}
	Integer(const Integer &other) : data(other.data) {}
	bool operator==(const Integer &t)
	{
		return data == t.data;
	}
	ostream &operator<<(ostream &out)
	{
		out << this->data << '\n';
	}
};
