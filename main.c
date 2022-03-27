#include <raylib.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// #include <windows.h>

#include "../MetaLauncher/EngineCode/KettleRenderer.h"
#include "../MetaLauncher/EngineCode/PhysicsObject.h"

#define MAX_SAMPLES               512//05/02/2022 v1.4
#define MAX_SAMPLES_PER_UPDATE   4096//^

const int screenWidth = 450;
const int screenHeight = 450;

#define cube_dim 10
#define cube_dim_v (Vector3){cube_dim,cube_dim,cube_dim}
#define seeddim .4
#define seeddims seeddim,30,seeddim


#ifndef PARTICLEFX
#define PARTICLEFX 1.4

//Animation IDs
#define SPLASH_ANIMATION 0x01
#define DIG_ANIMATION 0x02
#define SPARKLE_ANIMATION 0x03
#define ROCK_BREAK_ANIMATION 0x04

//Animation lengths
#define DIG_ANIM_LENGTH 6000
#define SPLASH_ANIM_LENGTH 6000
#define SPARKLE_ANIM_LENGTH 360
#define ROCK_BREAK_ANIM_LENGTH 360

typedef struct PartiFX
{
	int type;
	int frames_left;
	Vector3 position;
}
PartiFX;

PartiFX* getCurrentPartiFX()
{
	static PartiFX current_animation = { 0 };
	return &current_animation;
}

void renderDigParticles(int frame, Vector3 position);
void renderSplashParticles(int frame, Vector3 position);
void renderSparkleParticles(int frame, Vector3 position);
void renderRockBreakParticles(int frame, Vector3 position);

void initialiseDigAnimation(Vector3 pos)
{
	getCurrentPartiFX()->frames_left = DIG_ANIM_LENGTH;
	getCurrentPartiFX()->type = DIG_ANIMATION;
	getCurrentPartiFX()->position = pos;
}

void renderAnimationFrame(PartiFX* animation)
{
	switch (animation->type)
	{
	case DIG_ANIMATION:
		renderDigParticles(animation->frames_left, animation->position);
		break;

	case SPLASH_ANIMATION:
		renderSplashParticles(animation->frames_left, animation->position);
		break;

	case SPARKLE_ANIMATION:
		renderSparkleParticles(animation->frames_left, animation->position);
		break;

	case ROCK_BREAK_ANIMATION:
		renderRockBreakParticles(animation->frames_left, animation->position);
		break;
	
	default:
		break;
	}
}

void digPartiFX(Vector3 pos)
{
	getCurrentPartiFX()->frames_left = DIG_ANIM_LENGTH;
	getCurrentPartiFX()->type = DIG_ANIMATION;
	getCurrentPartiFX()->position = pos;
}

void renderDigParticles(int frame, Vector3 position)
{
	#define DIRT_AMOUNT 12
	static PhysicsObject dirt[DIRT_AMOUNT];
	if(frame == DIG_ANIM_LENGTH)
	{
		for(int i = 0; i != DIRT_AMOUNT; i++)
		{
			dirt[i].position = position;
			dirt[i].position.x += cube_dim / 2;
			dirt[i].position.z += cube_dim / 2;
			dirt[i].position.y += cube_dim;
			dirt[i].direction = (Vector3){GetRandomValue(1, 3), GetRandomValue(4, 6), GetRandomValue(1, 3)};
			dirt[i].weight = 2.f;
		}
	}
	for(int i = 0; i != DIRT_AMOUNT; i++)
	{
		updatePhysicsObjectWeight(&dirt[i]);
		updatePhysicsObjectPosition(&dirt[i]);
		DrawSphere(dirt[i].position, .25f, BROWN);
	}
}

void splashPartiFX(Vector3 pos)
{
	getCurrentPartiFX()->frames_left = SPLASH_ANIM_LENGTH;
	getCurrentPartiFX()->type = SPLASH_ANIMATION;
	getCurrentPartiFX()->position = pos;
}

