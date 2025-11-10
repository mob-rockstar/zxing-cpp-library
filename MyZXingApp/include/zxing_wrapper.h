
#pragma once

#include <stdbool.h>  // For C-compatible bool
#include <stddef.h>   // For size_t
#include <string>

// #ifdef ZXING_WRAPPER_EXPORTS
//     // When building the DLL itself
//     #define ZXING_API __declspec(dllexport)
// #else
//     // When including this header in another project (e.g., MyZXingApp)
//     #define ZXING_API __declspec(dllimport)
// #endif

extern "C" {

    /**
     * Decode any supported barcode (QR, DataMatrix, PDF417, etc.) from an image file.
     * @param imagePath - Path to PNG/JPG image.
     * @return UTF-8 text result, or error message (static buffer).
     */
    const char* ZXDecodeImage(const char* imagePath);

    /**
     * Generate a barcode or QR code and save to a PNG file.
     * @param text - text to encode
     * @param format - string name ("QR_CODE", "DATA_MATRIX", "PDF417", "CODE_128", "EAN_13", "EAN_8" "AZTEC", "UPC_A", "UPC_E", etc.)
     * @param outputPath - where to save the PNG
     * @param size - width/height in pixels
     * @return 1 on success, 0 on failure
     */
    bool ZXGenerateBarcode(const char* text, const char* outputPath, const char* formatName, int size);

}
