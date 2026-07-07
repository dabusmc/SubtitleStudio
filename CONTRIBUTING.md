# Building

Requirements:

- Visual Studio 2022
- Qt 6.11 MSVC2022 64-bit
- CMake 3.21+

Clone the repository.

Configure CMake (set your local Qt path if required).

Build.

Run.

## Configuring CMake

Open `CMakePresets.json` and change `CMAKE_PREFIX_PATH` to the path to Qt's MSVC folder (e.g. `"C:/Qt/6.11.1/msvc2022_64"`)