void renderSplashParticles(int frame, Vector3 position)
{
	#define SPLASH_AMOUNT 90
	static PhysicsObject droplets[SPLASH_AMOUNT];
	if(frame == SPLASH_ANIM_LENGTH)
	{
		for(int i = 0; i != SPLASH_AMOUNT; i++)
		{
			droplets[i].position = position;
			droplets[i].position.x += (GetRandomValue(1, 3) / 3);
			droplets[i].position.z += (GetRandomValue(1, 3) / 3);
			droplets[i].position.y += 20 + (GetRandomValue(1, 3));
			droplets[i].direction = (Vector3){GetRandomValue(1, 5) / 5, 0, GetRandomValue(1, 5) / 5};
			droplets[i].weight = 5.f;
		}
	}
	for(int i = 0; i != SPLASH_AMOUNT; i++)
	{
		updatePhysicsObjectWeight(&droplets[i]);
		updatePhysicsObjectPosition(&droplets[i]);
		DrawSphere(droplets[i].position, .25f, BLUE);
	}
}

void sparklePartiFX(Vector3 pos)
{
	getCurrentPartiFX()->frames_left = SPARKLE_ANIM_LENGTH;
	getCurrentPartiFX()->type = SPARKLE_ANIMATION;
	getCurrentPartiFX()->position = pos;
}

void renderSparkleParticles(int frame, Vector3 position)
{
	#define SPARKLE_AMOUNT 50
	static PhysicsObject Sparkles[SPARKLE_AMOUNT];
	if(frame == SPARKLE_ANIM_LENGTH)
	{
		for(int i = 0; i != SPARKLE_AMOUNT; i++)
		{
			Sparkles[i].position = position;
			// Sparkles[i].position.x += cube_dim / 2;
			// Sparkles[i].position.z += cube_dim / 2;
			// Sparkles[i].position.y += cube_dim;

			Sparkles[i].direction = (Vector3){GetRandomValue(-10, 10) / 10, GetRandomValue(-10, 10) / 10, GetRandomValue(-10, 10) / 10};
			Sparkles[i].weight = 0.f;
		}
	}
	for(int i = 0; i != SPARKLE_AMOUNT; i++)
	{
		updatePhysicsObjectPosition(&Sparkles[i]);
		DrawSphere(Sparkles[i].position, .25f, (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255});
	}
}


void rockBreakPartiFX(Vector3 pos)
{
	getCurrentPartiFX()->frames_left = ROCK_BREAK_ANIM_LENGTH;
	getCurrentPartiFX()->type = ROCK_BREAK_ANIMATION;
	getCurrentPartiFX()->position = pos;
}

void renderRockBreakParticles(int frame, Vector3 position)
{
	#define FRAGMENT_AMOUNT 10

	static PhysicsObject fragments[FRAGMENT_AMOUNT];
	if(frame == ROCK_BREAK_ANIM_LENGTH)
	{
		for(int i = 0; i != FRAGMENT_AMOUNT; i++)
		{
			fragments[i].position = position;
			// Sparkles[i].position.x += cube_dim / 2;
			// Sparkles[i].position.z += cube_dim / 2;
			// Sparkles[i].position.y += cube_dim;

			fragments[i].direction = (Vector3){GetRandomValue(-3, 3), 5, GetRandomValue(-3, 3)};
			fragments[i].weight = 5.f;
		}
	}
	for(int i = 0; i != FRAGMENT_AMOUNT; i++)
	{
		updatePhysicsObjectWeight(&fragments[i]);
		updatePhysicsObjectPosition(&fragments[i]);
		DrawSphere(fragments[i].position, 1, DARKGRAY);
	}
}

void renderPartiFX()
{
	if(getCurrentPartiFX()->type)
	{
		if(getCurrentPartiFX()->frames_left)
		{
			renderAnimationFrame(getCurrentPartiFX());
			getCurrentPartiFX()->frames_left--;
		}
	}
}

void setPartiFX(int type, Vector3 pos)
{
	switch (type)
	{
	case DIG_ANIMATION:
		digPartiFX(pos);
		break;

	case SPLASH_ANIMATION:
		splashPartiFX(pos);
		break;

	case SPARKLE_ANIMATION:
		sparklePartiFX(pos);
		break;

	case ROCK_BREAK_ANIMATION:
		rockBreakPartiFX(pos);
		break;

	default:
		exit(0);
		break;
	}
}

#endif

