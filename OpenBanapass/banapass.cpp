#include "banapass.h"
#include "log.h"
#include <filesystem>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <format>
// #include <mutex>
#include <future>

// Open Banapassport Version (For Logging)
constexpr auto OPEN_BANA_VERSION = "2.1.0";

// Banapassport API Version
constexpr auto BANA_API_VERSION = "Ver 1.6.0";

// Card filename which is monitored for changes
constexpr auto CARD_FILENAME = "card.txt";

// Card filename to use as backup 'if key is pressed'
constexpr auto BACKUP_FILENAME = "card.backup.txt";

// Key which is pressed to use backup card
constexpr auto BACKUP_KEY = 'C';

// Delimiter key for the card files
constexpr auto DELIM_KEY = ';';

// Sleep delay (wait for thread) in ms
const auto WAIT_TOUCH_TIMEOUT = 250;

// Standard Namespace
using namespace std;

// Card Struct
struct Card 
{
	// Card Chip ID
	std::string chipId;

	// Car Access Code
	std::string accessCode;
};

// randomHex(str[]: char, length: int): void
// Given a char array and a length, generates
// a random hex string and returns it to the caller.
void randomHex(char str[], int length)
{
	//hexadecimal characters
	char hexCharacterTable[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

	// Seed the random number generator
	srand(time(NULL));
	
	// Write a random character from the character set to each index of the array
	for (int i = 0; i < length; i++) { str[i] = hexCharacterTable[rand() % 16]; }
	
	// Set the last index to zero
	str[length] = 0;
}

// randomNumberString(str[]: char, length: int): void
// Given a char array and a length, generates a random
// number string and returns it to the caller.
void randomNumberString(char str[], int length)
{
	// Number character table
	char CharacterTable[] = { '0','1','2','3','4','5','6','7','8','9'};

	// Seed the random number generator
	srand(time(NULL));

	// Write a random character from the character set to each index of the array
	for (int i = 0; i < length; i++) { str[i] = CharacterTable[rand() % 10]; }

	// Set the last index to zero
	str[length] = 0;
}

// createCard(void): void
// If the backup card file does not
// exist, it is created using a random
// chip ID and access code, seperated
// using a semicolon.
void createCard() 
{
	// If the backup filename already exists, do not create it
	if (std::filesystem::exists(BACKUP_FILENAME))
	{
		log("Backup card file already exists, and has not been created.\n");
	}
	else // Backup filename does not exist
	{
		// Generate the random chip id
		char generatedChipId[21] = "00000000000000000000";
		randomNumberString(generatedChipId, 20);

		// Generate the random access code
		char generatedAccessCode[33] = "00000000000000000000000000000000";
		randomHex(generatedAccessCode, 32);

		// Create the output filestream
		std::ofstream stream;

		// Open the output filestream for the backup filename
		stream.open(BACKUP_FILENAME, std::ofstream::out | std::ofstream::app);

		// Write the chipId / accessCode to the file, seperated using a semicolon
		// e.g. 5a6d546dc795d441eee568842a8c7f46;17214280067552411462
		stream << generatedChipId << ";" << generatedAccessCode;

		// Close the file stream
		stream.close();
	}
}

// clearCard(filename: char*): void
// Given a filename, clears the contents
// in the given file.
void clearCard(const char* filename) 
{
	// If the backup filename already exists, do not create it
	if (std::filesystem::exists(filename))
	{
		// Create the output filestream
		std::ofstream stream;

		// Open the output filestream with output, truncate privileges
		stream.open(filename, std::ofstream::out | std::ofstream::trunc);

		// Close the file stream
		stream.close();

		log("Card file found. Data cleared.\n");
	}
	else
	{
		log("Card file not found, not cleared.\n");
	}
}

// getCard(filename: char *): void
// Given a filename, attempts to open
// the given file and writes the card
// properties to the given card struct.
bool getCard(const char * filename, Card* card) 
{
	// Create the input filestream
	std::ifstream stream;

	// Open the input filestream for the given filename
	stream.open(filename, std::ifstream::in);

	// Raw card string
	string rawCard;

	// Write the raw card data from 
	// the stream to the string
	stream >> rawCard;

	// Find the index of the delimiter in the string
	int index = rawCard.find(DELIM_KEY);

	// If the delimiter key is found
	if (index != std::string::npos)
	{
		// Get the substring for the chip id
		card->chipId = rawCard.substr(0, index);

		log("Chip ID: %s\n", (card->chipId).c_str());

		// Get the substring for the access code
		card->accessCode = rawCard.substr(index + 1, rawCard.length() - 1);

		log("Access Code: %s\n", (card->accessCode).c_str());

		// Success, return true
		return true;
	}
	else // Delim key not found
	{
		log("Delimiter not present in card file!\n");

		// Failed, return false
		return false;
	}
}

extern "C"
{

	ULONGLONG BngRwAttach(UINT a1, char* a2, int a3, int a4, void* callback, long a6)
	{
		log("BngRwAttach()\n");
		createCard();
		// Do something with the callback perhaps?
		// Idk what it does
		return 1;
	}

	long BngRwInit()
	{
		log("Open Banapass Ver. %s is starting ...\n", OPEN_BANA_VERSION);
		log("BngRwInit()\n");
		return 0;
	}

	ULONGLONG BngRwReqSetLedPower()
	{
		log("BngRwSetLedPower()\n");
		return 0;
	}

	int BngRwDevReset(UINT a, ULONGLONG b, ULONGLONG c)
	{
		log("BngRwDevReset()\n");
		return 0;
	}

	ULONGLONG BngRwExReadMifareAllBlock()
	{
		log("BngRwExReadMifareAllBlock()\n");
		return 0xffffff9c;
	}

	// Finalise?
	void BngRwFin()
	{
		log("BngRwFin()\n");
	}

	UINT BngRwGetFwVersion(UINT a)
	{
		log("BngRwGetFwVersion()\n");
		return 0;
	}

	UINT BngRwGetStationID(UINT a)
	{
		log("BngRwGetStationID()\n");
		return 0;
	}

	const char* BngRwGetVersion()
	{
		log("BngRwGetVersion()\n");
		return BANA_API_VERSION;
	}

	ULONGLONG BngRwIsCmdExec(UINT a)
	{
		log("BngRwIsCmdExec()\n");
		return 0xFFFFFFFF;
	}

	UINT BngRwGetTotalRetryCount(UINT a)
	{
		log("BngRwGetTotalRetryCount()\n");
		return 0;
	}

	int BngRwReqLed(UINT a, UINT b, ULONGLONG c, ULONGLONG d)
	{
		log("BngRwReqLed()\n");
		return 1;
	}

	int BngRwReqAction(UINT a, UINT b, ULONGLONG c, ULONGLONG d)
	{
		log("BngRwReqAction()\n");
		return 1;
	}

	int BngRwReqAiccAuth(UINT a, int b, UINT c, int* d, ULONGLONG e, ULONGLONG f, ULONGLONG* g)
	{
		log("BngRwReqAiccAuth()\n");
		return 1;
	}

	int BngRwReqBeep(UINT a, UINT b, ULONGLONG c, ULONGLONG d)
	{
		log("BngRwReqBeep()\n");
		return 1;
	}

	int BngRwReqCancel(UINT a)
	{
		log("BngRwReqCancel()\n");
		if (7 < a)
		{
			return -100;
		}
		return 1;
	}

	int BngRwReqFwCleanup(UINT, ULONGLONG, ULONGLONG)
	{
		log("BngRwReqFwCleanup()\n");
		return 1;
	}

	int BngRwReqFwVersionup(UINT a, ULONGLONG b, ULONGLONG c, ULONGLONG d)
	{
		log("BngRwReqFwVersionup()\n");
		return 1;
	}

	int BngRwReqLatchID(UINT a, ULONGLONG b, ULONGLONG c)
	{
		log("BngRwReqLatchId()\n");
		if (a < 8)
		{
			return -100;
		}
		return 1;
	}

	int BngRwReqSendMailTo(UINT a, int b, UINT c, int* d,
		char* e, char* f, char* g, char* h, ULONGLONG i, ULONGLONG j)
	{
		log("BngRwReqSendMailTo()\n");
		if (7 < a)
		{
			return -100;
		}
		if (!e)
		{
			return -100;
		}
		return 1;
	}

	int BngRwReqSendUrlTo(UINT a, int b, UINT c, int* d,
		char* e, char* f, ULONGLONG g, ULONGLONG h)
	{
		log("BngRwReqSendUrlTo()\n");
		if (7 < a)
		{
			return -100;
		}
		if (!e)
		{
			return -100;
		}
		return 1;
	}

	// Debug: Callback handler process
	void BngRwReqWaitTouchCallbackHandler(Card * card, void (*callback)(int, int, void*, void*), void * e) {

		log("Callback handler thread start ...\n");

		// Raw card data and some other stuff, who cares
		unsigned char rawCardData[168] = {
			0x01, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x92, 0x2E, 0x58, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x7F, 0x5C, 0x97, 0x44, 0xF0, 0x88, 0x04, 0x00,
			0x43, 0x26, 0x2C, 0x33, 0x00, 0x04, 0x06, 0x10, 0x30, 0x30, 0x30, 0x30,
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
			0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30,
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
			0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
			0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x4E, 0x42, 0x47, 0x49, 0x43, 0x36, 0x00, 0x00, 0xFA, 0xE9, 0x69, 0x00,
			0xF6, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

		log("Raw card data allocated ...\n");

		// Copy the access code into the raw card data
		memcpy(rawCardData + 0x50, card->accessCode.c_str(), card->accessCode.size() + 1);

		log("Access code written ...\n");

		// Copy the chip id into the raw card data
		memcpy(rawCardData + 0x2C, card->chipId.c_str(), card->chipId.size() + 1);

		log("Card chip id written ...\n");

		// Create the callback thread
		std::thread t(callback, 0, 0, rawCardData, e);

		// Wait for thread to end
		t.join();

		log("Callback handler thread end.\n");
	}

	int BngRwReqWaitTouch(UINT a, int maxIntSomehow, UINT c, void (*callback)(int, int, void*, void*), void* e)
	{
		log("BngRwReqWaitTouch()\n");

		// Create a new card struct
		Card card;

		// By default, card not read
		bool read = false;

		// If there is card data in the card file
		if (getCard(CARD_FILENAME, &card)){

			// Card data read
			read = true;

			log("Card data received!\n");

			// Clear the card data
			clearCard(CARD_FILENAME);
		}
		// Otherwise, if the backup key is pressed
		else if (GetAsyncKeyState(BACKUP_KEY))
		{
			// Attempt to read the backup card data
			read = getCard(BACKUP_FILENAME, &card);

			log("Key pressed. Using backup data.\n");
		}

		// If the card was read successfully
		if (read)
		{
			log("Card data read successfully. Starting callback thread ...\n");

			std::thread t(BngRwReqWaitTouchCallbackHandler , &card, callback, e);

			// Detach the thread
			t.detach();

			// Wait 250ms before continuing
			std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TOUCH_TIMEOUT));

			// Success response
			return 1;
		}
		else 
		{
			// Not found response
			return -1;
		}
	}
}