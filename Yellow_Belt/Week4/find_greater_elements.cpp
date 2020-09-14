#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

template <typename T>
vector<T> FindGreaterElements(const set<T>& elements, const T& border) {
	vector<T> ans;
	for (auto it = elements.begin(); it != elements.end(); it++) {
		if (*it > border) {
			ans.push_back(*it);
		}
	}
	std::sort(ans.begin(), ans.end());
	return ans;
}

int main() {
	for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) {
		cout << x << " ";
	}
	cout << endl;

	string to_find = "Python";
	cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
	return 0;
}
