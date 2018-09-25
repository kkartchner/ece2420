#include "CryptoAES.hpp"
#include <iostream>
#include <cstring>
#include <sstream>

#include <vector>

int main(int argc, char** argv) {	
	static const char msg[] = "Hello there dude!!!!";
	
	std::vector<uint8_t> buffCipherText;
	std::vector<uint8_t> buffPlainText (msg, msg + sizeof(msg) / sizeof(msg[0]));
	
	auto encryptCallback =
		[&buffCipherText](const uint8_t *data, uint32_t len)
		{
			fwrite (data, 1, len, stdout);
			buffCipherText.insert(buffCipherText.end(), data, data + len);
		};

	auto decryptCallback = 
		[](const uint8_t *data, uint32_t len)
		{
			fwrite (data, 1, len, stdout);
		};
	
	uint8_t plainLen = buffPlainText.size();
	std::cout << "plainLen: " << (int) plainLen << std::endl;
	
 	std::shared_ptr<Crypto> testing = Crypto::cryptoFactory(encryptCallback, decryptCallback, Algorithm::eAES);
	testing->genKeys();
	testing->encrypt(buffPlainText.data(), buffPlainText.size());
	testing->encrypt(nullptr, 0);
	
	uint8_t	cipherLen = buffCipherText.size();
	std::cout << "cipherLen: " << (int) plainLen << std::endl;
	
	testing->decrypt(buffCipherText.data(), buffCipherText.size());
}