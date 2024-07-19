# Ping-Pong Game in C

This project is a simple Ping-Pong game implemented in C, running in the terminal. The game features basic paddle and ball mechanics, where the player can control the paddle to bounce the ball back. The opponent paddle moves automatically.

## Features

- Real-time paddle movement using keyboard input.
- Ball movement and collision detection with paddles and walls.
- Increasing ball speed over time.
- Pause and resume functionality.

## Planned Improvements

- **Improved Collisions**: Enhance collision detection for more realistic ball rebounds.
- **Randomized Rebounds**: Add randomness to ball bounces for unpredictable gameplay.
- **Speed and Game Tick Adjustments**: Fine-tune ball speed and game tick intervals for smoother gameplay.
- **Score System**: Implement a scoring system to keep track of points.

## How to Compile and Run

1. **Compile the code:**
   ```bash
   gcc -o ping_pong ping_pong.c && ./ping_pong
