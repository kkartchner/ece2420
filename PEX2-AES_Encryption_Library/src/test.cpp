#include "Crypto.hpp"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

// Validates that data == ex_message
bool Verify(const uint8_t *ex_msg, int ex_len, std::vector<uint8_t> data) {
	if (ex_len != data.size()) {
		std::cout
			<< "\tMessage length incorrect:" << std::endl
			<< "\t\tExpected: " << ex_len << std::endl
			<< "\t\tActual: " << data.size() << std::endl;
		return false;
	}

	bool equal = true;
	for (int i = 0; i < ex_len; ++i) {
		if (data[i] != ex_msg[i]) {
			equal = false;
			break;
		}
	}

	if(!equal) {
		std::cout
			<< "\tMessage contents incorrect:" << std::endl
			<< "\t\tExpected: \"";
		std::cout.write((const char*)ex_msg, ex_len);
		std::cout
			<< "\"" << std::endl
			<< "\t\tActual: \"";
		std::cout.write((const char*)data.data(), data.size());
		std::cout
			<< "\"" << std::endl;
		return false;
	}

	return true;
}

// A struct that compresses the passing of 3 counting variables
struct TestResults {
public:
	int total = 0;
	int passed = 0;
	int failed = 0;
};

bool testBlockSize(uint8_t blockSize, const uint8_t *msg, uint32_t len)
{
	std::cout << +blockSize << " byte blocks" << std::endl;

	std::shared_ptr<Crypto> testing;
	std::vector<uint8_t> buffCiphertext;
	std::vector<uint8_t> buffPlaintext;

	auto encryptCallback =
		[&buffCiphertext](const uint8_t *data, uint32_t len)
		{
			buffCiphertext.insert(buffCiphertext.end(), data, data + len);
		};

	auto decryptCallback = 
		[&buffPlaintext](const uint8_t *data, uint32_t len)
		{
			buffPlaintext.insert(buffPlaintext.end(), data, data + len);
		};

	testing = Crypto::cryptoFactory(encryptCallback, decryptCallback, Algorithm::eAES);
	if (testing == nullptr) {
		std::cout << "\tcryptoFactory returned nullptr!" << std::endl;
		return false;
	}

	uint8_t *pubKey;
	uint8_t *priKey;
	uint32_t pubLen;
	uint32_t priLen;
	
	if (testing->getKeys(&pubKey, pubLen, &priKey, priLen)) {
		std::cout << "\tgetKeys returned something before gen/set!" << std::endl;
		return false;
	}

	if (testing->encrypt(nullptr, 0) || testing->decrypt(nullptr, 0)) {
		std::cout << "encrypt or decrypt returned true before gen/set!" << std::endl;
	}

	testing->genKeys();
	if (!testing->getKeys(&pubKey, pubLen, &priKey, priLen)) {
		std::cout << "\tgetKeys returned false!" << std::endl;
		return false;
	}
	
	if (pubLen != 0) {
		std::cout << "\tpublic key was used!" << std::endl;
		return false;
	}
	
	priKey[0] += 15;
	uint8_t *testPri;
	if (!testing->getKeys(&pubKey, pubLen, &testPri, priLen)) {
		std::cout << "\tgetKeys returned false! But only sometimes?" << std::endl;
		return false;
	}

	if (priKey[0] == testPri[0]) {
		std::cout << "\tPrivate member was exposed" << std::endl;
		return false;
	}

	// Test the cryptoFactory
	testing = Crypto::cryptoFactory(encryptCallback, decryptCallback, Algorithm::eAES);
	if (testing == nullptr) {
		std::cout << "\tcryptoFactory returned nullptr! But only sometimes?" << std::endl;
		return false;
	}

	testing->genKeys();
	if (!testing->getKeys(&pubKey, pubLen, &priKey, priLen)) {
		std::cout << "\tgetKeys returned false!" << std::endl;
		return false;
	}

	for (int i = 0; i < len; i += blockSize) {
		int block = (len - i > blockSize) ? blockSize : len - i;
		if (!testing->encrypt(msg + i, block)) {
			std::cout << "\tencrypt(msg + " << i << ", " << block << ") "
				"returned false (len: " << len << ")" << std::endl;
			return false;
		}
	}
	if (!testing->encrypt(nullptr, 0)) {
		std::cout << "\tReturned false on encrypt flush command" << std::endl;
		return false;
	}

	testing->destroyKeys();
	testing = Crypto::cryptoFactory(encryptCallback, decryptCallback, Algorithm::eAES);
	if (testing == nullptr) {
		std::cout << "\tcryptoFactory returned nullptr! But only sometimes?" << std::endl;
		return false;
	}
	testing->setKeys(pubKey, pubLen, priKey, priLen);

	uint8_t *cipher = buffCiphertext.data();
	uint32_t cLen   = buffCiphertext.size();

	for (int i = 0; i < cLen; i += blockSize) {
		int block = (cLen - i > blockSize) ? blockSize : cLen - i;
		if (!testing->decrypt(cipher + i, block)) {
			std::cout << "\tdecrypt(cipher + " << i << ", " << block << ") "
				"returned false (cLen: " << cLen << ")" << std::endl;
			return false;
		}
	}
	if (!testing->decrypt(nullptr, 0)) {
		std::cout << "\tReturned false on decrypt flush command" << std::endl;
		return false;
	}

	testing->destroyKeys();
	return Verify(msg, len, buffPlaintext);
}

