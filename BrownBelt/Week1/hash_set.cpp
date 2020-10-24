#include "test_runner.h"

#include <forward_list>
#include <iterator>
#include <algorithm>

using namespace std;

template <typename Type, typename Hasher>
class HashSet {
public:
	using BucketList = forward_list<Type>;

public:
	explicit HashSet(size_t num_buckets, const Hasher& hasher_ = {})
		: buckets(num_buckets), hasher(hasher_), data(num_buckets) {}

	void Add(const Type& value) {
		if(!Has(value)) {
			data[hasher(value) % buckets].push_front(value);
		}
	}
	bool Has(const Type& value) const {
		BucketList bucket = data[hasher(value) % buckets];
		auto it = find(bucket.begin(), bucket.end(), value);
		return it != bucket.end();
	}
	void Erase(const Type& value) {
		data[hasher(value) % buckets].remove(value);
	}
	const BucketList& GetBucket(const Type& value) const {
		return data[hasher(value) % buckets];
	}

private:
	Hasher hasher;
	size_t buckets;
	vector<BucketList> data;
};

struct IntHasher {
	size_t operator()(int value) const {
		// Это реальная хеш-функция из libc++, libstdc++.
		// Чтобы она работала хорошо, std::unordered_map
		// использует простые числа для числа бакетов
		return value;
	}
};

struct TestValue {
	int value;

	bool operator==(TestValue other) const {
		return value / 2 == other.value / 2;
	}
};

struct TestValueHasher {
	size_t operator()(TestValue value) const {
		return value.value / 2;
	}
};

void TestSmoke() {
	HashSet<int, IntHasher> hash_set(2);
	hash_set.Add(3);
	hash_set.Add(4);

	ASSERT(hash_set.Has(3));
	ASSERT(hash_set.Has(4));
	ASSERT(!hash_set.Has(5));

	hash_set.Erase(3);

	ASSERT(!hash_set.Has(3));
	ASSERT(hash_set.Has(4));
	ASSERT(!hash_set.Has(5));

	hash_set.Add(3);
	hash_set.Add(5);

	ASSERT(hash_set.Has(3));
	ASSERT(hash_set.Has(4));
	ASSERT(hash_set.Has(5));
}

void TestEmpty() {
	HashSet<int, IntHasher> hash_set(10);
	for (int value = 0; value < 10000; ++value) {
		ASSERT(!hash_set.Has(value));
	}
}

void TestIdempotency() {
	HashSet<int, IntHasher> hash_set(10);
	hash_set.Add(5);
	ASSERT(hash_set.Has(5));
	hash_set.Add(5);
	ASSERT(hash_set.Has(5));
	hash_set.Erase(5);
	ASSERT(!hash_set.Has(5));
	hash_set.Erase(5);
	ASSERT(!hash_set.Has(5));
}

void TestEquivalence() {
	HashSet<TestValue, TestValueHasher> hash_set(10);
	hash_set.Add(TestValue{2});
	hash_set.Add(TestValue{3});

	ASSERT(hash_set.Has(TestValue{2}));
	ASSERT(hash_set.Has(TestValue{3}));

	const auto& bucket = hash_set.GetBucket(TestValue{2});
	const auto& three_bucket = hash_set.GetBucket(TestValue{3});
	ASSERT_EQUAL(&bucket, &three_bucket);

	ASSERT_EQUAL(1, distance(begin(bucket), end(bucket)));
	ASSERT_EQUAL(2, bucket.front().value);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestSmoke);
	RUN_TEST(tr, TestEmpty);
	RUN_TEST(tr, TestIdempotency);
	RUN_TEST(tr, TestEquivalence);

	map<string,int> words;
	for (auto it = words.begin(); it != words.end(); ) {
		if ((it->second)) {
			it = words.erase(it);
		} else {
			++it;
		}
	}

	return 0;
}