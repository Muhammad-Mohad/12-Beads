#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <fstream>
using namespace std;
using namespace sf;


const int board_Size = 5;
const int square_Size = 200;

// FUNCTIONS

void initializeBoard(char board[board_Size][board_Size], bool &isCyansTurn);
void restartGame(char board[board_Size][board_Size], bool &isCyansTurn);
void writeWinnerToFile(int cyanCount, int magentaCount);
void displayGameWinFile();
void saveGameState(const char board[board_Size][board_Size], bool isCyansTurn);
void loadGameState(char board[board_Size][board_Size], bool &isCyansTurn);



int main() {
    char board[board_Size][board_Size];
    bool restart = false;
    bool isMenuState = true;

    srand(static_cast<unsigned int>(time(nullptr)));

    // Initializing the Board
    bool isCyansTurn;
    initializeBoard(board, isCyansTurn);


    float backgroundSpeed = 50.0f;
    float backgroundPositionX = 0.0f;

    // Time
    Clock clock;

    // TEXTURES
    Texture gameexit1;
    gameexit1.loadFromFile("./Assets/Images/gameexit3.png");
    Sprite exit(gameexit1);

    Texture gameexit2;
    gameexit2.loadFromFile("./Assets/Images/gameexit4.png");
    Sprite exit2(gameexit2);

    Texture texture;
    texture.loadFromFile("./Assets/Images/beads_texture1.jpg");
    texture.setSmooth(true);

    Texture lines;
    lines.loadFromFile("./Assets/Images/lines4.png");
    Sprite linesprite(lines);

    Texture another;
    another.loadFromFile("./Assets/Images/bg2.jpg");
    Sprite sprite(another);

    Texture menu;
    menu.loadFromFile("./Assets/Images/menu3.png");
    Sprite menusprite(menu);



    // BEADS
    CircleShape bead(square_Size / 2 - 60);
    bead.setTexture(&texture);


    // FONTS
    Font font;
    font.loadFromFile("./Assets/Font/fonts.ttf");



    // TEXT
    Text text2("Cyan's Turn", font, 60);
    text2.setFillColor(Color(5, 153, 158));
    text2.move(992.0f, 370.0f);

    Text text3("Magneta's Turn", font, 60);
    text3.setFillColor(Color(120, 22, 186));
    text3.move(940.0f, 370.0f);


    Text title1("12", font, 65);
    title1.setFillColor(Color(120, 22, 186));
    title1.move(1030.0f, 15.0f);

    Text title2("BEADS", font, 65);
    title2.setFillColor(Color(5, 153, 158));
    title2.move(1117.0f, 15.0f);

    Text save("Save Game", font, 50);
    save.setFillColor(Color::Black);
    save.setOutlineColor(Color::White);
    save.setOutlineThickness(2);
    save.move(1050.0f, 790.0f);

    Text load("Load Game" , font , 50);
    load.setFillColor(Color::Black);
    load.setOutlineColor(Color::White);
    load.setOutlineThickness(2);
    load.move(1050.0f, 860.0f);

    Text stats("Stats", font, 50);
    stats.setFillColor(Color::Black);
    stats.setOutlineColor(Color::White);
    stats.setOutlineThickness(2);
    stats.move(1095.0f, 720.0f);


    // AUDIO EFFECTS
    SoundBuffer buffer_move;
    buffer_move.loadFromFile("./Assets/Audio/move.wav");
    Sound sound_move;
    sound_move.setBuffer(buffer_move);

    SoundBuffer buffer_invalid;
    buffer_invalid.loadFromFile("./Assets/Audio/crush.wav");
    Sound invalid;
    invalid.setBuffer(buffer_invalid);

    SoundBuffer victory;
    victory.loadFromFile("./Assets/Audio/victory.wav");
    Sound victoryy;
    victoryy.setBuffer(victory);

    SoundBuffer connor;
    connor.loadFromFile("./Assets/Audio/connor.wav");
    Sound hehehaw;
    hehehaw.setBuffer(connor);
    hehehaw.setLoop(true);



    // GAME WINDOW
    RenderWindow window(VideoMode(board_Size * square_Size + 400, board_Size * square_Size), "12 Beads Game");   // Window Size based on Beads
    window.setFramerateLimit(120);
    hehehaw.play();

    // Bead Position
    Vector2i selectedBeadPosition;
    bool isBeadSelected = false;

    while (window.isOpen()) {
        Time elapsed = clock.restart();
        Event event;
        while (window.pollEvent(event)) {

            Vector2i mousePosition = Mouse::getPosition(window);
            Color originalColor = Color::Black;
            Color hoverColor = Color(142, 172, 205);


            if (event.type == Event::Closed) {
                window.close();
            }
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::E) {
                window.close();
            }
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::R) {
                restart = true;
            }
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::S) {
                isMenuState = false;  // Switch to Board when 'S' is pressed
                victoryy.play();
            }
            else if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {

                    // Taking positions
                    Vector2i mousePosition = Mouse::getPosition(window);

                    if (save.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        saveGameState(board, isCyansTurn);
                    }
                    else if(load.getGlobalBounds().contains(mousePosition.x, mousePosition.y)){
                        loadGameState(board, isCyansTurn);
                    }
                    else if (stats.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                        displayGameWinFile();
                    }
                    else {
                        for (int i = 0; i < board_Size; i++) {
                            for (int j = 0; j < board_Size; j++) {

                                bead.setPosition(j * square_Size + 60, i * square_Size + 60);

                                // bead gets selected
                                if (bead.getGlobalBounds().contains(mousePosition.x, mousePosition.y) && (isCyansTurn && board[i][j] == 'C' || !isCyansTurn && board[i][j] == 'M')) {
                                    selectedBeadPosition = Vector2i(j, i);
                                    isBeadSelected = true;
                                    break;
                                }
                            }
                        }
                    }
                    
                }
            }
            else if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    // again taking positions
                    Vector2i mousePosition = Mouse::getPosition(window);

                    // Indices to be precise
                    int row = (mousePosition.y - 60) / square_Size;
                    int col = (mousePosition.x - 60) / square_Size;



                    // when sum of x and y indices is even only then the beads can move diagonally
                    if (row >= 0 && row < board_Size && col >= 0 && col < board_Size && board[row][col] == ' ' && abs(row - selectedBeadPosition.y) <= 2 && abs(col - selectedBeadPosition.x) <= 2 && (col + row) % 2 == 0) {

                        bool validMove = true;
                        if ((abs(col - selectedBeadPosition.x) == 2 || abs(col - selectedBeadPosition.x) == 0) && (abs(row - selectedBeadPosition.y) == 2 || abs(row - selectedBeadPosition.y) == 0)) {

                            // bead cancellation
                            if (board[(row + selectedBeadPosition.y) / 2][(col + selectedBeadPosition.x) / 2] == isCyansTurn ? 'M' : 'C') {
                                    board[(row + selectedBeadPosition.y) / 2][(col + selectedBeadPosition.x) / 2] = ' ';
                                    invalid.play();
                            }
                            else validMove = false;
                        }

                        if (validMove) {
                            board[row][col] = board[selectedBeadPosition.y][selectedBeadPosition.x];
                            board[selectedBeadPosition.y][selectedBeadPosition.x] = ' ';
                            isCyansTurn = !isCyansTurn;
                            sound_move.play();
                        }
                    }



                    // the beads will not move diagonally if sum of x and y indices is odd
                    else if (row >= 0 && row < board_Size && col >= 0 && col < board_Size && board[row][col] == ' ' && abs(row - selectedBeadPosition.y) <= 2 && abs(col - selectedBeadPosition.x) <= 2 && (row == selectedBeadPosition.y || col == selectedBeadPosition.x)) {

                        bool validMove = true;
                        if ((abs(col - selectedBeadPosition.x) == 2 || abs(col - selectedBeadPosition.x) == 0) && (abs(row - selectedBeadPosition.y) == 2 || abs(row - selectedBeadPosition.y) == 0)) {
                            // bead cancellation
                            if (board[(row + selectedBeadPosition.y) / 2][(col + selectedBeadPosition.x) / 2] == (isCyansTurn ? 'M' : 'C')) {
                                    board[(row + selectedBeadPosition.y) / 2][(col + selectedBeadPosition.x) / 2] = ' ';
                                    invalid.play();
                            }
                            else validMove = false;
                        }
                        if (validMove) {
                            board[row][col] = board[selectedBeadPosition.y][selectedBeadPosition.x];
                            board[selectedBeadPosition.y][selectedBeadPosition.x] = ' ';
                            isCyansTurn = !isCyansTurn;
                            sound_move.play();
                        }
                    }


                    isBeadSelected = false;
                }
            }

            // Changing the colour on Mouse Hover

            if (save.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                save.setFillColor(hoverColor);
            }
            else {
                save.setFillColor(originalColor);
            }

            if (load.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                load.setFillColor(hoverColor);
            }
            else {
                load.setFillColor(originalColor);
            }

            if (stats.getGlobalBounds().contains(mousePosition.x, mousePosition.y)) {
                stats.setFillColor(hoverColor);
            }
            else {
                stats.setFillColor(originalColor);
            }

        }

        // Moving Background
        backgroundPositionX += backgroundSpeed * elapsed.asSeconds();
        if (backgroundPositionX > texture.getSize().x) {
            backgroundPositionX -= texture.getSize().x;
        }

        if (restart) {
            int cyanCount = 0;
            int magentaCount = 0;
            for (int i = 0; i < board_Size; i++) {
                for (int j = 0; j < board_Size; j++) {
                    if (board[i][j] == 'C') {
                        cyanCount++;
                    }
                    else if (board[i][j] == 'M') {
                        magentaCount++;
                    }
                }
            }

            // Game Win History
            if (cyanCount == 0 || magentaCount == 0) {
                writeWinnerToFile(cyanCount, magentaCount);
                restartGame(board, isCyansTurn);
                restart = false; // Reset restart 
            }
        }

        if (!isMenuState) {
            // Game Win Check

            int cyanCount = 0;
            int magentaCount = 0;
            for (int i = 0; i < board_Size; i++) {
                for (int j = 0; j < board_Size; j++) {
                    if (board[i][j] == 'C') {
                        cyanCount++;
                    }
                    else if (board[i][j] == 'M') {
                        magentaCount++;
                    }
                }
            }

            window.clear();
            sprite.setPosition(-backgroundPositionX, 0.0f);
            window.draw(sprite);
            window.draw(linesprite);

            // Rendering of text according to the player's turn
            if (isCyansTurn) {
                window.draw(text2);
            }
            else if (!isCyansTurn) {
                window.draw(text3);
            }

            // Board Rendering
            for (int i = 0; i < board_Size; i++) {
                for (int j = 0; j < board_Size; j++) {
                    bead.setPosition(j * square_Size + 60, i * square_Size + 60);

                    if (board[i][j] == 'C') {
                        bead.setFillColor(Color::Cyan);
                    }
                    else if (board[i][j] == 'M') {
                        bead.setFillColor(Color::Magenta);
                    }
                    else {
                        bead.setFillColor(Color::Transparent);
                    }

                    // Selected bead is highlighted
                    if (isBeadSelected && (selectedBeadPosition.x == j && selectedBeadPosition.y == i)) {
                        bead.setFillColor(Color::White);
                    }

                    window.draw(bead);
                    window.draw(save);
                    window.draw(load);
                    window.draw(stats);
                    window.draw(title1);
                    window.draw(title2);
                }
            }

            // Ending Screen
            if (cyanCount == 0) {
                window.draw(exit2);
                victoryy.play();
            }
            else if (magentaCount == 0) {
                window.draw(exit);
                victoryy.play();
            }
            window.display();
        }

        else {
            // Display menu 
            window.clear();
            window.draw(menusprite);
            window.display();
        }
    }

    return 0;
}





