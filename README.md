# Tic-Tac-Toe Solver

Tic-tac-toe is a game in which players alternate placing pieces in a 3 by 3 grid. If a player gets three marks in a row horizontally, vertically, or diagonally, they win.

The files in this repository utilize the SFML 3.0.0 library to display a tic-tac-toe game. The users can play normally by playing alternating turns or they can use the artificial intelligence to make the most optimal move.

<p align="center">
  <img src="https://github.com/user-attachments/assets/eed2e11b-1677-4e30-b0e1-4ce10126f75d" width="500" />
</p>

## The Tic-Tac-Toe Game

The Tic-tic-toe game is creating by starting with an empty grid. The grid is displayed using the `renderGrid` function. This function directly uses the SFML library circles and rectangles to display a tic-tac-toe board for the user. The different players' pieces are denoted by a red note and a blue circle for the first player and second player respectively.

## Interaction

The players can interact with the board in two ways: clicking on the spot they wish to claim, or pressing the 'p' key in order to play the most optimal move using an AI solve.

Mouse clicking is listened for thanks to the `eventHandler` function. This function uses the SFML `Event` class to listen for any events from the user. 

A player's interaction only occurs upon a click event. Once a click event is found, the mouse position is checked in order to place the player's piece wherever they click. If the place where the player clicked is available, their corresponding piece is placed and the next turn is initiated.

An AI solution only occurs upon a 'p' key press event. Once a 'p' key press is found, the program uses the `findMove` function to find the optimal move assuming a perfect game. Then, the piece corresponding to the current player is placed in that optimal spot.

## The Negamax Algorithm

This program uses the negamax variant of the minmax algorithm to find the most optimal move.

### What actually is negamax?

Negamax is a search algorithm used in two-player, zero-sum games (such as chess, tic-tac-toe, and checkers) to determine the optimal move for a player. It's essentially a simplified version of the Minimax algorithm, where both players are assumed to play optimally and the goal is to minimize the opponent's possible outcome while maximizing your own. In most cases, including in this program, this is done by using depth-first-search through a decision tree.

### The Implementation

Optimal moves are chosen through the `findMove` function. This function initializes a negamax procedure and then uses the `negamax` function to search through the decisions tree. Upon finding a leaf node, the negamax function returns 0 upon a cat's game (no one wins) result, and returns a weighted result based on if the winner of that outcome corresponds to the current player. The sooner the position, the stronger the weight.

Both the `findMove` and `negamax` functions also include alpha-beta pruning. Alpha-beta pruning can be described in-depth [here](https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning/), but is a way to avoid considering many nodes in the decision tree in order to make the functions far more efficient.
