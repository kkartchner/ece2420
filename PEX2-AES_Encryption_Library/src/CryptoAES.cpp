#include "CryptoAES.hpp"
#include <iostream>
#include <mcrypt.h>
#include <cstring>

#define DEBUG 0

CryptoAES::CryptoAES(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
	    std::function<void(const uint8_t *data, uint32_t len)> decryptCallback) 
 : Crypto(encryptCallback, decryptCallback) {	 
	 m_td = mcrypt_module_open((char*)"rijndael-128", nullptr , (char*)"cbc", nullptr);
	 
	 m_buffer.str("");
	 m_bufferLen = 0;
	 
	 srand(time(0));
 }

void CryptoAES::genKeys()
{
	if(DEBUG) std::cout << "Generating CryptoAES keys..." << std::endl;

	// Generate array of 16 random bytes:
	m_key = new uint8_t[BLOCK_SIZE];
	m_IV = new uint8_t[BLOCK_SIZE];
	
	for (int i = 0; i < BLOCK_SIZE; i++){
		m_key[i] = rand();
		m_IV[i] = rand();
	}
	
	mcrypt_generic_init(m_td, m_key, BLOCK_SIZE, m_IV);		
		
	
	if(DEBUG) std::cout << "Keys have been generated" << std::endl;
}

bool CryptoAES::getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen)
{	
	if(DEBUG) std::cout << "Getting CryptoAES keys..." << std::endl;
	
	if (m_key == nullptr){
		if(DEBUG) std::cout << "Keys have not been set. Have genKeys() or setKeys() been called?" << std::endl;
		return false;
	} else {
		*priKey = new uint8_t[BLOCK_SIZE*2];
		
		std::memcpy((*priKey) + 0, 		 m_key, BLOCK_SIZE);	
		std::memcpy((*priKey) + BLOCK_SIZE, m_IV, BLOCK_SIZE);
	
		priLen = BLOCK_SIZE * 2;
	
		*pubKey = nullptr;
		pubLen = 0;	
	
		if(DEBUG) std::cout << "Keys have been received" << std::endl;
		return true;
	}
}

void CryptoAES::setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen)
{	
	if(DEBUG) std::cout << "Setting CryptoAES to specified keys..." << std::endl;
	
	if (priLen != BLOCK_SIZE || priLen != BLOCK_SIZE * 2){
		if(DEBUG) { std::cout << "Incorrect key length. Must be either 16 bytes if just the key" 
				  << " or 32 bytes if it includes the instantiation vector (IV) after the key." 
				  << std::endl; }
		return;
	}
	
	delete[](m_key); delete[](m_IV);
	
	m_key = new uint8_t[BLOCK_SIZE];
	m_IV = new uint8_t[BLOCK_SIZE];
	
	std::memcpy(m_key, priKey, BLOCK_SIZE);	
	if (priLen == BLOCK_SIZE){						// If key passed in only has the key 
		for (int i = 0; i < BLOCK_SIZE; i++){		// 		Generate random IV
			m_IV[i] = rand();
		}	
	} else {										// Else 
		std::memcpy(m_IV, (priKey + BLOCK_SIZE), BLOCK_SIZE);	// Set key to the second 16 bytes of the key passed in
	}
	
	if(DEBUG) std::cout << "Keys have been set" << std::endl;
}

void CryptoAES::destroyKeys()
{
	if(DEBUG) std::cout << "Destroying CryptoAES keys..." << std::endl;
	
	delete[](m_key); delete[](m_IV);
	
	m_IV = m_key = nullptr;
	
	if(DEBUG) std::cout << "Keys have been destroyed" << std::endl;
}

bool CryptoAES::encrypt(const uint8_t *data, uint32_t len)
{
	if(DEBUG) std::cout << "Encrypting data using CryptoAES..." << std::endl;
	
	if (m_key == nullptr){
		if(DEBUG) std::cout << "Keys have not been set. Have genKeys() or setKeys() been called?" << std::endl;
		return false;
		
	} else {
		m_buffer.write((char*)data, len);  // Add all data to the buffer
		m_bufferLen += len;
		
		mcrypt_generic_init(m_td, m_key, KEY_SIZE, m_IV);
		
		while (m_bufferLen >= BLOCK_SIZE){						// Encrypt data one block (16 bytes) at a time
			uint8_t *data_block = new uint8_t[BLOCK_SIZE];
			m_buffer.read((char*) data_block, BLOCK_SIZE);
			m_bufferLen -= BLOCK_SIZE;		
			
  	  		mdecrypt_generic(m_td, data_block, BLOCK_SIZE);  	// Encrypt current block
			
			m_encryptCallback(data_block, len);					// Encrypt callback on each block
		}
		
		mcrypt_generic_deinit (m_td);
		
		if(DEBUG) std::cout << "Data has been encrypted" << std::endl;
		return true;
	}
}

bool CryptoAES::decrypt(const uint8_t *data, uint32_t len)
{
	if(DEBUG) std::cout << "Decrypting data encrypted by CryptoAES..." << std::endl;
	if (m_key == nullptr){
		if(DEBUG) std::cout << "Keys have not been set. Have genKeys() or setKeys() been called?" << std::endl;
		return false;
		
	} else {
		uint8_t decrypted_data[len];
	
		m_decryptCallback(decrypted_data, len);
	
		if(DEBUG) std::cout << "Data has been decrypted" << std::endl;
		return true;
	}
} 