// Initializing Board
void initializeBoard(char board[board_Size][board_Size], bool &isCyansTurn) {
    isCyansTurn = rand() % 2;   // 50% chance of being true

    for (int i = 0; i < board_Size; i++) {
        for (int j = 0; j < board_Size; j++) {
            if (i == 2 && j == 2) {
                board[i][j] = ' ';
            }
            else if (i < 2 || (i == 2 && j < 2)) {
                board[i][j] = 'C';
            }
            else {
                board[i][j] = 'M';
            }
        }
    }
}

// Re-Rendering the Board
void restartGame(char board[board_Size][board_Size], bool &isCyansTurn) {
    initializeBoard(board, isCyansTurn);
}

// Writing into the file
void writeWinnerToFile(int cyanCount, int magentaCount) {
    ofstream file("gamewin.txt", ios::app);
    if (file.is_open()) {
        if (cyanCount == 0) {
            file << "Magneta wins" << endl;
        }
        else if (magentaCount == 0) {
            file << "Cyan wins" << endl;
        }
        file.close();
    }
    else {
        cout << "Unable to open the file 'gamewin.txt' for writing" << endl;
    }
}

// Showing the Content of the File
void displayGameWinFile() {
    ifstream file("gamewin.txt");
    if (file.is_open()) {
        const int Size = 1000;
        char line[Size];

        while (file.getline(line, Size)) {
            cout << line << endl;
        }

        file.close();
    }
    else {
        cout << "Unable to open the file 'gamewin.txt' for reading" << endl;
    }
}

