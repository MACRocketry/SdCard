#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include <stdint.h>
#include <stddef.h>

/**
 * Note: all function follows the same return value
 *       unless stated otherwise
 *       
 * @return     (0 <= ret) if success
 *             (ret < 0)  if failed
 */

#define SD_BUFFER_SIZE 512

class SdCard {
public:
	/**
	 * @brief      connects an sd card
	 * @param      spi     The SPI bus (current library only support spi = 0)
	 * @param      pin_cs  The chip select pin
	 * @return     pointer to SdCard object, NULL if fail
	 * @note       repeated call (same spi) will return the same SdCard object
	 */
	static SdCard *ConnectSdCard(const uint8_t spi, const uint8_t pin_cs);

	/**
	 * @brief      opens file with next available number
	 * @return     return the file number, negative if fail
	 */
	int openNextFile(void);
	int openFile(const char *file); // open specific file

	bool isFileOpen(void);

	/**
	 * @brief      exactly same as normal c function write()
	 */
	int write(const char *buf, size_t count);

private:
	SdCard(const uint8_t spi, const uint8_t pin_cs);
	~SdCard(void){};

private:
	uint16_t bufferCnt; //buffer up to 512 char


};

#endif // !_SD_CARD_H_