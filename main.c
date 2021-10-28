#include "raylib.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const int screenWidth = 450;
const int screenHeight = 450;

/*
For the sake of managment the blocks wil
*/
Camera camera = { 0 };

Vector3 cube_positions[9];
BoundingBox cube_bounds[9];
#define NO_ACTION 0x00
#define MENU 0x10
#define HARVEST 0x20
#define SAVE 0x30
#define SELL 0x40

#define RNG_LEN 2
/*
Just write some big complex function to do some RNG code.
*/
uint8_t rng_seed[RNG_LEN] = {0, 0};

int rng_output;

//a function to (sloppily) generate a 32-bit number.
uint32_t badRNGFunction()
{
	//counts how many frames of RNG processing there have been.
	static uint32_t rng_frames = 0;
	if(rng_frames > 360){rng_frames = 0;}
	//increments it.
	rng_frames++;
	/*The resulting RNG value.
	.
	sqrt(rng_frames << 3) * (sin(rng_frames)) * 100
	.
	*/
	int rng_output = sqrt(rng_frames << 3) * (sin(rng_frames)) * 100;
	return rng_output;
}
/*
here is a chart of the maximum values that you can get base on the shift.
0 = 31  | 2 = 1
0 = 30  | 2 = 3
0 = 29  | 2 = 7
0 = 28  | 2 = 15
0 = 27  | 2 = 31
0 = 26  | 2 = 63
0 = 25  | 2 = 127
0 = 24  | 2 = 255
0 = 23  | 2 = 511
0 = 22  | 2 = 1021
0 = 21  | 2 = 2045
0 = 20  | 2 = 4094
0 = 19  | 2 = 8181
0 = 18  | 2 = 16377
0 = 17  | 2 = 32763
0 = 16  | 2 = 65485
0 = 15  | 2 = 131017
0 = 14  | 2 = 262086
0 = 13  | 2 = 524223
0 = 12  | 2 = 1048507
0 = 11  | 2 = 2097076
0 = 10  | 2 = 4194224
0 = 9   | 2 = 8388524
0 = 8   | 2 = 16777124
0 = 7   | 2 = 33554336
0 = 6   | 2 = 67108760
0 = 5   | 2 = 134217620
0 = 4   | 2 = 268435344
0 = 3   | 2 = 536870791
0 = 2   | 2 = 1073741699
0 = 1   | 2 = 2147483514
*/
uint32_t badRNGFunctionShifted(int shift_amount)
{
	return (badRNGFunction() << shift_amount) >> shift_amount;
}

uint32_t getHighestNumberForBitWidth(int shift_amount)
{
	uint32_t MAX = ~(0);
	return (MAX << shift_amount) >> shift_amount;
}

int crop_luck = 0;
int farmEXP = 0;
int farmLVL = 0;
int exp_to_next_farm_level = 10;

int money;

#define BLNK 0x00
#define DIRT 0x10
#define WATER 0x20
#define SEEDED 0x30

typedef struct FarmCube
{
	/*
	First byte is state, second state is type.
	*/
	uint8_t data;
	uint8_t type;
	uint8_t limiter;
}
FarmCube;

//for floating point RNG
float JProduceRandomValue(float min, float max)
{
	float A;
	float B;
	float AB;

	if ((int)max == 0)
	{
		max += 0.5f;
	}

	if ((int)min == 0)
	{
		min += 0.5f;
	}

	A = rand() % (int)max + (int)min;

	B = rand() % (int)max + (int)min;

	AB = A - B;

	AB = (AB) + (A - B);

	return AB;
}

Vector3 wind_direction;

//work on this later<3
void updateWind()
{
	wind_direction.x = 0.f;
	wind_direction.y = 0.f;
	wind_direction.z = 0.f;
}


Vector3 addVec(Vector3 a, Vector3 b)
{
	return (Vector3){a.x + b.x, a.y + b.y, a.z + b.z};
}

