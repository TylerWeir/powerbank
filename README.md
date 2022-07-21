# powerbank
Network based telemetry publisher for a trickle charge battery bank.


## Network Protocol

### ESP to Data Server
Structs should not be used as network protocols. Define an apllication protocle in
octets and write a library to pack and unpack structs into/from an octet stream.

Protocol needs to answer three basic questions:
1. What problem is the protocol trying to acheive?
	- Transmit telemetry from the ESP32 device to a server so the the data may be logged.
2. What messages are being transitted and what do they mean?
	- In the simplest form, a message would contain a single piece of telemetry containing an identifier, a value, and a timestamp.
	- In a larger form, a message may contain many pieces of said telemetry all transmitted in a single UDP packet.
3. What are the important, but unobvious, features of the protocol?
	- A single UDP packet may contain any number of these units of data.

**Telemetry Definition**
```
struct Telemetry {
	char id[4];
	float value;
	char timestamp[8];
}


ESP-IDF uses 32 bit time_t
```


**Protocol Definition**
A single piece of telemetry may be represented by 16 consecutive bytes:
id, value, timestamp ----> [4, 4, 8]

A packet sent via UDP must contain some multiple of 16 bytes (16, 32, 48, etc...)
The data is read into a buffer, and read into the Telemetry struct defined above according to the byte sizes. 
The end of a 16 byte chunk designates the end of a piece of telmetry and the begining of a new piece



**Esp32 program Pseudocode**
```
for things to measure {
	// read data into struct
	Telemetry t;
	t.id = "volt"
	t.value = gpio.read(pin#)
	t.timestamp = gettimeofday()

	buffer.append(tobytestring(t))
}

socket.send_from_buffer(buffer)
```





## Misc Notes

### Time
```
#include <sys/time.h>

// gettimeofday takes a timeval struct and modifies it to hold the time since the Epoch
int gettimeofday(struct timeval *restrict tv, struct timezone *restrict tz);


// getting the time looks like this
struct timeval *mytime;

if(gettimeofday(mytime, Null)) {
	// There was an error
}





// From ESP docs to get time with 1s acc:
time_t now;
char strftime_buf[64];
struct tm timeinfo;

time(&now);
// Set timezone to China Standard Time
setenv("TZ", "CST-8", 1);
tzset();

localtime_r(&now, &timeinfo);
strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
```
