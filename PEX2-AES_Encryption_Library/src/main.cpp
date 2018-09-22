#include <Crypto.hpp>
#include <iostream>

int main()
{
	std::shared_ptr<Crypto> testing;
	
	auto encryptCallback =
		[](const uint8_t *data, uint32_t len)
		{
			
		};

	auto decryptCallback = 
		[](const uint8_t *data, uint32_t len)
		{
			
		};

	testing = Crypto::cryptoFactory(encryptCallback, decryptCallback, Algorithm::eAES);
	
	uint8_t *key = new uint8_t[16];
	for (int i = 0; i < 16; i++){
		key[i] = rand();
	}
	
	
	testing->setKeys(nullptr, 0, key, 16);
	testing->destroyKeys();
	
	return 0;
}