#ifndef KE_SOUND
//05/02/2022 v1.4
#define KE_SOUND 1.4

#include "../MetaLauncher/EngineCode/ToasterDSP/ToasterDSP.h"

#define PLANT_GROWN_SOUND 0x01
#define DIG_SOUND 0x02
#define WATERING_SOUND 0x03
#define ROCK_BREAK_SOUND 0x04
#include "bird_chirp.h"

void digSound()
{
	Chirp chrp;
	chrp.frequency = 200;
	chrp.length = 100;
	playChirp(chrp);
}

void plantGrowSound()
{
	Chirp chrp;
	chrp.frequency = 100 * GetRandomValue(4, 6);
	chrp.length = 200;
	playChirp(chrp);
}

void rockBreakSound()
{
	Chirp chrp;
	chrp.frequency = 100;
	chrp.length = 300;
	playChirp(chrp);
}

void plantWaterSound()
{
	Chirp chrp;
	chrp.frequency = 800;
	chrp.length = 100;
	playChirp(chrp);
	chrp.frequency -= 50;
	playChirp(chrp);
	chrp.frequency -= 50;
	playChirp(chrp);
}

void playGameSound(int sound_id)
{
	switch (sound_id)
	{
	case PLANT_GROWN_SOUND:
		plantGrowSound();
		break;

	case DIG_SOUND:
		digSound();
		break;

	case ROCK_BREAK_SOUND:
		rockBreakSound();
		break;
	
	default:
		break;
	}
}

#endif

/*
Itch.io embed
<iframe src="https://itch.io/embed/1382222?linkback=true&amp;border_width=5&amp;bg_color=adf638&amp;fg_color=000000&amp;link_color=1e00f2&amp;border_color=000000" width="560" height="175" frameborder="0"><a href="https://btabtab.itch.io/applet-farm">Applet Farm by btabtab</a></iframe>
*/

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
here is a chart of the maximum values that you can get based on the shift amount ("shift_amount").
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

#define LOCKED 0xF0
#define BLNK 0x00
#define DIRT 0x10
#define WATER 0x20
#define SEEDED 0x30
#define ROCKS 0x40

typedef struct FarmCube
{
	/*
	First byte is state, second state is type.
	*/
	uint8_t data;
	uint8_t type;
	uint8_t limiter;

	int16_t exp;
	uint8_t level;
}
FarmCube;

typedef struct FarmData
{
	int crop_luck;
	int farmEXP;
	int farmLVL;
	int exp_to_next_farm_level;
	int money;

	int next_unlock_cost;

	int seeds_until_rock;

	FarmCube cubes[9];
	int times_can_used[9];
}
FarmData;


int* getSelectedCube()
{
	static int selected_cube = 0;
	return &selected_cube;
}

#include "../MetaLauncher/EngineCode/Saving.h"

void saveGame(FarmData* data, char* file_name)
{
	saveData(data, sizeof(*data), file_name);
}

void loadGame(FarmData* data, char* file_name)
{
	loadData(data, sizeof(*data), file_name);
}


//for floating point RNG
// float JProduceRandomValue(float min, float max)
// {
// 	float A;
// 	float B;
// 	float AB;
// 	if ((int)max == 0)
// 	{
// 		max += 0.5f;
// 	}
// 	if ((int)min == 0)
// 	{
// 		min += 0.5f;
// 	}
// 	A = rand() % (int)max + (int)min;
// 	B = rand() % (int)max + (int)min;
// 	AB = A - B;
// 	AB = (AB) + (A - B);
// 	return AB;
// }

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

void advanceCubes(FarmData* data)
{
	for(int i = 0; i != 9; i++)
	{
		switch (data->cubes[i].type)
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
			if(data->cubes[i].limiter > data->cubes[i].data)
			{
				data->cubes[i].data++;
				if(data->cubes[i].limiter == data->cubes[i].data)
				{
					playGameSound(PLANT_GROWN_SOUND);
					setPartiFX(SPARKLE_ANIMATION, addVec(cube_positions[i], (Vector3){0, cube_dim / 2 + data->cubes[i].data, 0}));
				}
			}
		}
		case ROCKS:
			break;

		default:
			break;
		}
	}
}

int selected_tool = 0;

