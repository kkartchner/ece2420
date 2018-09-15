#include "CryptoNone.hpp"
#include <iostream>

CryptoNone::CryptoNone(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
	    std::function<void(const uint8_t *data, uint32_t len)> decryptCallback) 
: Crypto(encryptCallback, decryptCallback) 
{
}

void CryptoNone::genKeys()
{
	m_pubKey = m_priKey = nullptr;
	m_pubLen = m_priLen = 0;
}

bool CryptoNone::getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen)
{
	*pubKey = m_pubKey;
	pubLen = m_pubLen;	
	
	*priKey = m_priKey;
	priLen = m_priLen;
}

void CryptoNone::setKeys(const uint8_t *pubKey, uint32_t pubLen,
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

void CryptoNone::destroyKeys()
{
	/*
	delete(m_pubKey);
	delete(m_priKey);
	
	m_pubKey = m_priKey = nullptr;	
	m_pubLen = m_priLen = 0;
	*/
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
