#include <raylib.h>
#include <time.h>
#include <stdio.h>
#define FPS 60 							// target fps
#define WINDOW_WIDTH 600 		// 600 pixels wide
#define WINDOW_HEIGHT 600   // 600 pixels tall

typedef struct {
	Vector2 pos;
	Vector2 dir;
	int radius;
} Ball;

Rectangle left_paddle, right_paddle;
Ball ball;
int scores[2];

void move_paddle_dy(Rectangle* paddle_p, float dy) {
	paddle_p->y += dy;

	if (paddle_p->y < 0) paddle_p->y = 0;
	if (paddle_p->y + paddle_p->height > WINDOW_HEIGHT) paddle_p->y = WINDOW_HEIGHT - paddle_p->height;
}

void launch_ball(Ball* ball) {
	float dirX = GetRandomValue(0, 100);
	float dirY = GetRandomValue(0, 100);
	ball->pos = (Vector2){.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};
	ball->dir = (Vector2){.x = dirX / 100, .y = dirY / 100};
}

void reset_ball(Ball* ball) {
	ball->pos = (Vector2){.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};
	ball->dir = (Vector2){0};
}

bool ball_score(Ball ball, int* who_out) {
	if (ball.pos.x + ball.radius > WINDOW_WIDTH) {
		*who_out = 0;
		return true;
	}
	if (ball.pos.x - ball.radius < 0) {
		*who_out = 1;
		return true;
	}
	return false;
}

bool ball_should_edge_bounce(Ball ball) {
	return ball.pos.y - ball.radius <= 0 || ball.pos.y + ball.radius >= WINDOW_HEIGHT;
}

#define max(a, b) (a) > (b) ? (a) : (b)
#define min(a, b) (a) < (b) ? (a) : (b)
bool ball_should_paddle_bounce(Ball ball, Rectangle paddle) {
	// Closest point on the AABB to the circle center
	float closestX = max(paddle.x, min(ball.pos.x, paddle.x + paddle.width));
	float closestY = max(paddle.y, min(ball.pos.y, paddle.y + paddle.height));

	// Distance squared between the circle center and the closest point
	float dx = ball.pos.x - closestX;
	float dy = ball.pos.y - closestY;
	float distanceSquared = dx * dx + dy * dy;

	// Intersection check
	return distanceSquared <= ball.radius * ball.radius;
}

int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong Game");
	SetRandomSeed(time(0));
	left_paddle = (Rectangle) {.x = 10, .y = 50, .width = 20, .height = 100};
	right_paddle = (Rectangle) {.x = WINDOW_WIDTH - 10 - 20, .y = 50, .width = 20, .height = 100};
	ball = (Ball) {.pos.x = WINDOW_WIDTH / 2.f, .pos.y = WINDOW_HEIGHT / 2.f, .radius = 10};
	scores[0] = 0;
	scores[1] = 0;

	while (!WindowShouldClose()) {
		// Update Logic
		if (IsKeyDown(KEY_W))    move_paddle_dy(&left_paddle, -200 * GetFrameTime());
		if (IsKeyDown(KEY_S))    move_paddle_dy(&left_paddle, 200 * GetFrameTime());
		if (IsKeyDown(KEY_UP))   move_paddle_dy(&right_paddle, -200 * GetFrameTime());
		if (IsKeyDown(KEY_DOWN)) move_paddle_dy(&right_paddle, 200 * GetFrameTime());

		if (IsKeyPressed(KEY_SPACE)) launch_ball(&ball);
		ball.pos.x += ball.dir.x * GetFrameTime() * 200;
		ball.pos.y += ball.dir.y * GetFrameTime() * 200;

		int who = -1;
		if (ball_score(ball, &who)) {
			scores[who]++;
			reset_ball(&ball);
		}

		if (ball_should_edge_bounce(ball)) {
			ball.dir.y *= -1;
		}

		if (ball_should_paddle_bounce(ball, left_paddle) || ball_should_paddle_bounce(ball, right_paddle)) {
			ball.dir.x *= -1;
		}

		// Game Over logic

		// Rendering code
		BeginDrawing();
			DrawFPS(WINDOW_WIDTH - 100, 10);
			DrawRectangle(left_paddle.x, left_paddle.y, left_paddle.width, left_paddle.height, BLUE);
			DrawRectangle(right_paddle.x, right_paddle.y, right_paddle.width, right_paddle.height, BLUE);
			DrawCircle(ball.pos.x, ball.pos.y, ball.radius, RED);
			DrawText(TextFormat("%d", scores[0]) , 200, 10, 20, RED);
			DrawText(TextFormat("%d", scores[1]), 400, 10, 20, RED);
		EndDrawing();
		ClearBackground(GRAY);
	}

	CloseWindow();

}
