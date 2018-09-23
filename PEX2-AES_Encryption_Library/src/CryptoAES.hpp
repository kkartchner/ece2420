#ifndef CryptoAES_HPP
#define CryptoAES_HPP

#include "Crypto.hpp"
#include <mcrypt.h>
#include <sstream>

/**
 * @class CryptoAES
 *
 */
class CryptoAES : public Crypto
{
public:
  /**
  * @brief Constructor
  */
  CryptoAES(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
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
  MCRYPT m_td;
  uint8_t *m_key; // 16 bytes
  uint8_t *m_IV; // 16 bytes
  static constexpr uint8_t KEY_SIZE = 16,  // 16 bytes
                           IV_SIZE = 16,
                           BLOCK_SIZE = 16; 
  
  uint8_t *m_buffer;
};

#endif /* CryptoAES_HPP */

 
