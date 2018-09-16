#include "CryptoCaesar.hpp"
#include <iostream>

CryptoCaesar::CryptoCaesar(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
	    std::function<void(const uint8_t *data, uint32_t len)> decryptCallback) 
 : Crypto(encryptCallback, decryptCallback) {
  
 }

void CryptoCaesar::genKeys()
{
	std::cout << "Generating CryptoCaesar keys..." << std::endl;
	
	m_pubKey = new uint8_t(1);
	m_pubLen = 1;
	
	m_priKey = nullptr;
	m_priLen = 0;	
	
	std::cout << "Keys have been generated" << std::endl;
}

bool CryptoCaesar::getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen)
{
	std::cout << "Getting CryptoCaesar keys..." << std::endl;
	
	*pubKey = m_pubKey;
	pubLen = m_pubLen;
	
	*priKey = m_priKey;
	priLen = m_priLen;
	
	std::cout << "Keys have been received" << std::endl;
}

void CryptoCaesar::setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen)
{	
	std::cout << "Setting CryptoCaesar to specified keys..." << std::endl;
	
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
	
	std::cout << "Keys have been set" << std::endl;
}

void CryptoCaesar::destroyKeys()
{
	std::cout << "Destroying CryptoCaesar keys..." << std::endl;
	delete(m_pubKey);
	delete(m_priKey);
	
	m_pubKey = m_priKey = nullptr;	
	m_pubLen = m_priLen = 0;
	
	std::cout << "Keys have been destroyed" << std::endl;
}

uint8_t shift_value (uint8_t value, uint8_t shift_amount){
	uint8_t m_val = (value + shift_amount) % 256;
	if (m_val < 0)
		m_val = 256 - m_val;
	return m_val;
}

bool CryptoCaesar::encrypt(const uint8_t *data, uint32_t len)
{
	std::cout << "Encrypting data using CryptoCaesar..." << std::endl;
	
	uint8_t encrypted_data[len];
	for (int i = 0; i < len; i++){
		encrypted_data[i] = shift_value(data[i], -m_pubKey[0]);
	}
	
	m_encryptCallback(encrypted_data, len);
	
	std::cout << "Data has been encrypted" << std::endl;
	return true;
}

bool CryptoCaesar::decrypt(const uint8_t *data, uint32_t len)
{
	std::cout << "Decrypting data encrypted by CryptoCaesar..." << std::endl;
	
	uint8_t decrypted_data[len];
	for (int i = 0; i < len; i++){
		decrypted_data[i] = shift_value(data[i], m_pubKey[0]);
	}
	
	m_decryptCallback(decrypted_data, len);
	
	std::cout << "Data has been decrypted" << std::endl;
	return true;
} 
