# buzz-bombers-game
A recreation of the classic Intellivision game Buzz Bombers for my CS1002 course.
## How to Build and Run

This project is built in C++ and requires the SFML library to be installed on your system.

#### **Prerequisites**
- A C++ Compiler (like g++ or the compiler included with Visual Studio).
- **SFML Library (v2.5.1 or newer)**. You must have SFML installed and linked correctly.

#### **Build & Run Instructions (Using g++ on Linux/macOS/Windows with MinGW)**

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/SabihaAbidd/buzz-bombers-game.git
    ```

2.  **Navigate to the project directory:**
    ```bash
    cd buzz-bombers-game
    ```

3.  **Compile the source code:**
    *(This command assumes all your asset folders like `Textures` and `Music` are in the same directory as the executable)*
    ```bash
    g++ buzz.cpp -o BuzzBombers -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    ```
    *Note: The main source file is `buzz.cpp`.*

4.  **Run the game:**
    - On Windows: `BuzzBombers.exe`
    - On macOS/Linux: `./BuzzBombers`