void renderCubes(FarmData* data)
{

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

	Vector3 generic_vec = { 0 };
	// Vector2 generic_2d;
	char buffer[30];
	for(int i = 0; i != 9; i++)
	{
		static uint32_t col_seed = 0;
		srand(col_seed);
		col_seed++;
		int randocolval[3] = {rand() % 255, rand() % 255, rand() % 255};
		switch (data->cubes[i].type)
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
		generic_vec = addVec(wind_direction, addVec(addVec(cube_positions[i], seed_offsets[0]), (Vector3){0, cube_dim / 2 + data->cubes[i].data, 0}));
		DrawCubeV(cube_positions[i], cube_dim_v, BEIGE);
		DrawLine3D(cube_positions[i], addVec(wind_direction, addVec(addVec(cube_positions[i], seed_offsets[0]), (Vector3){0, cube_dim / 2 + data->cubes[i].data, 0})), DARKGREEN);

		if(data->cubes[i].data == data->cubes[i].limiter)
		{
			DrawSphere(generic_vec, 1, YELLOW);
		}
		else
		{
			DrawSphere(addVec(wind_direction, addVec(addVec(cube_positions[i], seed_offsets[0]), (Vector3){0, cube_dim / 2 + data->cubes[i].data, 0})), 1, DARKGREEN);
		}
			break;

		case ROCKS:
		DrawCubeV(cube_positions[i], cube_dim_v, BROWN);
		DrawSphere(addVec(cube_positions[i], (Vector3){0, cube_dim / 2, 0}), 4, GRAY);

			break;

		case LOCKED:
		DrawCubeV(cube_positions[i], cube_dim_v, (Color){128, 128, 128, 100});
			break;

		default:
			break;
		}
	}
}

void renderCubesLabels(FarmData* data)
{
	Vector3 generic_vec = { 0 };
	Vector2 generic_2d;
	char buffer[30];
	for(int i = 0; i != 9; i++)
	{
		generic_vec = cube_positions[i];
		switch (data->cubes[i].type)
		{
		case BLNK:
			break;

		case DIRT:
			break;

		case WATER:
			break;

		case SEEDED:
		generic_vec = addVec(generic_vec, (Vector3){0, cube_dim / 2 + data->cubes[i].data, 0});

		generic_2d = GetWorldToScreen(generic_vec, camera);
		sprintf(buffer, "[%d/%d]", data->cubes[i].data, data->cubes[i].limiter);
		DrawText(buffer, generic_2d.x, generic_2d.y, 15, BLACK);
		
		if(data->times_can_used[i] == 1)
		{
			DrawText("O", generic_2d.x, generic_2d.y - 15, 15, DARKBLUE);
		}
		if(data->times_can_used[i] == 2)
		{
			DrawText("OO", generic_2d.x, generic_2d.y - 15, 15, DARKBLUE);
		}
			break;

		case ROCKS:
		generic_2d = GetWorldToScreen(generic_vec, camera);
		sprintf(buffer, "[%d/%d]", data->cubes[i].data, data->cubes[i].limiter);
		DrawText(buffer, generic_2d.x, generic_2d.y, 15, BLACK);
			break;

		case LOCKED:
			generic_2d = GetWorldToScreen(generic_vec, camera);
			sprintf(buffer, "£%d", data->next_unlock_cost);
			DrawText(buffer, generic_2d.x, generic_2d.y - 30, 15, DARKGREEN);
			break;

		default:
			break;
		}
	}
}

void getHighlightedCube()
{
	*getSelectedCube() += IsKeyPressed(KEY_UP);
	if(8 < *getSelectedCube()){*getSelectedCube() = 0;}
	*getSelectedCube() -= IsKeyPressed(KEY_DOWN);
	if(*getSelectedCube() < 0){*getSelectedCube() = 8;}
	DrawBoundingBox(cube_bounds[*getSelectedCube()], WHITE);
}