void advanceCubes(FarmCube* cubes)
{

	if(farmEXP > exp_to_next_farm_level)
	{
		farmLVL++;
		farmEXP = 0;
		exp_to_next_farm_level += badRNGFunctionShifted(25);
	}

	for(int i = 0; i != 9; i++)
	{
		switch (cubes[i].type)
		{
		case BLNK:
			break;

		case DIRT:
			break;

		case WATER:
			break;

		case SEEDED:
		if(badRNGFunctionShifted(25) == getHighestNumberForBitWidth(25))
		{
			if(cubes[i].limiter > cubes[i].data)
			{
				cubes[i].data++;
			}
		}
			break;

		default:
			break;
		}
	}
}

void jdrawText(const char* text)
{
	EndMode3D();
	DrawText(text, 10, 40, 20, DARKGRAY);
	BeginMode3D(camera);
}

void renderCubes(FarmCube* cubes)
{
	#define cube_dim 10
	#define cube_dim_v (Vector3){cube_dim,cube_dim,cube_dim}
	#define seeddim .4
	#define seeddims seeddim,30,seeddim

	Vector3 seed_offsets[10] =
	{
		(Vector3){+0.1f, 0.f, -0.1f},
		(Vector3){+0.2f, 0.f, -0.2f},
		(Vector3){+0.1f, 0.f, -0.1f},
		(Vector3){+0.3f, 0.f, +0.3f},
		(Vector3){-0.2f, 0.f, +0.2f},
		(Vector3){+0.1f, 0.f, +0.1f},
		(Vector3){-0.3f, 0.f, +0.3f},
		(Vector3){-0.5f, 0.f, -0.5f},
		(Vector3){-0.2f, 0.f, -0.2f},
		(Vector3){-0.4f, 0.f, -0.4f},
	};

	for(int i = 0; i != 9; i++)
	{
		static uint32_t col_seed = 0;
		srand(col_seed);
		col_seed++;
		int randocolval[3] = {rand() % 255, rand() % 255, rand() % 255};
		switch (cubes[i].type)
		{
		case BLNK:
			DrawCubeV(cube_positions[i], cube_dim_v, (Color){randocolval[0], randocolval[1], randocolval[2], 255});
			break;

		case DIRT:
		DrawCubeV(cube_positions[i], cube_dim_v, BROWN);
			break;

		case WATER:
		DrawCubeV(cube_positions[i], cube_dim_v, BLUE);
			break;

		case SEEDED:
		DrawCubeV(cube_positions[i], cube_dim_v, BEIGE);
		DrawLine3D(cube_positions[i], addVec(wind_direction, addVec(addVec(cube_positions[i], seed_offsets[0]), (Vector3){0, cube_dim / 2 + cubes[i].data, 0})), DARKGREEN);
			break;

		default:
			break;
		}
	}
}
void renderBounds()
{
	Ray collis_check_ray = GetMouseRay(GetMousePosition(), camera);
	for(int i = 0; i != 9; i++)
	{
		if (CheckCollisionRayBox(collis_check_ray, cube_bounds[i]))
		{
			DrawBoundingBox(cube_bounds[i], BLUE);
			return;
		}
	}
}

int getHighlightedCube()
{
	Ray collis_check_ray = GetMouseRay(GetMousePosition(), camera);
	for(int i = 0; i != 9; i++)
	{
		if (CheckCollisionRayBox(collis_check_ray, cube_bounds[i]))
		{
			DrawBoundingBox(cube_bounds[i], BLUE);
			return i;
		}
	}
	return -1;
}

int selected_cube = -1;

//This will house code that controls user interaction.
void userInteraction()
{
	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		selected_cube = getHighlightedCube();
	}
}
void displaySelectedCubeInfo(FarmCube* cubes)
{
	if(selected_cube == -1)
	{
		return;
	}
	DrawRectangle(0, GetScreenHeight() - 100, GetScreenWidth(), 100, BLACK);
	const char* type_names[6] =
	{
		"blank",
		"water",
		"dirt",
		"seeded",
		"awd",
		"awdas",
	};
	if(selected_cube == -1){return;}
	int h = cubes[selected_cube].type >> 4;
	DrawText(type_names[h], 10, GetScreenHeight() - 90, 40, WHITE);

	char buffer[30];
	{
		sprintf(buffer, "Growth level %d / %d", cubes[selected_cube].data, cubes[selected_cube].limiter);
		DrawText(buffer, 10, GetScreenHeight() - 50, 40, WHITE);
	}


}

