#include <SFML/Graphics.hpp>
#include <windows.h>
#include <SFML/Window.hpp>
#include <optional>
#include <SFML/System.hpp>
#include <set>
#include <limits>
#include <iostream>


// Finds all the empty spaces in the tic-tac-toe grid
std::set<int> all_spaces(int (&grid)[3][3]) {
    std::set<int> return_set;
    for (int i = 0; i < 9; i++) {
        if (grid[i / 3][i % 3] == 0) {
            return_set.insert(i);
        }
    }
    return return_set;
}

// Checks if and who has won the game
int isAWin(int grid[3][3]) {
    // Checks for any horizontal or vertical wins
    for (int i = 0; i < 3; i++) {
        if  ((grid[i][0] != 0 && grid[i][0] == grid[i][1] && grid[i][0] == grid[i][2])
            || (grid[0][i] != 0 && grid[0][i] == grid[1][i] && grid[0][i] == grid[2][i])) {
                return grid[i][i];
            }
    }
    // Checks for any diagonal wins
    if ((grid[0][0] != 0 && grid[0][0] == grid[1][1] && grid[0][0] == grid[2][2]) 
        || (grid[0][2] != 0 && grid[0][2] == grid[1][1] && grid[0][2] == grid[2][0])) {
            return grid[1][1];
    }
    // Returns 0 if there is no win
    return 0;
}

// This algorithm searches through possibles boards and returns corresponding scores
// Utilizes alpha-beta pruning make the algorithm extremely efficient
int negamax(int (&grid)[3][3], int player, std::set<int>& spaces, int alpha, int beta) {

    // Checks for leaf nodes (wins or cat's games)
    int winner = isAWin(grid);
    if (winner != 0) {
        return (winner == player) ? 1000*(spaces.size() + 1) : -1000*(spaces.size() + 1);
    } else if (spaces.empty()) {
        return 0;
    }

    // Max value initializes at a min
    int maxValue = std::numeric_limits<int>::min();
    // Copies the available moves
    std::vector<int> availableMoves(spaces.begin(), spaces.end());
    // Loops through all moves for consideration
    for (int move : availableMoves) {
        grid[move / 3][move % 3] = player;
        spaces.erase(move);
        int score = -negamax(grid, -player, spaces, -beta, -alpha);
        grid[move / 3][move % 3] = 0;
        spaces.insert(move);

        // Alpha-beta pruning makes sure that no nodes in the  
        // decision tree are considered when they don't have to be
        maxValue = std::max(maxValue, score);
        alpha = std::max(alpha, score);
        if (alpha >= beta) {
            break;
        }
    }
    // Returns highest possible score given a perfectly played game
    return maxValue;
}

// Finds the most optimal move for the current player using the negamax algorithm above
int findMove(int (&old_grid)[3][3], int player) {

    // Clones the current grid
    int grid[3][3];
    for (int i = 0; i < 9; i++) {
        grid[i / 3][i % 3] = old_grid[i / 3][i % 3];
    }

    std::set<int> spaces = all_spaces(grid);
    std::set<int> moves(spaces.begin(), spaces.end());

    int highScore = -10000;
    int best = -1;

    for (int move : moves) {
        grid[move / 3][move % 3] = player;
        spaces.erase(move);

        int score = -negamax(grid, -player, spaces, -10000, 10000);

        grid[move / 3][move % 3] = 0;
        spaces.insert(move);

        if (score > highScore) {
            highScore = score;
            best = move;
        }
    }

    return best;
}

// Draws everything with SFML 3.0.0
void renderGrid(sf::RenderWindow& window, int grid[3][3]) {
    sf::RectangleShape rectangle({300.f, 300.f});
    sf::CircleShape circle(105.f);
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            rectangle.setPosition(sf::Vector2f(x * 300, y * 300));
            circle.setPosition(sf::Vector2f(x * 300 + 45, y * 300 + 45));
            rectangle.setFillColor(sf::Color::Black);
            window.draw(rectangle);
            if (grid[y][x] == 1) {
                sf::RectangleShape line(sf::Vector2f(300, 36));
                line.setOrigin(150, 18);
                line.setPosition(x * 300 + 150, y * 300 + 150);
                line.setRotation(45);
                line.setFillColor(sf::Color::Red);
                window.draw(line);
                line.setRotation(-45);
                window.draw(line);
            } else if (grid[y][x] == -1) {
                circle.setFillColor(sf::Color::Black);
                circle.setOutlineColor(sf::Color::Blue);
                circle.setOutlineThickness(25);
                window.draw(circle);
            }
        }
    }
    sf::RectangleShape lineh({900.f, 10.f});
    sf::RectangleShape linev({10.f, 900.f});
    for (float i : {295.f, 595.f}) {
        lineh.setPosition(0.f, i);
        linev.setPosition(i, 0.f);
        window.draw(lineh);
        window.draw(linev);
    }
}

// Handles all events that occur using SFML
// These events are window closing, clicking on a specific space, or the pressing of the 'p' key
void eventHandler(sf::RenderWindow& window, int (&grid)[3][3], int& player, int& turn) {
    sf::Event event;
    while (window.pollEvent(event)) {
        // Window closing
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        // Makes a move wherever the user clicks
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / 300;
                int y = event.mouseButton.y / 300;
                if (grid[y][x] == 0) {
                    grid[y][x] = player;
                    player *= -1;
                    turn++;
                }
            }
        }
        // Makes a move using the AI upon the pressing of the 'p' key
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::P) {
                int aiMove = 4;
                if (!isAWin(grid) && turn < 9 && turn != 0) {
                    aiMove = findMove(grid, player);
                }
                grid[aiMove / 3][aiMove % 3] = player;
                player *= -1;
                turn++;
            }
        }
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode({900, 900}), "Tic Tac Toe");

    // Creates the grid to be used throughout the game
    int grid[3][3];
    for (int i = 0; i < 9; i++) {
        grid[i / 3][i % 3] = 0;
    }
    // The player integer is either 1 or -1 and represents who is playing
    int player = 1;
    // The turn variable keeps track of what turn it is
    // This is used within the negamax function to weight certain outcomes
    int turn = 0;

    while (window.isOpen()) {
        // Handles any events
        eventHandler(window, grid, player, turn);
        // Handles the window
        window.clear();
        renderGrid(window, grid);
        window.display();
        // Checks for any end of game status
        int winner = isAWin(grid);
        if (winner != 0 || turn == 9) {
            if (winner == 1) {
                std::cout << "Player X wins!" << std::endl;
            } else if (winner == -1) {
                std::cout << "Player O wins!" << std::endl;
            } else {
                std::cout << "Cat's game!" << std::endl;
            }
            Sleep(1000);
            window.close();
        }

    }
    
    return 0;
}
