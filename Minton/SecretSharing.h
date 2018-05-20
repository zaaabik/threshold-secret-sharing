#pragma once
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost::multiprecision;

struct GcdexResult {
	cpp_int x;
	cpp_int y;
	cpp_int d;
};

struct ShadowHeader {
	uint32_t resultSize;
	uint32_t moduloSize;
	uint32_t pSize;
};

struct Shadow {
	cpp_int result;
	cpp_int modulo;
	cpp_int p;
};

class SecretSharing {
public:
	static vector<Shadow> Share(const cpp_int &secret, const int n, const  int m);
	static cpp_int GetSecret(const cpp_int& secret, const vector<Shadow>& keys);
	static cpp_int chineseRemainderTheoremSolover(const vector<Shadow>& congruences);
	static GcdexResult gcdex(const cpp_int& a, const cpp_int& b);	
	static cpp_int getModulo(const cpp_int& a, const cpp_int& m);
	static vector<cpp_int> getPrimeGreater(const cpp_int& min, int count);
};
