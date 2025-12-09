#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdint>

#define DECIMAL 10
const std::string BASE64_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							     "abcdefghijklmnopqrstuvwxyz"
							     "0123456789+/";

uint8_t
hexToDecimal(char ch) {
	if(ch >= '0' && ch <= '9') return ch - '0';
	if(ch >= 'a' && ch <= 'f') return ch - 'a' + DECIMAL;
	if(ch >= 'A' && ch <= 'F') return ch - 'A' + DECIMAL;
	throw std::runtime_error("Invalid hex character");
}

std::vector<uint8_t>
hexToBytes(const std::string& hex) {
	assert(not (hex.size() & 1));
	
	std::vector<uint8_t> bytes(hex.size() >> 1);
	for(size_t i = 0; i < hex.size(); i += 2) {
		uint8_t high = hexToDecimal(hex[i]);
		uint8_t low = hexToDecimal(hex[i + 1]);
		bytes[i >> 1] = (high << 4) | low;
	}

	return bytes;
}

std::string
bytesToBase64(std::vector<uint8_t> bytes) {
	std::string encoding = "";
	size_t i = 0;
	while(i + 2 < bytes.size()) {
		uint8_t firstBits = bytes[i] >> 2;
		uint8_t secondBits = ((bytes[i] & 0x03) << 4) | (bytes[i + 1] >> 4);
		uint8_t thirdBits = ((bytes[i + 1]& 0x0f) << 2) | (bytes[i + 2] >> 6);
		uint8_t fourthBits = bytes[i + 2] & 0x3f;
		encoding += BASE64_TABLE[firstBits];
		encoding += BASE64_TABLE[secondBits];
		encoding += BASE64_TABLE[thirdBits];
		encoding += BASE64_TABLE[fourthBits];
		i += 3;
	}
	size_t rem = bytes.size() - i;

	if (rem == 1) {
		uint8_t firstBits  =  bytes[i] >> 2;
		uint8_t secondBits = (bytes[i] & 0x03) << 4;

		encoding += BASE64_TABLE[firstBits];
		encoding += BASE64_TABLE[secondBits];
		encoding += '=';
		encoding += '=';
	}
	else if (rem == 2) {
		uint8_t firstBits  =  bytes[i] >> 2;
		uint8_t secondBits = ((bytes[i] & 0x03) << 4) | (bytes[i + 1] >> 4);
		uint8_t thirdBits  = (bytes[i + 1] & 0x0f) << 2;

		encoding += BASE64_TABLE[firstBits];
		encoding += BASE64_TABLE[secondBits];
		encoding += BASE64_TABLE[thirdBits];
		encoding += '=';
	}
	return encoding;
}



int
main(void) {
	std::string input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	
	auto bytes = hexToBytes(input);
	
	std::string output = bytesToBase64(bytes);
	std::string expectedOutput = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	
	assert(output == expectedOutput);
	std::cout << "Tests passed successfully!\n";
	
	return 0;
}
