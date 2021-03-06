#include <boost/multiprecision/cpp_int.hpp>
#include <fstream>
#include <iostream>
#include <boost/format.hpp>
#include <map>
#include "BinaryReader.h"
#include "SecretSharing.h"
using namespace boost::multiprecision;
using namespace std;
int main(int argc, char* argv[]) {
	const string shareSecretKey = "-s";
	const string getSecretKey = "-g";
	const string inputFileKey = "-f";
	const string outputFileKey = "-o";
	const string shadowsCountKey = "-n";
	const string minShadowsCountKey = "-m";
	const string mode = "mode";	
	const string out = "out";	
	const char argsFirstSymbol = '-';
	int minShadowCount = 3;
	int ShadowCount = 5;
	
	map<std::string, std::string> commandLineArgs{
		{ inputFileKey,"key.txt" },
		{ mode, getSecretKey },	
		{out, "res.txt"},
	};

	vector<string> args;
	for (int i = 1; i < argc; ++i) {
		args.push_back(argv[i]);
	}
	
	vector<string> files;
	for (int i = 0; i < args.size(); ++i) {
		if (args[i] == outputFileKey) {
			commandLineArgs[out] = args[i + 1];			
			++i;			
		}else
		if (args[i] == shadowsCountKey) {
			ShadowCount = stoi(args[i + 1]);
			++i;
		} else
		if (args[i] == minShadowsCountKey) {
			minShadowCount = stoi(args[i + 1]);
			++i;
		} else
		if (args[i] == shareSecretKey) {
			commandLineArgs[mode] = shareSecretKey;
		} else
		if (args[i] == getSecretKey) {
			commandLineArgs[mode] = getSecretKey;
		} else
		if (args[i][0] != argsFirstSymbol) {
			files.push_back(args[i]);
		}
	}

	if (files.size() == 0) {
		cout << "files count cant be 0" << endl;
		return 0;
	}

	if (commandLineArgs[mode] == shareSecretKey) {
		auto data = BinaryReader::read(files[0]);
		auto res = SecretSharing::Share(data, minShadowCount, ShadowCount);
		for (int i = 0; i < res.size(); ++i) {
			string fileName = "key";
			fileName += to_string(i);
			fileName += ".txt";
			BinaryReader::Write(fileName, res[i]);
		}
	}
	
	if (commandLineArgs[mode] == getSecretKey) {
		vector<Shadow> shadows;
		for (auto file : files) {
			shadows.push_back(BinaryReader::ReadShadow(file));		
		}
		auto res = SecretSharing::chineseRemainderTheoremSolover(shadows);		
		BinaryReader::Write(commandLineArgs[out], res);
		
	}

    return 0;
}
