#include "log.h"
#include <stdio.h>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <fstream>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Log File Name
#define LOG_FILE "bngrw.log"

// Generate time stamp string
std::string timestamp() {
	
	// Timestamp Buffer Size
	const int TZBUFSIZ = 80;

	// Time variables
	std::time_t rawtime;
	std::tm* timeinfo;

	// String Buffer
	char buffer[TZBUFSIZ];

	// Assign the current time to rawtime
	std::time(&rawtime);

	// Get local time conversion of rawtime
	timeinfo = std::localtime(&rawtime);

	// Write the string timestamp to the buffer
	std::strftime(buffer, TZBUFSIZ, "%Y-%m-%d-%H-%M-%S", timeinfo);

	// Generate a string object using the buffer
	std::string timestamp = std::string(buffer);

	// Return the timestamp
	return timestamp;
}

// Write event to stdout / logfile
void log(const char* format, ...)
{
	// Get timestamp from the callback
	std::string ts = timestamp();

	va_list args;
	va_start(args, format);

// If log file is defined
#ifdef LOG_FILE

	// Write to log file
	FILE* file = fopen(LOG_FILE, "a");

	// Print the timestamp to the file
	fprintf(file, "[%s] ", ts.c_str());

	// Pass argument list to vfprintf
	vfprintf(file, format, args);

	// Close the stream
	fclose(file);

#else // Log file not defined

	// Write to stdout
	char buffer[1024];

	char fmt[1024] = "[OpenBanapass] ";
	strcat(fmt, format);
	vsprintf(buffer, fmt, args);
	OutputDebugStringA(buffer);

#endif
	va_end(args);
}