//This will house code that controls user interaction.
void userInteraction()
{
	// if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_Z))
	// {
	// 	*getSelectedCube() = getHighlightedCube();
	// }
}
void displaySelectedCubeInfo(FarmData* data)
{
	if(*getSelectedCube() == -1)
	{
		return;
	}
	DrawRectangle(0, GetScreenHeight() - 100, GetScreenWidth(), 100, BLACK);
	const char* type_names[6] =
	{
		"blank",
		"dirt",
		"water",
		"seeded",
		"rock",
		"awdas",
	};
	if(*getSelectedCube() == -1){return;}

	int h = data->cubes[*getSelectedCube()].type >> 4;

	DrawText(type_names[h], 10, GetScreenHeight() - 90, 30, WHITE);

	char buffer[128];

	switch (data->cubes[*getSelectedCube()].type)
	{
	case BLNK:
		break;
	case DIRT:
		DrawText("Needs to be seeded!", 10, GetScreenHeight() - 50, 20, WHITE);

		sprintf(buffer, "\nCube Level: %d, EXP to next level: %d", data->cubes[*getSelectedCube()].level, data->cubes[*getSelectedCube()].exp);
		DrawText(buffer, 10, GetScreenHeight() - 50, 20, WHITE);

		break;
	case WATER:
		break;
	case SEEDED:
		{
			sprintf(buffer, "Growth level %d / %d\nCube Level: %d, EXP to next level: %d", data->cubes[*getSelectedCube()].data, data->cubes[*getSelectedCube()].limiter, data->cubes[*getSelectedCube()].level, data->cubes[*getSelectedCube()].exp);
			DrawText(buffer, 10, GetScreenHeight() - 50, 20, WHITE);
		}
		break;
	case ROCKS:
		{
			sprintf(buffer, "Strikes left %d / %d", data->cubes[*getSelectedCube()].data, data->cubes[*getSelectedCube()].limiter);
			DrawText(buffer, 10, GetScreenHeight() - 50, 20, WHITE);
		}
		break;
	case LOCKED:
		{
			sprintf(buffer, "Unlock Cost %d", data->next_unlock_cost);
			DrawText(buffer, 10, GetScreenHeight() - 50, 20, WHITE);
		}

	default:
		break;
	}




}

#define SPADE 0x00
#define SEEDS 0x01
#define PICKAXE 0x02
#define WATERING_CAN 0x03
#define DEBUG_WAND 0x04

FarmCube newRocks(FarmCube old_data)
{
	FarmCube ret;
	ret = old_data;

	ret.limiter = 1 + GetRandomValue(0, old_data.level + 2);
	ret.data = 0;
	ret.type = ROCKS;
	return ret;
}

FarmCube newFarmLand(FarmCube old_data)
{
	FarmCube ret;
	ret = old_data;

	ret.limiter = 0;
	ret.data = 0;
	ret.type = DIRT;
	return ret;
}

FarmCube newSeeded(FarmCube old_data)
{
	FarmCube ret;
	ret = old_data;

	ret.limiter = GetRandomValue(ret.level + 1,  ret.level + 4);
	ret.data = 0;
	ret.type = SEEDED;
	return ret;
}

void setCubeType(FarmCube* cube, int cube_type)
{
	switch (cube_type)
	{
	case DIRT:
		*cube = newFarmLand(*cube);
		break;

	case ROCKS:
		*cube = newRocks(*cube);
		break;

	case SEEDED:
		*cube = newSeeded(*cube);
		break;
	default:
		break;
	}
}

