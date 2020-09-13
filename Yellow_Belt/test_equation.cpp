#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
	os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
	os << "{";
	bool first = true;
	for (const auto& x : s) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
	os << "{";
	bool first = true;
	for (const auto& kv : m) {
		if (!first) {
			os << ", ";
		}
		first = false;
		os << kv.first << ": " << kv.second;
	}
	return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
	if (t != u) {
		ostringstream os;
		os << "Assertion failed: " << t << " != " << u;
		if (!hint.empty()) {
			os << " hint: " << hint;
		}
		throw runtime_error(os.str());
	}
}

void Assert(bool b, const string& hint) {
	AssertEqual(b, true, hint);
}

class TestRunner {
public:
	template <class TestFunc>
	void RunTest(TestFunc func, const string& test_name) {
		try {
			func();
			cerr << test_name << " OK" << endl;
		} catch (exception& e) {
			++fail_count;
			cerr << test_name << " fail: " << e.what() << endl;
		} catch (...) {
			++fail_count;
			cerr << "Unknown exception caught" << endl;
		}
	}

	~TestRunner() {
		if (fail_count > 0) {
			cerr << fail_count << " unit tests failed. Terminate" << endl;
			exit(1);
		}
	}

private:
	int fail_count = 0;
};

int SolveLinear(double b, double c) {
	if (b != 0) {
		return 1 ;
	} else {
		return 0;
	}
}

int SolveQuadratic(double a, double b, double c) {
	double discriminant = b * b - 4 * a * c;

	if (discriminant > 0) {
		return 2;
	} else if (discriminant < 0){
		return 0;
	} else {
		return 1;
	}
}

int GetDistinctRealRootCount(double a, double b, double c) {
	if (a == 0) {
		return SolveLinear(b, c);
	} else {
		return SolveQuadratic(a, b, c);
	}
}

void TestEquation() {
	AssertEqual(GetDistinctRealRootCount(3, -4, 2), 0);
	AssertEqual(GetDistinctRealRootCount(0, -4, 2), 1);
	AssertEqual(GetDistinctRealRootCount(1, -6, 9), 1);
	AssertEqual(GetDistinctRealRootCount(1, -4, -5), 2);
	AssertEqual(GetDistinctRealRootCount(1, 0, -5), 2);
	AssertEqual(GetDistinctRealRootCount(1, 0, 0), 1);
	AssertEqual(GetDistinctRealRootCount(0, 0, 1), 0);
	AssertEqual(GetDistinctRealRootCount(1, 1, 0), 2);
}

int main() {
	TestRunner runner;
	runner.RunTest(TestEquation, "first");
	return 0;
}