// Save Game State
void saveGameState(const char board[board_Size][board_Size], bool isCyansTurn) {
    ofstream file("gamesave.txt");
    if (file.is_open()) {
        // Save board state
        for (int i = 0; i < board_Size; i++) {
            for (int j = 0; j < board_Size; j++) {
                file << board[i][j];
            }
            file << endl;
        }
        // Save turn
        file << isCyansTurn << endl;

        file.close();
        cout << "Game saved successfully" << endl;
    }
    else {
        cout << "Unable to open the file 'gamesave.txt' for writing" << endl;
    }
}

// Load Game State
void loadGameState(char board[board_Size][board_Size], bool& isCyansTurn) {
    ifstream file("gamesave.txt");
    if (file.is_open()) {
        // Load board state
        for (int i = 0; i < board_Size; i++) {
            char line[board_Size + 1]; // +1 for null terminator
            file.getline(line, board_Size + 1);
            for (int j = 0; j < board_Size; j++) {
                board[i][j] = line[j];
            }
        }
        // Load turn
        char turn;
        file.get(turn);
        isCyansTurn = (turn == '1');

        file.close();
        cout << "Game loaded successfully" << endl;
    }
    else {
        cout << "Unable to open the file 'gamesave.txt' for reading" << endl;
    }
}





