#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdint>

#define DECIMAL 10

uint8_t
hexToDecimal(char ch) {
	if(ch >= '0' && ch <= '9') return ch - '0';
	if(ch >= 'a' && ch <= 'f') return ch - 'a' + DECIMAL;
	if(ch >= 'A' && ch <= 'F') return ch - 'A' + DECIMAL;
	throw std::runtime_error("Invalid hex character");
}

char 
decimalToHex(uint8_t d) {
	if(d < 10) return '0' + d;
	return 'a' + d - DECIMAL;
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
bytesToHex(const std::vector<uint8_t>& bytes) {
	std::string result = "";
	for(size_t i = 0; i < bytes.size(); i++) {
		uint8_t div = bytes[i] >> 4;
		result += decimalToHex(div);
		result += decimalToHex(bytes[i] - (div << 4));
	}
	return result;
}

std::vector<uint8_t>
xorCombination(const std::vector<uint8_t>& bytes1, const std::vector<uint8_t>& bytes2) {
	assert(bytes1.size() == bytes2.size());
	std::vector<uint8_t> result(bytes1.size());
	for(size_t i = 0; i < bytes1.size(); i++)
		result[i] = bytes1[i] ^ bytes2[i];
	return result;
}

int
main(void) {
	std::string str1 = "1c0111001f010100061a024b53535009181c";
	std::string str2 = "686974207468652062756c6c277320657965";
	
	std::vector<uint8_t> bytes1 = hexToBytes(str1);
	std::vector<uint8_t> bytes2 = hexToBytes(str2);
	std::vector<uint8_t> bytesResult = xorCombination(bytes1, bytes2);
	std::string result = bytesToHex(bytesResult);
	
	std::string expectedResult = "746865206b696420646f6e277420706c6179";
	
	assert(expectedResult == result);
	std::cout << "All tests succesfully passed!\n";
}
