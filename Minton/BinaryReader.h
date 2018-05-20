#pragma once
#include <boost/multiprecision/cpp_int.hpp>
#include "SecretSharing.h"
#include <cstdint>
#include <vector>
using namespace boost::multiprecision;
using namespace std;
class BinaryReader {
public:
	static void Write(string path, cpp_int number);
	static void Write(string path, vector<uint8_t> bytes);
	static void Write(string path, Shadow shadow);
	static Shadow ReadShadow(string path);
	static cpp_int read(std::string path);
};