# SugarCrash

![](readme-src/demonstration.gif)

# Build Instructions

Follow these steps to build the SugarCrash project:

1. **Download and install Java OpenJDK 8+**
    > Recommended OpenJDK from Eclipse Adoptium: https://adoptium.net/
2. **Download and install MSYS2**: https://www.msys2.org/
3. **Open the MSYS2 Shell**:
    - Locate the "MSYS2 MINGW64" shortcut in the Start menu.
    - Open the shell.
4. **Update Packages**: In the MSYS2 shell, run the following command:
    ```sh
    pacman -Syu pactoys git mingw-w64-x86_64-binutils
    ```
    > [!NOTE]
    > The MSYS2 shell may prompt you to restart it after installing packages and updating.
5. **Install Build Dependencies**: Use `pacboy` to install the necessary build tools
    ```sh
    pacboy -S toolchain:p cmake:p ninja:p
    ```
6. **Clone the Source Code** repository using Git:
    ```sh
    git clone https://github.com/alcoloid0/SugarCrash
    cd SugarCrash
    ```
7. **Build using CMake**
- Configure the project using CMake:
    ```sh
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    ```
- Build the project:
    ```sh
    cmake --build build
    ```
