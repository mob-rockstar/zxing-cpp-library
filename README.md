# zxing-cpp-library
ZXing-Cpp (C++) library with support for generating .dll and .lib files for integration into other projects.

Features

- Decode all major barcode types (QR, DataMatrix, PDF417, Aztec, Code128, Code39, EAN-8/13, UPC-A/E)
- Generate QR, Code128, EAN13, PDF417, DataMatrix, etc.
- C-compatible API for use in C++, C#, Python, Node.js, etc.
- Uses stb_image / stb_image_write for self-contained image handling (no OpenCV dependency).

Getting Started

Folder structure should be like;
```
C:/dev/
├── zxing-cpp/          # Clean ZXing-CPP source (from GitHub)
├── zxing-build/        # Compiled binaries (zxing.lib, zxing.dll, etc.)
├── zxing-app/          # Your wrapper DLL project
```

1️⃣ Clone the ZXing-CPP repository

Open a Developer Command Prompt for VS 2022 (or PowerShell with cmake in PATH) and run:
```
cd C:\dev
git clone https://github.com/zxing-cpp/zxing-cpp.git
```

That creates:
```
C:/dev/zxing-cpp/
```

2️⃣ Create a separate build folder
```
mkdir C:\dev\zxing-build
cd C:\dev\zxing-build
```

3️⃣ Generate Visual Studio solution
```
cmake -G "Visual Studio 17 2022" -A x64 ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DBUILD_SHARED_LIBS=OFF ^
  C:\dev\zxing-cpp
```

Notes:
-A x64 → 64-bit build

-DBUILD_SHARED_LIBS=OFF → build a static library (zxing.lib), easiest to link from your wrapper

You can change to ON if you really need zxing.dll

This will create the file:
```
	C:/dev/zxing-build/zxing.sln
```
4️⃣ Build ZXing
```
cmake --build . --config Release
```
When it finishes you’ll see:
```
C:/dev/zxing-build/Release/zxing.lib
```
That’s the binary your wrapper project will link against.

5️⃣ Verify the header path
Headers live here:
```
C:/dev/zxing-cpp/core/src/

Examples:
	BarcodeFormat.h
	ReadBarcode.h
	WriteBarcode.h
	Result.h
	DecodeHints.h
	TextUtfEncoding.h
```
Your wrapper’s CMake file already includes this path:
```
include_directories(${ZXING_ROOT}/core/src)
```
***************************************
          zxing-wrapper 
***************************************
```
C:/dev/zxing-wrapper/
├── src/
│   ├── zxing_wrapper.cpp
│   ├── stb_image.h
│   └── stb_image_write.h
├── include/
│   └── zxing_wrapper.h
├── CMakeLists.txt
```
1. include/zxing_wrapper.h
2. src/zxing_wrapper.cpp
3. CMakeLists.txt
4. Build

```
cd C:\dev\zxing-wrapper
mkdir build && cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```
After building, check:
```
C:/dev/zxing-wrapper/build/Release/
├── zxing_wrapper.dll
├── zxing_wrapper.lib
```

***************************************
         How to use the DLL 
***************************************
⚙️ Step 1. Create a new C++ project
- Open Visual Studio 2022
  
- Click “Create a new project”
  
- In the search box, type:
  ```
    console app
  ```
- Select:
	Console App (C++)
- Click Next
- Set up the project:
  
	Project name: MyZXingApp

	Location: C:/dev

	✅ Make sure “Place solution and project in the same directory” is checked

- Click Create

⚙️ Step 2: Set configuration

- Once the project opens:
  
	At the top toolbar, set;

    Configuration: Release
  
    Platform: x64
  
	This must match how your DLL was built.

📁 Step 3: Create folder structure

- In File Explorer (not Visual Studio):

```
C:/dev/MyZXingApp/
├── libs/
│   ├── zxing-wrapper.lib
│   ├── zxing-wrapper.dll
├── include/
│   └── zxing_wrapper.h        ← copy from your wrapper project
└── MyZXingApp.sln
```

✅ Copy the following from your previous build:

zxing_wrapper.h → into include

zxing-wrapper.lib → into libs

zxing-wrapper.dll → into libs

🧭 Step 4: Configure project properties

- In Solution Explore, Right-click on the project → Properties

- Then set the following:

Configuration Properties → C/C++ → General	    Additional Include Directories	C:\dev\my-app\include

Configuration Properties → Linker → General	    Additional Library Directories	C:\dev\my-app\libs

Configuration Properties → Linker → Input	    Additional Dependencies	zxing-wrapper.lib

- Click Apply and OK

⚙️ Step 4 — Copy the DLL beside your EXE

- After linking succeeds, copy:
```
C:/dev/zxing-wrapper/build/Release/zxing-wrapper.dll
```
- into your app’s output folder:
```
C:/dev/MyZXingApp/x64/Release/
```
- Otherwise, the program will fail at runtime with a “DLL not found” error.

To read & write barcodes;

// Written by C++
<pre>#include &lt;iostream&gt
#include "zxing_wrapper.h"
int main()
{
    // Decoding qr image
    const char* imagePath = "C:\\dev\\images\\sample_qr.png";
    const char* decoded = ZXDecodeImage(imagePath);
    std::cout << "Decoded Result: " << decoded << std::endl;

    // Generating the qrcode
    const char* text = "https://www.example.com";
    const char* outputPath = "C:\\dev\\images\\generated_qr.png";
    ZXGenerateBarcode(text, outputPath, "QR_CODE", 256);

    return 0;
} </pre>

