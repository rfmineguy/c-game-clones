#include <raylib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include "vec2_ll.h"
#define FPS 60 							// target fps
#define WINDOW_WIDTH 600 		// 600 pixels wide
#define WINDOW_HEIGHT 600   // 600 pixels tall
#define GRID_DIM 20    			// 20x20 cells
#define CELL_WIDTH (WINDOW_WIDTH / GRID_DIM)
#define CELL_HEIGHT (WINDOW_HEIGHT / GRID_DIM)

void render_grid() {
	for (int i = 0; i < GRID_DIM; i++) {
		for (int j = 0; j < GRID_DIM; j++) {
			DrawRectangle(i * CELL_WIDTH, j * CELL_HEIGHT, CELL_WIDTH - 1, CELL_HEIGHT - 1, DARKGRAY);
		}
	}
}

void render_food(Vector2 food_pos) {
	DrawRectangle(food_pos.x * CELL_WIDTH, food_pos.y * CELL_WIDTH, CELL_WIDTH - 1, CELL_HEIGHT - 1, RED);
}

void render_snake(ll_Vector2 ll) {
	ll_Vector2_node* n = ll.head;
	while (n) {
		DrawRectangle(n->val.x * CELL_WIDTH, n->val.y * CELL_HEIGHT, CELL_WIDTH - 1, CELL_HEIGHT - 1, GREEN);
		n = n->next;
	}
}

#define VecAdd(a, b) (Vector2){.x = a.x + b.x, .y = a.y + b.y}
void move_snake(ll_Vector2* snake_list, Vector2 snake_dir) {
	assert(snake_list->head != NULL && "Head was null");
	Vector2 head = snake_list->head->val;
	Vector2 newpos = VecAdd(head, snake_dir);
	if (newpos.x < 0) newpos.x = GRID_DIM - 1;
	if (newpos.x > GRID_DIM) newpos.x = 0;
	if (newpos.y < 0) newpos.y = GRID_DIM - 1;
	if (newpos.y > GRID_DIM) newpos.y = 0;
	ll_Vector2_pushfront(snake_list, newpos);
	ll_Vector2_popback(snake_list);
}

bool snake_collide_food(ll_Vector2 snake_list, Vector2 food) {
	ll_Vector2_node* head = snake_list.head;
	assert(head != NULL && "Head was null");
	return head->val.x == food.x && head->val.y == food.y;
}

bool snake_collide_self(ll_Vector2 snake_list) {
	ll_Vector2_node* head = snake_list.head;
	assert(head != NULL && "Head was null");
	ll_Vector2_node* n = head->next;
	while (n) {
		if (n->val.x == head->val.x && n->val.y == head->val.y) return true;
		n = n->next;
	}
	return false;
}

void grow_snake(ll_Vector2* snake_list, Vector2 snake_dir) {
	assert(snake_list->head != NULL && "Head was null");
	Vector2 head = snake_list->head->val;
	ll_Vector2_pushfront(snake_list, VecAdd(head, snake_dir));
}

Vector2 getRandomFoodPos() {
	return (Vector2) {
		.x = GetRandomValue(0, GRID_DIM - 1),
		.y = GetRandomValue(0, GRID_DIM - 1),
	};
}

int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Snake Game");
	// SetTargetFPS(FPS);
	SetRandomSeed(time(0));
	Vector2 food_pos = getRandomFoodPos();
	Vector2 snake_dir = {0};
	ll_Vector2 snake_list = {0};
	bool game_over = false;

	ll_Vector2_pushfront(&snake_list, (Vector2){0, 0});

	float moveTimer = 0;
	while (!WindowShouldClose()) {
		// Game Over logic
		if (game_over) {
			if (IsKeyPressed(KEY_R)) {
				game_over = false;
				ll_Vector2_free(&snake_list);
				ll_Vector2_pushfront(&snake_list, (Vector2){0, 0});
				food_pos = getRandomFoodPos();
			}
		}
		else {
			moveTimer += GetFrameTime() * 10;

			// Update logic
			if (IsKeyPressed(KEY_F))	   food_pos = getRandomFoodPos();
			if (IsKeyPressed(KEY_UP))    { snake_dir.x = 0; snake_dir.y = -1; }
			if (IsKeyPressed(KEY_DOWN))  { snake_dir.x = 0; snake_dir.y =  1; }
			if (IsKeyPressed(KEY_RIGHT)) { snake_dir.y = 0; snake_dir.x =  1; }
			if (IsKeyPressed(KEY_LEFT))  { snake_dir.y = 0; snake_dir.x = -1; }

			if (moveTimer > 1) {
				move_snake(&snake_list, snake_dir);
				moveTimer = 0;
			}

			if (snake_collide_food(snake_list, food_pos)) {
				grow_snake(&snake_list, snake_dir);
				food_pos = getRandomFoodPos();
			}

			if (snake_collide_self(snake_list)) {
				game_over = true;
			}
		}

		// Rendering code
		BeginDrawing();
			render_grid();
			render_food(food_pos);
			render_snake(snake_list);
			if (game_over) {
				DrawText("Game Over", 200, 300, 40, WHITE);
				DrawText("Restart (R)", 200, 340, 40, WHITE);
			}
			DrawFPS(WINDOW_WIDTH - 100, 10);
		EndDrawing();
		ClearBackground(GRAY);
	}

	CloseWindow();
}
