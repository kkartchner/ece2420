#include "CryptoAES.hpp"
#include <iostream>
#include <mcrypt.h>
#include <cstring>

CryptoAES::CryptoAES(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
	    std::function<void(const uint8_t *data, uint32_t len)> decryptCallback) 
 : Crypto(encryptCallback, decryptCallback) {
	 // m
	 
 }

void CryptoAES::genKeys()
{
	std::cout << "Generating CryptoAES keys..." << std::endl;

	// Generate array of 16 random bytes:
	srand(time(0));
	
	m_key = new uint8_t[BLOCK_SIZE];
	for (int i = 0; i < BLOCK_SIZE; i++){
		m_key[i] = rand();
		std::cout << "m_key[" << i << "]: " << (int) m_key[i] << std::endl;
	}
		
	
	std::cout << "Keys have been generated" << std::endl;
}

bool CryptoAES::getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen)
{
	std::cout << "Getting CryptoAES keys..." << std::endl;
	
	std::memcpy((*priKey) + 0, m_key, BLOCK_SIZE);
	std::memcpy((*priKey) + BLOCK_SIZE, m_IV, BLOCK_SIZE);
	priLen = BLOCK_SIZE;
	
	*pubKey = nullptr;
	pubLen = 0;
	
	//	mcrypt_generic_init(td, key, key_len, IV);	
	
	
	std::cout << "Keys have been received" << std::endl;
	return true;
}

void CryptoAES::setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen)
{	
	std::cout << "Setting CryptoAES to specified keys..." << std::endl;
	if (priLen != BLOCK_SIZE){
		std::cout << "Key is not the correct length. Must be 16 bytes.";
		return;
	}
	delete[](m_key); delete[](m_IV);
	
	m_key = new uint8_t[BLOCK_SIZE];
	m_IV = new uint8_t[BLOCK_SIZE];
		
	std::memcpy(m_key, priKey, BLOCK_SIZE);
	std::memcpy(m_IV, (priKey + BLOCK_SIZE), BLOCK_SIZE);
	
	// mcrypt_generic_init(td, key, key_len, IV);
	
	std::cout << "Keys have been set" << std::endl;
}

void CryptoAES::destroyKeys()
{
	std::cout << "Destroying CryptoAES keys..." << std::endl;
	
	delete[](m_key);	
	m_key = nullptr;
	
	std::cout << "Keys have been destroyed" << std::endl;
}

bool CryptoAES::encrypt(const uint8_t *data, uint32_t len)
{
	std::cout << "Encrypting data using CryptoAES..." << std::endl;
	
	uint8_t encrypted_data[len];
	//for (int i = 0; i < len; i++){
	//	encrypted_data[i] =
	//}
	
	m_encryptCallback(encrypted_data, len);
	
//	mcrypt_generic_init(td, key, key_len, IV);
	
	std::cout << "Data has been encrypted" << std::endl;
	return true;
}

bool CryptoAES::decrypt(const uint8_t *data, uint32_t len)
{
	std::cout << "Decrypting data encrypted by CryptoAES..." << std::endl;
	
	uint8_t decrypted_data[len];
	
	m_decryptCallback(decrypted_data, len);
	
	std::cout << "Data has been decrypted" << std::endl;
	return true;
} 
