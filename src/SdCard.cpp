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
	if (0 == spi && NULL == sd_pt) {
		sd_pt = new SdCard(spi, pin_cs);
		return sd_pt;
	}
	return NULL;
}

SdCard::SdCard(const uint8_t spi, const uint8_t pin_cs)
{
	isSDConnected = SD.begin(pin_cs);
	isFileConnected = false;
	bufferCnt = 0;

}

int SdCard::openNextFile(void)
{
	if (isSDConnected){ // if SD card is connected
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
	}
	return -1; // no SD card or num_next has reach max
}

int SdCard::openFile(const char *file)
{
	if (isSDConnected) {
		if (sdFile) {       // if current file is open
			sdFile.close(); // close current file
			delay(5);       // delay 5ms
			isFileConnected = false;
		}
		// open new file for write
		if ((sdFile = SD.open(file, FILE_WRITE))) {
			const char log_start[] = "start log...";
			write(log_start, sizeof(log_start));
			isFileConnected = true;
			return 0;
		}
	}
	return -1; // failed to open file
}

int SdCard::write(const char *buf, size_t count) {
	if (!isFileConnected) return -1;
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
