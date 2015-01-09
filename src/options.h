#include <string>
#include <algorithm>
#include "utils.h"

namespace opt {
	char* getOption(char** begin, char** end, const std::string& option) {
		char ** itr = std::find(begin, end, option);
		if (itr != end && ++itr != end) return *itr;
		return 0;
	}

	bool chkOption(char** begin, char** end, const std::string& option) {
		return std::find(begin, end, option) != end;
	}

	int getInt(char** begin, char** end, const std::string& option, int d) {
		int value = d;
		char* str = getOption(begin, end, option);
		if (str) {
			size_t lastChar;
			try {
				value = std::stoi(str, &lastChar, 10);
			} catch(std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}
		return value;
	}

	u64 getU64(char** begin, char** end, const std::string& option, u64 d) {
		u64 value = d;
		char* str = getOption(begin, end, option);
		if (str) {
			try {
				SSCANF((str, "%llu", &value));
			} catch(std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}
		return value;
	}
}