// Calls the static Crypto factory method and tests both the encryption and decryption processes
bool doTest(const char *test, const uint8_t *msg, uint32_t len)
{
	std::cout << test << " - Start" << std::endl;
	bool passed = true;

	passed &= testBlockSize(1,  msg, len);
	passed &= testBlockSize(15, msg, len);
	passed &= testBlockSize(16, msg, len);
	passed &= testBlockSize(17, msg, len);

	return passed;
}

void report(TestResults *results, const char *name, bool test) {
	++(results->total);
	if (test) {
		++(results->passed);
		std::cout << name << " - Passed" << std::endl;
	}
	else {
		++(results->failed);
		std::cout << name << " - Failed" << std::endl;
	}
	std::cout << std::endl;
}

int main() {
	std::cout
		<< "+--------------------------------------------------+" << std::endl
		<< "|            Programming Exercise 2: AES           |" << std::endl
		<< "|                    Unit Tests                    |" << std::endl
		<< "+--------------------------------------------------+" << std::endl
		<< std::endl;

	// Counters across tests
	TestResults results;

	// The data to test
	constexpr char *test1 = "Small message";
	constexpr uint8_t m1[] = "Hello";
	constexpr int m1Len = sizeof(m1) - 1;
	report(&results, test1, doTest(test1, m1, m1Len));

	// The data to test
	constexpr char *test2 = "Complete Block";
	constexpr uint8_t m2[] = "123456789012345";
	constexpr int m2Len = sizeof(m2) - 1;
	report(&results, test2, doTest(test2, m1, m2Len));

	// The data to test
	constexpr char *test3 = "Overfull Block";
	constexpr uint8_t m3[] = "1234567890123456";
	constexpr int m3Len = sizeof(m3) - 1;
	report(&results, test3, doTest(test3, m1, m3Len));
	
	// The data to test
	constexpr char *test4 = "Large Message";
	constexpr uint8_t m4[] = "Water... Earth... Fire... Air. "
		"Long ago, the four nations lived together in harmony. "
		"Then everything changed when the Fire Nation attacked. "
		"Only the Avatar, master of all four elements, could stop them. "
		"But when the world needed him most, he vanished. "
		"A hundred years passed and my brother and I discovered the new Avatar, an airbender named Aang. "
		"And although his airbending skills are great, he still has a lot to learn before he's ready to save anyone. "
		"But I believe Aang can save the world.";
	constexpr int m4Len = sizeof(m4) - 1;
	report(&results, test4, doTest(test4, m1, m4Len));
	
	std::cout
		<< "+--------------------------------------------------+" << std::endl
		<< "|                     Summary:                     |" << std::endl
		<< "| Tests Attempted: " << results.total << std::endl
		<< "| Tests Passed: " << results.passed << std::endl
		<< "| Tests Failed: " << results.failed << std::endl
		<< "+--------------------------------------------------+" << std::endl;

	return 0;
}
