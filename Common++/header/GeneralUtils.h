#ifndef PCAPPP_GENERAL_UTILS
#define PCAPPP_GENERAL_UTILS

#include <string>
#include <stdint.h>

/// @file

/**
 * \namespace pcpp
 * \brief The main namespace for the PcapPlusPlus lib
 */
namespace pcpp
{
	/**
	 * Convert a byte array into a string of hex characters. For example: for the array { 0xaa, 0x2b, 0x10 } the string
	 * "aa2b10" will be returned
	 * @param[in] byteArr A byte array
	 * @param[in] byteArrSize The size of the byte array [in bytes]
	 * @param[in] stringSizeLimit An optional parameter that enables to limit the returned string size. If set to a positive
	 * integer value the returned string size will be equal or less than this value. If the string representation of the
	 * whole array is longer than this size then only part of the array will be read. The default value is -1 which means no
	 * string size limitation
	 * @return A string of hex characters representing the byte array
	 */
	std::string byteArrayToHexString(const uint8_t* byteArr, size_t byteArrSize, int stringSizeLimit = -1);

	/**
	 * Convert a string of hex characters into a byte array. For example: for the string "aa2b10" an array of values
	 * { 0xaa, 0x2b, 0x10 } will be returned
	 * @param[in] hexString A string of hex characters
	 * @param[out] resultByteArr A pre-allocated byte array where the result will be written to
	 * @param[in] resultByteArrSize The size of the pre-allocated byte array
	 * @return The size of the result array. If the string represents an array that is longer than the pre-allocated size
	 * (resultByteArrSize) then the result array will contain only the part of the string that managed to fit into the
	 * array, and the returned size will be resultByteArrSize. However if the string represents an array that is shorter
	 * than the pre-allocated size then some of the cells will remain empty and contain zeros, and the returned size will
	 * be the part of the array that contain data
	 */
	size_t hexStringToByteArray(const std::string& hexString, uint8_t* resultByteArr, size_t resultByteArrSize);
}

#endif // PCAPPP_GENERAL_UTILS