int useTool(FarmData* data)
{
	static int selected_tool = 0;
	if(IsKeyDown(KEY_D) && IsKeyDown(KEY_E) && IsKeyPressed(KEY_B))
	{
		selected_tool = DEBUG_WAND;
	}
	if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) || IsKeyPressed(KEY_X))
	{
		selected_tool++;
		if(selected_tool > 3)
		{
			selected_tool = 0;
		}
	}
	if(IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON) || IsKeyPressed(KEY_C))
	{
		selected_tool--;
		if(selected_tool < 0)
		{
			selected_tool = 3;
		}
	}

	if((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsKeyPressed(KEY_Z)) && *getSelectedCube() != -1)
	{
		if(data->cubes[*getSelectedCube()].type == LOCKED)
		{
			if(data->next_unlock_cost <= data->money + 1)
			{
				data->money -= data->next_unlock_cost;
				data->next_unlock_cost += GetRandomValue(data->next_unlock_cost, data->next_unlock_cost * 3);
				data->cubes[*getSelectedCube()].type = DIRT;
			}
		}
		switch (selected_tool)
		{
		case SPADE:
			playGameSound(DIG_SOUND);
			if(data->cubes[*getSelectedCube()].data != data->cubes[*getSelectedCube()].limiter)
			{
				return selected_tool;
			}
			if(data->cubes[*getSelectedCube()].type == SEEDED)
			{
				data->cubes[*getSelectedCube()].exp -= data->cubes[*getSelectedCube()].data;
				
				if(data->cubes[*getSelectedCube()].exp < 1)
				{
					data->cubes[*getSelectedCube()].level++;
					data->cubes[*getSelectedCube()].exp = GetRandomValue(10, 15 + (data->cubes[*getSelectedCube()].level * 5));//(data->cubes[*getSelectedCube()].level * data->cubes[*getSelectedCube()].level) * data->cubes[*getSelectedCube()].limiter;
				}

				data->money += GetRandomValue(1 + data->cubes[*getSelectedCube()].level, data->cubes[*getSelectedCube()].limiter + data->cubes[*getSelectedCube()].level + 1);

				if(!data->seeds_until_rock)
				{
					data->seeds_until_rock = GetRandomValue(3, 3 + data->farmLVL);
					data->cubes[*getSelectedCube()] = newRocks(data->cubes[*getSelectedCube()]);
					return selected_tool;
				}
				data->seeds_until_rock--;

				data->cubes[*getSelectedCube()] = newFarmLand(data->cubes[*getSelectedCube()]);
				setPartiFX(DIG_ANIMATION, cube_positions[*getSelectedCube()]);
				data->times_can_used[*getSelectedCube()]--;

			}
			break;

		case SEEDS:
			if(data->cubes[*getSelectedCube()].type == DIRT)
			{
				data->cubes[*getSelectedCube()] = newSeeded(data->cubes[*getSelectedCube()]);
			}
			break;

		case PICKAXE:
		if (data->cubes[*getSelectedCube()].type == ROCKS)
		{
			data->cubes[*getSelectedCube()].data++;
			if(data->cubes[*getSelectedCube()].data == data->cubes[*getSelectedCube()].limiter)
			{
				playGameSound(ROCK_BREAK_SOUND);
				setPartiFX(ROCK_BREAK_ANIMATION, cube_positions[*getSelectedCube()]);
				data->exp_to_next_farm_level -= (data->cubes[*getSelectedCube()].data * 2);
				data->cubes[*getSelectedCube()] = newFarmLand(data->cubes[*getSelectedCube()]);
			}
		}
			break;
			/*
			Realease notes:
			using the watering can will decrease the amount the plant *grown* and increase the amount it can grow. This can only be used a few times per crop.
			*/
		case WATERING_CAN:
		
			if(data->cubes[*getSelectedCube()].type != SEEDED)
			{
				return selected_tool;
			}
			if(data->cubes[*getSelectedCube()].limiter == data->cubes[*getSelectedCube()].data)
			{
				if(2 <= data->times_can_used[*getSelectedCube()])
				{
					return selected_tool;
				}
				data->times_can_used[*getSelectedCube()]++;
				playGameSound(WATERING_SOUND);
				Vector3 pos = cube_positions[*getSelectedCube()];
				setPartiFX(SPLASH_ANIMATION, pos);
				if(3 >= data->cubes[*getSelectedCube()].limiter)
				{
					data->cubes[*getSelectedCube()].limiter++;
					data->cubes[*getSelectedCube()].data = 0;
					return selected_tool;
				}
				data->cubes[*getSelectedCube()].limiter += data->cubes[*getSelectedCube()].limiter / 4;
				data->cubes[*getSelectedCube()].data = data->cubes[*getSelectedCube()].data / 4;
			}
			break;

		case DEBUG_WAND:

			if(IsKeyDown(KEY_Q))
			{
				Vector3 pos = cube_positions[*getSelectedCube()];
				pos.y += 2;
				setPartiFX(SPARKLE_ANIMATION, pos);
				return selected_tool;
			}
			if(IsKeyDown(KEY_W))
			{
				Vector3 pos = cube_positions[*getSelectedCube()];
				pos.y += 2;
				setPartiFX(SPLASH_ANIMATION, pos);
				return selected_tool;
			}
			if(IsKeyDown(KEY_TAB))
			{
				data->cubes[*getSelectedCube()].level++;
				Vector3 pos = cube_positions[*getSelectedCube()];
				pos.y += 2;
				setPartiFX(SPARKLE_ANIMATION, pos);
				return selected_tool;
			}
			if(IsKeyDown(KEY_A))
			{
				data->cubes[*getSelectedCube()] = newRocks(data->cubes[*getSelectedCube()]);
			}
			if(IsKeyDown(KEY_M))
			{
				data->money += 8;
			}

			break;
		
		default:
			break;
		}
	}
	return selected_tool;
}

