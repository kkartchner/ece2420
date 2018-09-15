#include "CryptoCaesar.hpp"
#include <iostream>

CryptoCaesar::CryptoCaesar(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
	    std::function<void(const uint8_t *data, uint32_t len)> decryptCallback) 
 : Crypto(encryptCallback, decryptCallback) {
  
 }

void CryptoCaesar::genKeys()
{	
	uint8_t key[] = { 1 };
	m_pubKey = key;
	m_pubLen = (sizeof(key)/sizeof(*key));
	
	std::cout << "m_pubLen is: " << m_pubLen << std::endl;
	
}

bool CryptoCaesar::getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen)
{
	*pubKey = m_pubKey;
	pubLen = m_pubLen;
}

void CryptoCaesar::setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen)
{	
	if (m_pubKey != nullptr){
	    *m_pubKey = *pubKey;
	} else {
        m_pubKey = new uint8_t(*pubKey);
    }
	m_pubLen = pubLen;
	
	if (m_priKey != nullptr){
	    *m_priKey = *priKey;
	} else {
        m_priKey = new uint8_t(*priKey);
    }
	m_priLen = priLen;
}

void CryptoCaesar::destroyKeys()
{
}

bool CryptoCaesar::encrypt(const uint8_t *data, uint32_t len)
{
	uint8_t data_copy[len];
	for (int i = 0; i < len; i++){
		std::cout << "data[i] = " << data[i] << std::endl;
		data_copy[i] = (data[i] - 1) % 256;
		std::cout << "data_copy[i] = " << data_copy[i] << std::endl;
	}
	
	m_encryptCallback(data_copy, len);
	return true;
}

bool CryptoCaesar::decrypt(const uint8_t *data, uint32_t len)
{
	uint8_t data_copy[len];
	for (int i = 0; i < len; i++){
		std::cout << "data[i] = " << data[i] << std::endl;
		data_copy[i] = (data[i] + 1) % 256;
		std::cout << "data_copy[i] = " << data_copy[i] << std::endl;
	}
	m_decryptCallback(data_copy, len);
	return true;
} 
