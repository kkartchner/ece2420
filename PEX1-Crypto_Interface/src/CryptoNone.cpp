#include "CryptoNone.hpp"
#include <iostream>

CryptoNone::CryptoNone(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
	    std::function<void(const uint8_t *data, uint32_t len)> decryptCallback) 
: Crypto(encryptCallback, decryptCallback) 
{
}

void CryptoNone::genKeys()
{

}

bool CryptoNone::getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen)
{
	*pubKey = m_pubKey;
	pubLen = m_pubLen;	
}

void CryptoNone::setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen)
{
	
}

void CryptoNone::destroyKeys()
{

}

bool CryptoNone::encrypt(const uint8_t *data, uint32_t len)
{
	m_encryptCallback(data, len);
	return true;
}

bool CryptoNone::decrypt(const uint8_t *data, uint32_t len)
{
	m_decryptCallback(data, len);
	return true;
}
