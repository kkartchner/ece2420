#include "CryptoNone.hpp"
#include "CryptoShift.hpp"
#include <iostream>
#include <string>
#include <sstream>

void ctCallback(const uint8_t *data, uint32_t len)
{
  std::cout.write((char *)data, len);
}

int main()
{
  std::ostringstream ctOss;
  std::ostringstream ptOss;

  //trivial callbacks
  std::function<void(const uint8_t *data, uint32_t len)> ctFunc = [&ctOss](const uint8_t *buf, uint32_t len){
    ctOss.write((char *)buf, len);
  };
  std::function<void(const uint8_t *data, uint32_t len)> ptFunc = [&ptOss](const uint8_t *buf, uint32_t len){
    ptOss.write((char *)buf, len);
  };

  //None test
  ctOss.str("");
  ptOss.str("");
  //auto pCrypto = Crypto::cryptoFactory(ctFunc, ptFunc, Algorithm::eNONE);
  auto pCrypto = Crypto::cryptoFactory(ctCallback, ptFunc, Algorithm::eNONE);
  pCrypto->genKeys();
  std::string test = "Testing NONE transform\n";
  pCrypto->encrypt((uint8_t *)test.c_str(), test.size()); 
  pCrypto->encrypt(nullptr, 0); 
  std::cout << "CT: " << ctOss.str() << std::endl;  
  pCrypto->decrypt((uint8_t *)(ctOss.str().c_str()), ctOss.str().size()); 
  std::cout << "PT: " << ptOss.str() << std::endl;  

  //Ceasar test
  ctOss.str("");
  ptOss.str("");
  pCrypto = Crypto::cryptoFactory(ctFunc, ptFunc, Algorithm::eCAESAR);
  pCrypto->genKeys();
  test = "Testing SHIFT transform\n";
  pCrypto->encrypt((uint8_t *)test.c_str(), test.size()); 
  pCrypto->encrypt(nullptr, 0); 
  std::cout << "CT: " << ctOss.str() << std::endl;  
  pCrypto->decrypt((uint8_t *)(ctOss.str().c_str()), ctOss.str().size()); 
  std::cout << "PT: " << ptOss.str() << std::endl;  











#if 0
  std::shared_ptr<Crypto> pCrypto = Crypto::cryptoFactory(ctFunc, ptFunc, Algorithm::eAES);
  pCrypto->genKeys();
  std::string test = "Testing AES transform\n";

  pCrypto->encrypt((uint8_t *)test.c_str(), test.size()); 
  pCrypto->encrypt((uint8_t *)test.c_str(), 0); 
  std::cout << "CT: " << ctOss.str() << std::endl;
  
  pCrypto->decrypt((uint8_t *)(ctOss.str().c_str()), ctOss.str().size()); 
  std::cout << "PT: " << ptOss.str() << std::endl;  
#endif
}
