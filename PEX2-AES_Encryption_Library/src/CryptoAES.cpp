#include "CryptoAES.hpp"
#include <iostream>
#include <cstring>

#define DEBUG 0
#define DEBUG2 0

CryptoAES::CryptoAES(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
	    std::function<void(const uint8_t *data, uint32_t len)> decryptCallback) 
 : Crypto(encryptCallback, decryptCallback) {	 
	 m_td = mcrypt_module_open((char*)"rijndael-128", nullptr , (char*)"cbc", nullptr);
	 
	 m_buffer.str(""); m_buffer.clear();
	 m_bufferLen = 0;
	 
	 srand(time(0));
 }

CryptoAES::~CryptoAES(){
	mcrypt_generic_deinit(m_td);
	mcrypt_module_close(m_td);
}

void CryptoAES::genKeys()
{
	if(DEBUG) { std::cout << "Generating CryptoAES keys..." << std::endl; }

	// Generate array of 16 random bytes:
	m_key = new uint8_t[BLOCK_SIZE];
	m_IV = new uint8_t[BLOCK_SIZE];
	
	for (int i = 0; i < BLOCK_SIZE; i++){
		m_key[i] = rand();
		m_IV[i] = rand();
	}
	
	mcrypt_generic_init(m_td, m_key, BLOCK_SIZE, m_IV);		
		
	
	if(DEBUG) { std::cout << "Keys have been generated" << std::endl; }
}

bool CryptoAES::getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen)
{	
	if(DEBUG) { std::cout << "Getting CryptoAES keys..." << std::endl; }
	
	if (m_key == nullptr){
		if(DEBUG) { std::cout << "Keys have not been set. Have genKeys() or setKeys() been called?" << std::endl; }
		return false;
	} else {
		*priKey = new uint8_t[BLOCK_SIZE*2];
		
		std::memcpy((*priKey) + 0, 		 m_key, BLOCK_SIZE);	
		std::memcpy((*priKey) + BLOCK_SIZE, m_IV, BLOCK_SIZE);
	
		priLen = BLOCK_SIZE * 2;
		if(DEBUG) { std::cout << "priLen = " << (int) priLen << std::endl; }
	
		*pubKey = nullptr;
		pubLen = 0;	
	
		if(DEBUG) { std::cout << "Keys have been received" << std::endl; }
		return true;
	}
}

