#include <string_view>

#include <MultiFormatReader.h>
#include <MultiFormatWriter.h>
#include <ReadBarcode.h>
#include <TextUtfEncoding.h>
#include <DecodeHints.h>
#include <CharacterSet.h>
#include <BarcodeFormat.h>
#include <BitMatrix.h>
#include <ImageView.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

// simple header-only PNG writer
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

using namespace ZXing;

// Save BitMatrix to PNG manually
static void SaveMatrixAsPNG(const BitMatrix& matrix, const std::string& outputPath)
{
    int width = matrix.width();
    int height = matrix.height();
    std::vector<unsigned char> pixels(width * height * 3, 255); // white background

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            bool set = matrix.get(x, y);
            int idx = (y * width + x) * 3;
            if (set) {
                pixels[idx + 0] = 0;
                pixels[idx + 1] = 0;
                pixels[idx + 2] = 0; // black pixel
            }
        }
    }

    if (!stbi_write_png(outputPath.c_str(), width, height, 3, pixels.data(), width * 3))
        throw std::runtime_error("Failed to write PNG: " + outputPath);
}

// Generate barcode of any supported format
extern "C" __declspec(dllexport)
bool ZXGenerateBarcode(const char* text, const char* outputPath, const char* formatName, int size)
{
    try {
        std::string fmtStr = formatName ? std::string(formatName) : "QR_CODE";
        BarcodeFormat format = BarcodeFormat::QRCode;

        if (fmtStr == "QR_CODE" || fmtStr == "QR" || fmtStr == "qrcode") format = BarcodeFormat::QRCode;
        else if (fmtStr == "DATAMATRIX" || fmtStr == "DATA_MATRIX") format = BarcodeFormat::DataMatrix;
        else if (fmtStr == "PDF417") format = BarcodeFormat::PDF417;
        else if (fmtStr == "CODE128" || fmtStr == "CODE_128") format = BarcodeFormat::Code128;
        else if (fmtStr == "AZTEC") format = BarcodeFormat::Aztec;
        else if (fmtStr == "EAN13" || fmtStr == "EAN_13") format = BarcodeFormat::EAN13;
        else if (fmtStr == "EAN8" || fmtStr == "EAN_8") format = BarcodeFormat::EAN8;
        else if (fmtStr == "UPC_A") format = BarcodeFormat::UPCA;
        else if (fmtStr == "UPC_E") format = BarcodeFormat::UPCE;
        else if (fmtStr == "ITF") format = BarcodeFormat::ITF;
        else format = BarcodeFormat::QRCode;

        MultiFormatWriter writer(format);
        writer.setMargin(10);
        writer.setEncoding(CharacterSet::UTF8);

        auto matrix = writer.encode(std::string(text), size, size);

        SaveMatrixAsPNG(matrix, outputPath);

        std::cout << "✅ Barcode generated: " << outputPath << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "❌ Error generating barcode: " << e.what() << std::endl;
        return false;
    }
}

/**
 * Decode any supported barcode or QR code from an image file.
 * Supported formats: QR, DataMatrix, PDF417, Aztec, Code128, Code39,
 * EAN-8/13, UPC-A/E, ITF, MaxiCode, RSS14/Expanded.
 */
extern "C" __declspec(dllexport)
const char* ZXDecodeImage(const char* imagePath)
{
    static std::string result;

    try {
        int w = 0, h = 0, channels = 0;
        unsigned char* pixels = stbi_load(imagePath, &w, &h, &channels, 1); // grayscale
        if (!pixels)
            throw std::runtime_error("Cannot load image: " + std::string(imagePath));

        DecodeHints hints;
        hints.setTryHarder(true);
        hints.setTryRotate(true);
        hints.setTryInvert(true);

        // In ZXing-cpp v2.3.0, DecodeHints::setFormats(BarcodeFormat::All) does not exist yet
        // hints.setFormats(BarcodeFormats::All()); // enables all supported formats

        ImageView image(pixels, w, h, ImageFormat::Lum); // grayscale view
        auto res = ReadBarcode(image, hints);
        stbi_image_free(pixels);

        if (res.isValid()) {
            result = res.text();            
        } else {
            result = "No barcode found";
        }        
    }
    catch (const std::exception& e) {
        result = std::string("Error: ") + e.what();
    }
    catch (...) {
        result = "Unknown error while decoding image";
    }

    return result.c_str();
}
