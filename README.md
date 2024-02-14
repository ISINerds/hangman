# Hangman Game in C using Raylib and Raygui

Welcome to the Hangman game! This classic word-guessing game is built in C using the Raylib and Raygui libraries. The game offers two modes: Single Player and Two Player, each with customizable difficulty levels based on word length and letters rarity.
The game uses a binary tree to efficiently search for words and manage the game logic.


## Prerequisites

To run the game, ensure you have a Linux environment (Linux, WSL, etc.) and follow these steps:

1. Run `make install` to install all dependencies.
2. Run `make run` to start the game.
3. Optionally, you can run unit tests using `make run_test`.

## Game Modes

### Single Player Mode

In Single Player mode, you will play the classic Hangman game. Try to guess the hidden word letter by letter. The difficulty level can be adjusted based on word length and letter rarity.

If you win, you can submit your username and see your ranking on the leaderboard. Rankings are determined by the word difficulty and the number of attempts.

### Two Player Mode

In Two Player mode, two players take turns trying to guess the same hidden word. Each player plays on the same keyboard. When a player correctly guesses a letter, they continue their turn. If a player guesses a wrong letter, it becomes the other player's turn. The game continues in this alternating fashion until either one player correctly guesses the entire word, someone wins, or both players lose if the hangman is completely drawn.


## Difficulty Levels

The difficulty levels are determined by the length of the word and the rarity of its letters. There are common, rare, and other variations to make the game more challenging.

## Leaderboard

After winning in Single Player mode, players can submit their usernames and view the leaderboard. The leaderboard displays usernames, scores, and ranks based on word difficulty and attempts.

## How to Play

- Use the keyboard to input your guesses.
- Guess the word letter by letter.
- In Two Player mode, take turns choosing and guessing words.

## Additional Features

- Run `make run_test` to execute unit tests for functions like delete word, add word, and display all the words of the dictionary or visualize the binary tree.

## Contributing

Feel free to contribute by adding new features, improving code, or fixing bugs. Create a pull request, and we'll review it together.

Have fun playing Hangman!