int selected_tool = 0;

void useTool(FarmCube* cubes)
{
	if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && selected_cube != -1)
	{
		if(cubes[selected_cube].limiter == cubes[selected_cube].data)
		{
			farmEXP += badRNGFunctionShifted(28) + cubes[selected_cube].data;
			money += badRNGFunctionShifted(29) + (farmLVL * 2);
			cubes[selected_cube].data = 0;
			cubes[selected_cube].limiter = badRNGFunctionShifted(28) + farmLVL;
		}
	}
}

void farmMenu()
{
	if(IsKeyPressed(KEY_M))
	{
		EndDrawing();
		while (!IsKeyPressed(KEY_ENTER))
		{
			BeginDrawing();
			
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GRAY);
			
			DrawText("Menu", 10, 10, 40, WHITE);
			char buffer[30];
			{
				sprintf(buffer, "Money %d", money);
				DrawText(buffer, 10, 60, 40, WHITE);
			}
			{
				sprintf(buffer, "Farm EXP %d", farmEXP);
				DrawText(buffer, 10, 100, 40, WHITE);
			}
			{
				sprintf(buffer, "Farm Level %d", farmLVL);
				DrawText(buffer, 10, 140, 40, WHITE);
			}
			{
				sprintf(buffer, "exp to next lvl %d", exp_to_next_farm_level);
				DrawText(buffer, 10, 180, 40, WHITE);
			}



			EndDrawing();
		}
	}
}

int main() 
{
	FarmCube cubes[9];

	for(int x = 3; x; x--)
	{
		for(int y = 3; y; y--)
		{
			cubes[(3 * x + y) - 4].data = 0x00;
			cubes[(3 * x + y) - 4].type = SEEDED;
			cubes[(3 * x + y) - 4].limiter = 3;
			
			cube_positions[(3 * x + y) - 4] = (Vector3){10 - (x * cube_dim), -cube_dim, 10 - (y * cube_dim)};
			BoundingBox box;
			box.max = addVec(cube_positions[(3 * x + y) - 4], (Vector3){+cube_dim / 2, +cube_dim / 2, +cube_dim / 2});
			box.min = addVec(cube_positions[(3 * x + y) - 4], (Vector3){-cube_dim / 2, -cube_dim / 2, -cube_dim / 2});
			cube_bounds[(3 * x + y) - 4] = box;
		}
	}
	// Initialization
	//--------------------------------------------------------------------------------------

	InitWindow(screenWidth, screenHeight, "TriTri Farm");

	camera.position = (Vector3){ 3.0f + cube_dim, 2.0f + (cube_dim * 2), 3.0f + cube_dim };
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 70.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	
	SetCameraMode(camera, CAMERA_ORBITAL);

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// UpdateCamera(&camera);
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

			ClearBackground(GREEN);

			BeginMode3D(camera);
				
				renderCubes(cubes);
				renderBounds();

			EndMode3D();

			// DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);
			userInteraction();
			displaySelectedCubeInfo(cubes);
			advanceCubes(cubes);
			updateWind();

			DrawFPS(10, 10);

			camera.position.y += IsKeyPressed(KEY_UP);
			camera.position.x -= IsKeyPressed(KEY_UP);
			camera.position.z -= IsKeyPressed(KEY_UP);

			camera.position.y -= IsKeyPressed(KEY_DOWN);
			camera.position.x += IsKeyPressed(KEY_DOWN);
			camera.position.z += IsKeyPressed(KEY_DOWN);

			if(IsKeyPressed(KEY_C))
			{
				camera.position = (Vector3){ 3.0f + cube_dim, 2.0f + (cube_dim * 2), 3.0f + cube_dim };
			}
			farmMenu();
			useTool(cubes);
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}