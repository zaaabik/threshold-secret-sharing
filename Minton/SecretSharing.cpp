#include <boost/multiprecision/miller_rabin.hpp>
#include <random>
#include <chrono>
#include "SecretSharing.h"
#include "BinaryReader.h"
bool primeTest(const cpp_int& number, const int tries);
cpp_int modPow(const cpp_int& a,const cpp_int& n, const cpp_int& m);
bool test(vector<cpp_int> v, int n, int m, cpp_int p) {
	cpp_int s1 = 1;
	for (int i = 0; i < m; ++i) {
		s1 *= v[i];
	}
	
	cpp_int s2 = 1;	
	for (int i = n - m + 1; i < n; ++i) {		
		s2 *= v[i];
	}	
	s2 *= p;
	return s1 > s2;
}
vector<Shadow> SecretSharing::Share(const cpp_int& secret, int m, int n) {
	auto p = getPrimeGreater(secret << 5, 1)[0];
	auto d = getPrimeGreater(p << 25, n);
	cpp_int multiple = 1;

	while (!test(d, n, m, p)) {
		auto p = getPrimeGreater(secret << 10, 1)[0];
		auto d = getPrimeGreater(p << 15, n);
	}
	for (int i = n - m + 1; i < n; ++i) {
		multiple *= d[i];
	}
	auto result = vector<Shadow>();
	cpp_int M = secret;

	cpp_int diff = multiple - M;	
	cpp_int r = diff / p;	
	srand(time(NULL));
	auto random = rand();
	r += random;
	M += r * p;

	multiple *= d[1];
	for (int i = 0; i < n; ++i) {	
		result.push_back(Shadow{getModulo(M, d[i]), d[i], p});
	}	
	return result;
}


cpp_int SecretSharing::GetSecret(const cpp_int& secret, const vector<Shadow>& keys) {
	return cpp_int();
}

GcdexResult SecretSharing::gcdex(const cpp_int& a, const cpp_int& b) {
	auto result = GcdexResult();
	if (a == 0) {
		result.x = 0;
		result.y = 1;
		result.d = b;
		return result;
	}
	GcdexResult r = gcdex(b % a, a);
	result.x = r.y - (b / a) * r.x;
	result.y = r.x;
	result.d = r.d;
	return result;
}

cpp_int SecretSharing::chineseRemainderTheoremSolover(const vector<Shadow>& congruences) {
	cpp_int M = 1;
	auto p = congruences[0].p;
	for (auto congruence : congruences) {
		M *= congruence.modulo;
	}

	auto mi = vector<cpp_int>(congruences.size());
	for (int i = 0; i < congruences.size(); ++i) {
		mi[i] = M / congruences[i].modulo;
	}

	auto mInversion = vector<cpp_int>(congruences.size());
	for (int i = 0; i < congruences.size(); ++i) {
		mInversion[i] = gcdex(mi[i], congruences[i].modulo).x;
	}

	cpp_int res = 0;
	for (int i = 0; i < congruences.size(); ++i) {
		res += mInversion[i] * mi[i] * congruences[i].result;
	}
	res = getModulo(res, M);
	return getModulo(res, p);
}

cpp_int SecretSharing::getModulo(const cpp_int& a, const cpp_int& m) {
	cpp_int result = a % m;
	if (result < 0) {		
		return result + m;
	}
	return result;
}

vector<cpp_int> SecretSharing::getPrimeGreater(const cpp_int& min, int count) {	
	const cpp_int tries = boost::multiprecision::pow(cpp_int(2), 100);
	auto result = vector<cpp_int>();
	int currentCount = 0;
	cpp_int m = min;
	int s = 0;
	auto rnd = cpp_int(min);
	for (cpp_int i = 1; i < tries, currentCount < count; ++i) {
		boost::random::mt19937 rng(time(NULL));
		boost::random::uniform_int_distribution<cpp_int> dist(rnd, rnd + 100);
		rnd += dist(rng);
		cpp_int number = i + m + dist(rng);
		bool isPrime = primeTest(number, 5);
		if (isPrime) {
			result.push_back(number);
			currentCount++;
			m = m << 2;
			i = 1;
		}
	}	
	if(currentCount < count)
		throw exception("can't find prime");
	return result;
}

bool primeTest(const cpp_int& number, const int tries) {
	if (number == 2 || number == 3) return true;
	if (number <= 1 || !(number & 1)) return false;

	boost::random::mt19937 rng;	
	int s = 0;
	boost::random::uniform_int_distribution<cpp_int> dist(2, number - 2);
	for (cpp_int m = number - 1; !(m & 1); ++s, m >>= 1);
	cpp_int d = (number - 1) / (1 << s);
	cpp_int two = 2;

	for (int i = 0; i < tries; ++i) {
		auto a = dist(rng);
		auto x = modPow(a, d, number);

		if (x == 1 || x == number - 1)
			continue;

		for (int r = 1; r <= s - 1; ++r) {
			x = modPow(x, two, number);
			if (x == 1) return false;
			if (x == number - 1) continue;
		}
	}
	return true;
}

cpp_int modPow(const cpp_int& a, const cpp_int& n, const cpp_int& m) {
	cpp_int res = 0;
	cpp_int deg = n;
	cpp_int tmp_a = a;
	while (deg) {
		if (tmp_a % 2) { 
			res = (res * tmp_a) % m; 
		}

		tmp_a = (tmp_a * tmp_a) % m;
		deg /= 2;
	}
	return res;
}