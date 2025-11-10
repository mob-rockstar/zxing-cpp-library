
#pragma once
#include <string>

#ifdef ZXING_WRAPPER_EXPORTS
    #define ZXING_API __declspec(dllexport)
#else
    #define ZXING_API __declspec(dllimport)
#endif

extern "C" {

    /**
     * Decode any supported barcode (QR, DataMatrix, PDF417, etc.) from an image file.
     * @param imagePath - Path to PNG/JPG image.
     * @return UTF-8 text result, or error message (static buffer).
     */
    ZXING_API const char* ZXDecodeImage(const char* imagePath);

    /**
     * Decode barcode from memory (RGB buffer).
     * @param data - pointer to image buffer
     * @param width, height - dimensions
     * @param channels - typically 3
     * @return UTF-8 result or error.
     */
    // ZXING_API const char* decode_barcode_from_memory(const unsigned char* data, int width, int height, int channels);

    /**
     * Generate a barcode or QR code and save to a PNG file.
     * @param text - text to encode
     * @param format - string name ("QR_CODE", "DATA_MATRIX", "PDF417", "CODE_128", "EAN_13", "EAN_8" "AZTEC", "UPC_A", "UPC_E", etc.)
     * @param outputPath - where to save the PNG
     * @param size - width/height in pixels
     * @return 1 on success, 0 on failure
     */
    ZXING_API bool ZXGenerateBarcode(const char* text, const char* outputPath, const char* formatName, int size);

}