void CryptoAES::setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen)
{	
	if(DEBUG) { 
		std::cout << "Setting CryptoAES to specified keys..." << std::endl; 
		std::cout << "priLen = " << (int) priLen << std::endl;
	}
	
	if (!(priLen == BLOCK_SIZE || priLen == BLOCK_SIZE * 2)){
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
	
	mcrypt_generic_init(m_td, m_key, BLOCK_SIZE, m_IV);		
	
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
	if(DEBUG){ std::cout << "Encrypting data using CryptoAES..." << std::endl; }
	
	if (m_key == nullptr){
		if(DEBUG) { std::cout << "Keys have not been set. Have genKeys() or setKeys() been called?" << std::endl; }
		return false;
		
	} else if (len == 0){
		if(DEBUG2){ std::cout << "Encrypt flush initiated.." << std::endl; }
		
		mcrypt_generic_init(m_td, m_key, BLOCK_SIZE, m_IV);
		m_buffer.str(""); m_buffer.clear();
		m_bufferLen = 0;
		
		if(DEBUG2){ std::cout << "Buffer flushed and mcrypt reinitialized" << std::endl; }
		return true;
		
	} else {
		m_buffer.str(""); m_buffer.clear();
		m_buffer.write((char*)data, len);  						// Add all data to the buffer
		m_bufferLen = len;										
		
		uint8_t *data_block = new uint8_t[BLOCK_SIZE];
		
		while (m_bufferLen >= BLOCK_SIZE){						// Encrypt data one (full) block (16 bytes) at a time
			m_buffer.read((char*) data_block, BLOCK_SIZE);		// Read data into data_block
			m_bufferLen -= BLOCK_SIZE;							// "Remove" block of data
			
			mcrypt_generic(m_td, data_block, BLOCK_SIZE);  		// Encrypt current block
			
			m_encryptCallback(data_block, BLOCK_SIZE);			// Call encryptCallback() on each block		 		
		}
		
		if (DEBUG2) { std::cout << "m_bufferLen: " << m_bufferLen << std::endl; }
		
		if (m_bufferLen != 0){									// If there is still info in the buffer
			uint8_t padNum = (BLOCK_SIZE - m_bufferLen);		// Pad the last block with pad num, padnum times, then encrypt
			
			if(DEBUG2) {  std::cout << "padNum: " << (int) padNum << std::endl; }
			
			m_buffer.read((char*) data_block, m_bufferLen);
			
			for (int i = m_bufferLen-1; i < BLOCK_SIZE; i++){
			//	if(DEBUG2) { std::cout << "data_block["<< i << "]:" << (int) data_block[i] << std::endl; }
				
				data_block[i] = padNum;  
				
			//	if(DEBUG2) { std::cout << "data_block["<< i << "]:" << "changed to: " << (int) data_block[i] << std::endl; }
				
			}
			m_bufferLen = 0;
			
		
			if(DEBUG2){
				std::cout << "data_block: ";
				for (int i = 0; i < BLOCK_SIZE; i++){
					 std::cout << (int) data_block[i] << " "; 
				}
				std::cout << std::endl;
			}	
			
			mcrypt_generic(m_td, data_block, BLOCK_SIZE);  	// Encrypt current block
			
			m_encryptCallback(data_block, BLOCK_SIZE);
			
			if(DEBUG2){
				std::cout << "data_block after encrypt:";
				for (int i = 0; i < BLOCK_SIZE; i++){
					 std::cout << (int) data_block[i] << " "; 
				}
				std::cout << std::endl;
			}	
		}
		
		if(DEBUG) std::cout << "Data has been encrypted" << std::endl;
		return true;
	}
}

bool CryptoAES::decrypt(const uint8_t *data, uint32_t len)
{
	if(DEBUG2){ std::cout << "Decrypting data encrypted by CryptoAES..." << std::endl; }
	
	if (m_key == nullptr){
		if(DEBUG){ std::cout << "Keys have not been set. Have genKeys() or setKeys() been called?" << std::endl; }
		return false;
		
	} else if (len == 0){
		if(DEBUG2){ std::cout << "Decrypt flush initiated.." << std::endl; }
		
		mcrypt_generic_init(m_td, m_key, BLOCK_SIZE, m_IV);
		m_buffer.str(""); m_buffer.clear();			// Reset buffer
		m_bufferLen = 0;
		
		if(DEBUG2){ std::cout << "Buffer flushed and mcrypt reinitialized" << std::endl; }
		return true;
		
	} else { 
		m_buffer.str(""); m_buffer.clear();
		m_buffer.write((char*)data, len);  						// Add all data to the buffer
		m_bufferLen = len;										
		
		uint8_t *data_block = new uint8_t[BLOCK_SIZE];
		uint8_t padNum, firstPad;
		
		if (DEBUG2){
			std::cout << "m_bufferLen: " << (int) m_bufferLen << std::endl; 
			char* buffer = new char[len];
			m_buffer.read(buffer, len);
			
			std::cout << "m_buffer: "; 
				for (int i = 0; i < len; i++){
					 std::cout << (int) buffer[i] << " "; 
				}
				std::cout << std::endl;
	
		}
		
		/* Decrypt all full blocks */
		while (m_bufferLen >= BLOCK_SIZE){							// Decrypt data one (full) block (16 bytes) at a time
			m_buffer.read((char*) data_block, BLOCK_SIZE);			// Read data into data_block
			m_bufferLen -= BLOCK_SIZE;								// "Remove" block of data
			
			if(DEBUG2){
				std::cout << "data_block: ";
				for (int i = 0; i < BLOCK_SIZE; i++){
					 std::cout << (int) data_block[i] << " "; 
				}
				std::cout << std::endl;
			}	
			
			mdecrypt_generic(m_td, data_block, BLOCK_SIZE);  		// Decrypt current block
		
			if(DEBUG2){
				std::cout << "data_block after decrypt:";
				for (int i = 0; i < BLOCK_SIZE; i++){
					 std::cout << (char) data_block[i] << " "; 
				}
				std::cout << std::endl;
			}		
			
			padNum = data_block[BLOCK_SIZE-1];						// Remove padding
			firstPad = BLOCK_SIZE - padNum;
			
			bool inRange = (firstPad < BLOCK_SIZE && firstPad >= 0);
			
			if (inRange && data_block[firstPad] == padNum){					// If first padding exists
				m_decryptCallback(data_block, firstPad);	
			} else {			
				m_decryptCallback(data_block, BLOCK_SIZE);
			} 														// Call encryptCallback() on each block		 		
		}
		
		
		if (DEBUG2) {
																/* Encrypt remaining partial block */
		if (m_bufferLen != 0){										// If there is still info in the buffer
			std::cout << "m_bufferLen not 0 after decrypt?? It's:" << m_bufferLen << std::endl;
		}
					}
		if(DEBUG) {std::cout << "Data has been decrypted" << std::endl;}
		return true;
	} 
}