void renderTool(int tool)
{
	#define TOOL_OFFSET_AMOUNT 50
	DrawCircle(40, 40, 40, ORANGE);
	switch (tool)
	{
	case SPADE:
	DrawText(">--D", 20, 30, 20, BLACK);
		break;
	
	case SEEDS:
	DrawText("[:.`,]", 20, 30, 20, BLACK);
		break;

		case PICKAXE:
	DrawText("---}", 20, 30, 20, BLACK);
		break;

	case WATERING_CAN:
	DrawText("\\|_|]", 20, 30, 20, BLACK);
		break;

	case DEBUG_WAND:
	DrawText("#---", 20, 30, 20, BLACK);
		break;

	default:
		break;
	}
}

void farmMenu(FarmData* data)
{
	// if(IsKeyPressed(KEY_M))
	{
		// EndDrawing();
		// while (!IsKeyPressed(KEY_ENTER))
		{
			// BeginDrawing();
			int text_size = 20;
			int Y_offset = 250;
			char buffer[30];
			{
				sprintf(buffer, "Money: %d", data->money);
				DrawText(buffer, 0, Y_offset + (text_size * 1), text_size, WHITE);
			}
			{
				sprintf(buffer, "Farm EXP left: %d", data->farmEXP);
				DrawText(buffer, 0, Y_offset + (text_size * 2), text_size, WHITE);
			}
			{
				sprintf(buffer, "Farm Level: %d", data->farmLVL);
				DrawText(buffer, 0, Y_offset + (text_size * 3), text_size, WHITE);
			}
			{
				sprintf(buffer, "exp to next lvl: %d", data->exp_to_next_farm_level);
				DrawText(buffer, 0, Y_offset + (text_size * 4), text_size, WHITE);
			}



			// EndDrawing();
		}
	}
}

void showKeyBindScreen()
{
	BeginDrawing();
	{
		ClearBackground(WHITE);
		char* text[] =
		{
			"Up arrow  | next farmcube",
			"Down arrow| previous farmcube",
			"Z         | Use selected tool",
			"X         | Next tool",
			"C         | Last tool",
			"R         | Switches camera style",
			"[HOLD S ] + Enter: Save",
			"[HOLD L + O] + Enter: Load",
		};
		for(int i = 0 ; i != sizeof(text) / sizeof(char*); i++)
		{
			DrawText(text[i], 20, 20 + (i * 20), 20, BLACK);
		}
	}
	EndDrawing();
}

