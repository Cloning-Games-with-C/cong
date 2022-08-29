#include "stdio.h"
#include "stdbool.h"

#include "raylib.h"

#define WIDTH 640
#define HEIGHT 480

typedef struct
{
    int x;
    int y;
} Point;

Point ball = {.x = WIDTH / 2, .y = HEIGHT / 2};
int ball_dx;
int ball_dy;
bool start;

int left_paddle_y = HEIGHT / 2;
int right_paddle_y = HEIGHT / 2;

int left_score;
int right_score;

void update_ball()
{
    ball_dx = GetRandomValue(0, 1) == 0 ? -1 : 1;
    ball_dy = GetRandomValue(0, 1) == 0 ? -1 : 1;
}

bool rect_collision(int x, int y, int w, int h, int x2, int y2, int w2, int h2)
{
    return (x < x2 + w2 && x > x2 && y < y2 + h2 && y > y2) ||
           (x + w < x2 + w2 && x + w > x2 && y + h < y2 + h2 && y + h > y2);
}

void check_collision()
{
    if (ball.y <= 0 || ball.y >= HEIGHT - 10)
    {
        ball_dy = -ball_dy;
    }

    if (rect_collision(ball.x, ball.y, 10, 10, 20, left_paddle_y, 10, 80) || rect_collision(ball.x, ball.y, 10, 10, WIDTH - 30, right_paddle_y, 10, 80))
    {
        ball_dx = -ball_dx;
    }

    if (ball.x < 0 || ball.x >= WIDTH - 10)
    {
        if (ball.x < 0)
        {
            left_score++;
        }
        else if (ball.x >= WIDTH - 10)
        {
            right_score++;
        }

        ball.x = WIDTH / 2;
        ball.y = HEIGHT / 2;
        ball_dx = 0;
        ball_dy = 0;
        start = false;
    }
}

void paddle_movement()
{
    if (IsKeyDown(KEY_W))
    {
        left_paddle_y -= 2;
        if (left_paddle_y < 0)
        {
            left_paddle_y = 0;
        }
    }
    else if (IsKeyDown(KEY_S))
    {
        left_paddle_y += 2;
        if (left_paddle_y >= HEIGHT - 80)
        {
            left_paddle_y = HEIGHT - 80;
        }
    }
    if (IsKeyDown(KEY_UP))
    {
        right_paddle_y -= 2;
        if (right_paddle_y < 0)
        {
            right_paddle_y = 0;
        }
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        right_paddle_y += 2;
        if (right_paddle_y >= HEIGHT - 80)
        {
            right_paddle_y = HEIGHT - 80;
        }
    }
}

int main(void)
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(WIDTH, HEIGHT, "cong");
    SetTargetFPS(120);

    while (!WindowShouldClose())
    {

        if(IsKeyReleased(KEY_SPACE))
        {
            update_ball();
            start = true;
        }

        paddle_movement();

        ball.x += ball_dx;
        ball.y += ball_dy;

        check_collision();

        BeginDrawing();
        ClearBackground(BLACK);

        for(int y = 0; y < HEIGHT;)
        {
            DrawRectangle(WIDTH / 2 - 5, y, 10, 30, GRAY);
            y += 35;
        }

        DrawRectangle(ball.x, ball.y, 10, 10, WHITE);
        DrawRectangle(20, left_paddle_y, 10, 80, WHITE);
        DrawRectangle(WIDTH - 30, right_paddle_y, 10, 80, WHITE);

        char score_text[12];
        snprintf(score_text, 12, "%02d\t\t%02d", left_score, right_score);
        DrawText(score_text, (WIDTH / 2) - (MeasureText(score_text, 22) / 2), 30, 22, WHITE);

        EndDrawing();
    }

    CloseWindow();
}