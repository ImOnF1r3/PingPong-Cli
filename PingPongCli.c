#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define GAME_TICK 20000                      // Speed of game tick, set lower to gain speed
#define SCREEN_WIDTH 41                      // Size battle field
#define SCREEN_HEIGHT (SCREEN_WIDTH / 2)     //
#define PADDLE_SIZE 3                        // Size of the paddle
#define INITIAL_BALL_TICK 25                 // Start Speed
#define MIN_BALL_TICK 1                      // Final Speed, don't set below the zero
#define SPEED_INCREASE_INTERVAL 5            // Increase speed every 5 seconds

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

void drawScreen(int y, int x, int paddle_pos, int opponent_pos, int ball_x, int ball_y) {
    printf("\033[%d;%dH", y, x);
    printf("▀ "); for (int i = 0; i < SCREEN_WIDTH; i++) printf("=");
    printf(" ▀\n");
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        printf("%*s║", x, "");
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            if ((j == 1 && i >= paddle_pos && i < paddle_pos + PADDLE_SIZE) || (j == SCREEN_WIDTH - 2 && i >= opponent_pos && i < opponent_pos + PADDLE_SIZE)) printf("|");
            else if (i == ball_y && j == ball_x) printf("O");
            else printf(" ");
        }
        printf("║\n");
    }
    printf("%*s▄ ", x - 1, ""); for (int i = 0; i < SCREEN_WIDTH; i++) printf("=");
    printf(" ▄\n\n\n");
    fflush(stdout);
}

void clear() {
    printf("\033[H\033[J");
    fflush(stdout);
}

int main() {
    srand(time(NULL)); // Initialize the random number generator
    struct winsize w;
    int norRow, norCol;
    int paddle_pos = SCREEN_HEIGHT / 2;
    int opponent_pos = SCREEN_HEIGHT / 2;
    int ball_x = SCREEN_WIDTH / 2, ball_y = SCREEN_HEIGHT / 2;
    int ball_dir_x = -1, ball_dir_y = 1;
    int paused = 0;
    int ball_tick = INITIAL_BALL_TICK;
    int ball_tick_counter = 0;
    time_t last_speed_increase_time = time(NULL);

    enableRawMode();

    while (1) {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        if (!paused) {
            if (ball_tick_counter >= ball_tick) {
                ball_tick_counter = 0;
                ball_x += ball_dir_x;
                ball_y += ball_dir_y;

                // Ball collision with paddles
                if (ball_x == 1 && ball_y >= paddle_pos && ball_y < paddle_pos + PADDLE_SIZE) {
                    ball_dir_x = -ball_dir_x;
                    ball_dir_y = (rand() % 3) - 1; // -1, 0, or 1
                }
                if (ball_x == SCREEN_WIDTH - 2 && ball_y >= opponent_pos && ball_y < opponent_pos + PADDLE_SIZE) {
                    ball_dir_x = -ball_dir_x;
                    ball_dir_y = (rand() % 3) - 1; // -1, 0, or 1
                }

                // Ball collision with borders
                if (ball_y == 0 || ball_y == SCREEN_HEIGHT - 1) ball_dir_y = -ball_dir_y;
                if (ball_x < 0 || ball_x > SCREEN_WIDTH - 1) goto end_game; // Ball went out of bounds, end the game

                // Move opponent paddle
                if (opponent_pos + PADDLE_SIZE / 2 < ball_y) opponent_pos++;
                else if (opponent_pos + PADDLE_SIZE / 2 > ball_y) opponent_pos--;
                if (opponent_pos < 0) opponent_pos = 0;
                if (opponent_pos > SCREEN_HEIGHT - PADDLE_SIZE) opponent_pos = SCREEN_HEIGHT - PADDLE_SIZE;
            } else {
                ball_tick_counter++;
            }

            // Gradually increase ball speed
            if (time(NULL) - last_speed_increase_time >= SPEED_INCREASE_INTERVAL) {
                if (ball_tick > MIN_BALL_TICK) {
                    ball_tick--; // Decrease ball tick to increase speed
                }
                last_speed_increase_time = time(NULL);
            }
        }

        clear();
        norRow = (w.ws_row / 2) - SCREEN_HEIGHT / 2;
        norRow = norRow - 2;
        norCol = (w.ws_col / 2) - SCREEN_WIDTH / 2;
        drawScreen(norRow, norCol, paddle_pos, opponent_pos, ball_x, ball_y);

        printf("                    Ball-pos: (%d, %d), Opp-pos: %d, Ball-tick: %d\n", ball_x, ball_y, opponent_pos, ball_tick);
        printf("                    Press [x] to exit, [p] to pause\n");

        if (kbhit()) {
            char c = getchar();
            if (c == '\033') {
                getchar();
                char arrow_key = getchar();
                if (arrow_key == 'A' && paddle_pos > 0) paddle_pos--;
                else if (arrow_key == 'B' && paddle_pos < SCREEN_HEIGHT - PADDLE_SIZE) paddle_pos++;
            } else if (c == 'x') break;          // x - e(X)it
            else if (c == 'p') paused = !paused; // p - (P)ause
        }

        usleep(GAME_TICK);
    }

end_game:
    disableRawMode();
    return 0;
}