int main() 
{

	FarmData farm_data;
	farm_data.crop_luck = 0;
	farm_data.exp_to_next_farm_level = 20;
	farm_data.farmEXP = 0;
	farm_data.farmLVL = 1;
	farm_data.money = 0;
	farm_data.next_unlock_cost = 8;

	for(int i = 0; i != 9; i++)
	{
		farm_data.times_can_used[i] = 0;
	}

	int curr_cube;

	for(int x = 3; x; x--)
	{
		for(int y = 3; y; y--)
		{
			curr_cube = (3 * x + y) - 4;
			farm_data.cubes[curr_cube].level = 0;
			farm_data.cubes[curr_cube].exp = GetRandomValue(10, 40);
			farm_data.cubes[curr_cube].data = 0x00;
			farm_data.cubes[curr_cube].type = LOCKED;
			farm_data.cubes[curr_cube].limiter = 3;
			
			cube_positions[curr_cube] = (Vector3){10 - (x * cube_dim), -cube_dim, 10 - (y * cube_dim)};
			BoundingBox box;
			box.max = addVec(cube_positions[(3 * x + y) - 4], (Vector3){+cube_dim / 2, +cube_dim / 2, +cube_dim / 2});
			box.min = addVec(cube_positions[(3 * x + y) - 4], (Vector3){-cube_dim / 2, -cube_dim / 2, -cube_dim / 2});
			cube_bounds[curr_cube] = box;
		}
	}
	farm_data.cubes[4].type = SEEDED;
	// Initialization
	//--------------------------------------------------------------------------------------

	InitWindow(screenWidth, screenHeight, "Applet Farm");

	camera.position = (Vector3){ 3.0f + cube_dim, 2.0f + (cube_dim * 2), 3.0f + cube_dim };
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 70.0f;
	camera.projection = CAMERA_ORTHOGRAPHIC;
	
	SetCameraMode(camera, CAMERA_ORBITAL);

	HideCursor();

	SetTargetFPS(30);               // Set our game to run at 60 frames-per-second
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
				
				getHighlightedCube();
				renderCubes(&farm_data);
				renderPartiFX();
				// renderBounds();
			EndMode3D();

			// DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);
			userInteraction();
			displaySelectedCubeInfo(&farm_data);
			renderCubesLabels(&farm_data);
			advanceCubes(&farm_data);
			updateWind();
			// DrawFPS(10, 10);

			// {
			// 	camera.position.y += IsKeyDown(KEY_UP);
			// 	camera.position.x -= IsKeyDown(KEY_UP);
			// 	camera.position.z -= IsKeyDown(KEY_UP);
			// }
			// {
			// 	camera.position.y -= IsKeyDown(KEY_DOWN);
			// 	camera.position.x += IsKeyDown(KEY_DOWN);
			// 	camera.position.z += IsKeyDown(KEY_DOWN);
			// }

			if(IsKeyPressed(KEY_R))
			{
				static int render_perspective_mode = 0;
				render_perspective_mode = !render_perspective_mode;
				int mode[2] = {CAMERA_ORTHOGRAPHIC, CAMERA_PERSPECTIVE};
				camera.projection = mode[render_perspective_mode];
				camera.position = (Vector3){ 3.0f + cube_dim, 2.0f + (cube_dim * 2), 3.0f + cube_dim };
				camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
			}

			// if(IsKeyPressed(KEY_V))
			// {
			// 	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
			// 	camera.position = (Vector3){3.0f + cube_dim, 2.0f + (cube_dim * 2), 3.0f + cube_dim};
			// }
			// if(IsKeyPressed(KEY_B))
			// {
			// 	camera.target = (Vector3){ 0.0f, 10.0f, 30.0f };
			// 	camera.position = (Vector3){0, 10, 0};
			// }
			farmMenu(&farm_data);

			renderTool(useTool(&farm_data));

			if(IsKeyDown(KEY_S))
			{
				DrawText("! HOLDING 'S' TO SAVE !\nPress 'Enter' to save", screenWidth / 3, screenHeight / 3, 15, WHITE);
				if(IsKeyPressed(KEY_ENTER))
				{
					DrawText("SAVED!", screenWidth / 2, screenHeight / 2, 15, RED);
					saveGame(&farm_data, "appletfarm.save");
				}
			}
			if(IsKeyDown(KEY_L) && IsKeyDown(KEY_O))
			{
				DrawText("! HOLDING 'L' AND 'O' TO LOAD !\nPress 'Enter' to save", screenWidth / 3, screenHeight / 3, 15, WHITE);
				if(IsKeyPressed(KEY_ENTER))
				{
					DrawText("Loaded!", screenWidth / 2, screenHeight / 2, 15, RED);
					saveGame(&farm_data, "appletfarm_backup.save");
					loadGame(&farm_data, "appletfarm.save");
				}
			}


		DrawText("Hold H for keybinds<3.", 450 - 230, 0, 20, BLACK);
		EndDrawing();

		if(IsKeyPressed(KEY_K))
		{
			TakeScreenshot("scrnshot.png");
		}

		while(IsKeyDown(KEY_H))
		{
			showKeyBindScreen();
		}
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}