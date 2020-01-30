#include "SPI.h"     // SPI library
#include "SD.h"      // SD card library
#include "SdCard.h"

/**
 * This is a quick wrapper for Arduino's SD library, therefore
 * it currently only supports one SD Card connection.
 * It also only support SPI bus 0
 */

static File sdFile;
static SdCard *sd_pt = NULL;
SdCard *SdCard::ConnectSdCard(const uint8_t spi, const uint8_t pin_cs)
{
	if (0 != spi) return NULL; // only support SPI 0
	if (sd_pt) return sd_pt;   // SD object already exist

	if (!SD.begin(pin_cs)) return NULL; // failed to connect SD card
	return sd_pt = new SdCard(spi, pin_cs); // return new obj if success
}

SdCard::SdCard(const uint8_t spi, const uint8_t pin_cs)
{
	bufferCnt = 0;
}

int SdCard::openNextFile(void)
{
	uint16_t num_next = 0;
	char  file_name[16] = "log_";
	char *file_num = file_name + strlen(file_name);
	sprintf(file_num, "%u", num_next);

	// if file already exist and num_next have not reach max value
	while (SD.exists(file_name) && num_next < UINT16_MAX)
		sprintf(file_num, "%u", ++num_next);

	// if num_next did not reach max value
	if (num_next < UINT16_MAX)
		return openFile(file_name);
	return -1;
}

int SdCard::openFile(const char *file)
{
	if (sdFile) {       // if current file is open
		sdFile.close(); // close current file
		bufferCnt = 0;  // reset counter
		delay(5);       // delay 5ms for hw to catch up
	}
	// open new file for write
	if ((sdFile = SD.open(file, FILE_WRITE))) {
		const char log_start[] = "start log...\n";
		write(log_start, sizeof(log_start) - 1);
		return 0;
	}
	return -1; // failed to open file
}

bool SdCard::isFileOpen(void)
{
	return sdFile ? true : false;
}

int SdCard::write(const char *buf, size_t count)
{
	if (!sdFile) return -1;
	int nbytes = 0, bytes;

	uint16_t buf_avail;
	while ((buf_avail = SD_BUFFER_SIZE - bufferCnt) < count) {
		// while data to write is bigger than available buffer

		// write up to available space and flush
		if ((bytes = sdFile.write((const uint8_t *)buf, buf_avail)) < 0)
			return -1; // failed to write
		nbytes += bytes;
		sdFile.flush();

		bufferCnt = 0;      // buffer is flushed, reset counter
		count -= buf_avail; // remaining length to write
		buf   += buf_avail; // move pointer to remaining buf
	}

	// write remaining buf
	if (0 < count) {
		if ((bytes = sdFile.write((const uint8_t *)buf, count)) < 0)
			return -1; // failed to write
		nbytes += bytes;
		bufferCnt += count;
	}

	return nbytes;
}
