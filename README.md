# Pong Game in C

This is a simple terminal-based Pong game written in C. The game features a player paddle, an opponent paddle controlled by the computer, and a ball that bounces around the screen. The objective is to prevent the ball from going out of bounds on your side.

## Features

- **Interactive Gameplay**: Control the paddle using the keyboard arrow keys.
- **Computer Opponent**: The computer controls the opponent paddle.
- **Dynamic Ball Speed**: The speed of the ball increases over time.
- **Pause and Resume**: The game can be paused and resumed.
- **Game Over Detection**: The game ends when the ball goes out of bounds.

## How to Compile

To compile this program, you need a C compiler such as `gcc`. Follow the steps below:

1. Open your terminal.
2. Navigate to the directory containing the source code file `pong.c`.
3. Compile the code using `gcc`:

   ```bash
   gcc pong.c -o pong
   ```

4. Run the compiled program:

   ```bash
   ./pong
   ```

## Controls

- **Arrow Up**: Move the paddle up.
- **Arrow Down**: Move the paddle down.
- **p** : Pause/Resume the game.
- **x** : Exit the game.

## Code Overview

### Variables

- `GAME_TICK`: Speed of game tick, lower value increases speed.
- `SCREEN_WIDTH`: Width of the game screen.
- `SCREEN_HEIGHT`: Height of the game screen.
- `PADDLE_SIZE`: Size of the paddle.
- `INITIAL_BALL_TICK`: Initial speed of the ball.
- `MIN_BALL_TICK`: Minimum speed of the ball.
- `SPEED_INCREASE_INTERVAL`: Time interval for increasing the ball speed.

### Functions

- `disableRawMode()`: Restores the terminal to its original state.
- `enableRawMode()`: Enables raw mode for the terminal to capture input without buffering.
- `kbhit()`: Checks if a key has been pressed.
- `drawScreen(int y, int x, int paddle_pos, int opponent_pos, int ball_x, int ball_y)`: Draws the game screen.
- `clear()`: Clears the terminal screen.

### Game Loop

The game loop handles the following:
- Checking for keyboard input to move the paddle, pause/resume the game, or exit.
- Updating the ball's position based on its direction and speed.
- Detecting collisions with the paddles and borders.
- Moving the opponent's paddle based on the ball's position.
- Gradually increasing the ball's speed at set intervals.

