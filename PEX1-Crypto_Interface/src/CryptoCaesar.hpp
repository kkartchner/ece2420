#ifndef CRYPTOCAESAR_HPP
#define CRYPTOCAESAR_HPP

#include "Crypto.hpp"

/**
 * @class CryptoCaesar
 *
 */
class CryptoCaesar : public Crypto
{
public:
  /**
  * @brief Constructor
  */
  CryptoCaesar(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
	    std::function<void(const uint8_t *data, uint32_t len)> decryptCallback);
 
  virtual void genKeys() override;
 
  virtual bool getKeys(uint8_t **pubKey, uint32_t &pubLen,
		       uint8_t **priKey, uint32_t &priLen) override;

  virtual void setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen) override;

  virtual void destroyKeys() override;
  
  virtual bool encrypt(const uint8_t *data,  uint32_t len) override;
  
  virtual bool decrypt(const uint8_t *data,  uint32_t len) override;

private:
  uint8_t *m_pubKey;
  uint32_t m_pubLen;
  uint8_t *m_priKey;
  uint32_t m_priLen;
};

#endif /* CRYPTOCAESAR_HPP */

 
