#include "CryptoNone.hpp"
#include <iostream>

CryptoNone::CryptoNone(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
	    std::function<void(const uint8_t *data, uint32_t len)> decryptCallback) 
: Crypto(encryptCallback, decryptCallback) 
{
}

void CryptoNone::genKeys()
{
	std::cout << "Generating CryptoNone keys..." << std::endl;
	
	m_pubKey = m_priKey = nullptr;
	m_pubLen = m_priLen = 0;
	
	std::cout << "Keys have been generated" << std::endl;
}

bool CryptoNone::getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen)
{
	std::cout << "Getting CryptoNone keys..." << std::endl;
	
	*pubKey = m_pubKey;
	pubLen = m_pubLen;	
	
	*priKey = m_priKey;
	priLen = m_priLen;
	
	std::cout << "Keys have been received" << std::endl;
}

void CryptoNone::setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen)
{
	std::cout << "Setting CryptoNone to specified keys..." << std::endl;
	
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

void CryptoNone::destroyKeys()
{
	std::cout << "Destroying CryptoNone keys..." << std::endl;
	
	delete(m_pubKey);
	delete(m_priKey);
	
	m_pubKey = m_priKey = nullptr;	
	m_pubLen = m_priLen = 0;
	
	std::cout << "Keys have been destroyed" << std::endl;
}

bool CryptoNone::encrypt(const uint8_t *data, uint32_t len)
{
	std::cout << "\"Encrypting\" data using CryptoNone..." << std::endl;
	
	std::cout << "Data has been \"encrypted\". It is now impossible to read." << std::endl;
	
	m_encryptCallback(data, len);
	
	return true;
}

bool CryptoNone::decrypt(const uint8_t *data, uint32_t len)
{
	std::cout << "\"Decrypting\" data encrypted by CryptoNone..." << std::endl;
	std::cout << "Data has been \"decrypted\". It is now possible to read." << std::endl;
	
	m_decryptCallback(data, len);
	
	return true;
}
