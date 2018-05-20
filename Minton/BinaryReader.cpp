#include <iostream>
#include <fstream>
#include "BinaryReader.h"
void BinaryReader::Write(std::string path, cpp_int number) {
	std::ofstream file(path, std::ios::binary);
	std::vector<uint8_t> bits;
	export_bits(number, std::back_inserter(bits), 8);	
	file.write((char*)&bits[0], bits.size());
	file.close();
}
void BinaryReader::Write(string path, vector<uint8_t> bytes) {
	std::ofstream file(path, std::ios::binary);		
	size_t size = bytes.size();
	file.write((char*)&bytes, size);
	file.close();
}

void BinaryReader::Write(string path, Shadow shadow) {
	std::ofstream file(path, std::ios::binary);
	
	std::vector<uint8_t> moduloBytes;
	export_bits(shadow.modulo, std::back_inserter(moduloBytes), 8);
	std::vector<uint8_t> resultBytes;
	export_bits(shadow.result, std::back_inserter(resultBytes), 8);
	std::vector<uint8_t> pBytes;
	export_bits(shadow.p, std::back_inserter(pBytes), 8);
	

	uint32_t moduloSize = moduloBytes.size();
	uint32_t resultSize = resultBytes.size();
	uint32_t pSize = pBytes.size();
	auto header = ShadowHeader{ resultSize, moduloSize, pSize };

	file.write((char*)&header, sizeof(ShadowHeader));
		
	file.write((char*)&moduloBytes[0], moduloBytes.size());
	file.write((char*)&resultBytes[0], resultBytes.size());
	file.write((char*)&pBytes[0], pBytes.size());
	file.close();
}

Shadow BinaryReader::ReadShadow(string path) {	
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open()) {
		throw exception("can't open the file!");
	}

	auto result = Shadow();
	
	auto header = ShadowHeader();
	file.read((char*)&header, sizeof(ShadowHeader));
	std::vector<uint8_t> numbers(std::istreambuf_iterator<char>{file}, {});
	cpp_int a;
	cpp_int b;
	cpp_int p;
	import_bits(a, numbers.begin(), numbers.begin() + header.moduloSize);
	import_bits(b, numbers.begin() + header.moduloSize, numbers.begin() + header.moduloSize + header.resultSize);	
	import_bits(p, numbers.begin() + header.moduloSize + header.resultSize, numbers.end());
	result.modulo = a;
	result.result = b;
	result.p = p;
	return result;
}

cpp_int BinaryReader::read(std::string path) {
	std::ifstream file(path, std::ios::binary);
	std::vector<uint8_t> res((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	boost::multiprecision::cpp_int number;
	boost::multiprecision::import_bits(number, res.begin(), res.end());
	file.close();
	return number;
}
