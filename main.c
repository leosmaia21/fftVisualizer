#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib/src/raylib.h"
#include <math.h>
#include <string.h>

Music music;
float data[4096];
_Atomic int numberofframes = 0;

int x = 0;
void ProcessAudio(void *buffer, unsigned int frames)
{
    float *samples = (float *)buffer;   // Samples internally stored as <float>s

	memcpy(data, buffer, frames * 2 * 4);
	numberofframes = frames;
	// memset(data, 0, 4 * 512);
 //    for (unsigned int frame = 0; frame < frames; frame++)
	// {
	// 	float *left = &samples[frame * 2 + 0], *right = &samples[frame * 2 + 1];
	// 	data[frame] = *left;
	// }
}

float map(float x, float smin, float smax, float dmin, float dmax) {
	return ((x - smin) * (dmax - dmin) / (smax - smin)) + dmin;
}
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 900;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Susana");

    InitAudioDevice();              // Initialize audio device


    music = LoadMusicStream("Sultans of Swing.mp3");
    // music = LoadMusicStream("0.01.wav");
    // Music music = LoadMusicStream("raylib/examples/audio/resources/weird.wav");
    // Sound sound = LoadSound("resources/coin.wav");
	
	AttachAudioMixedProcessor(ProcessAudio);
    PlayMusicStream(music);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		// Update
		//----------------------------------------------------------------------------------
		// if (GetMusicTimePlayed(music) < 1) {
		UpdateMusicStream(music);   // Update music buffer with new stream data

		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground((Color){0x18, 0x18, 0x18, 0xFF});
		if (numberofframes != 0) {
			// printf("frames:%d\n", numberofframes);
			int retSize = GetScreenWidth() / numberofframes;	
			float y, x = 0;
			for (size_t i = 0; i < numberofframes; i++) {
				float value = data[2 * i];
				// if (value >= 0) {
				y = map(fabsf(value), -1, 1, GetScreenHeight() / 2.0f, 0);
				x += retSize;
				if (value < 0)
					y += GetScreenHeight() / 2.0f / 2.0f - y;
				DrawRectangle(x, y, retSize, fabsf(value) * GetScreenHeight() / 2.0f / 2, RED);
			}
			numberofframes = 0;
		}

		EndDrawing();
		// }
		//----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(music);   // Unload music stream buffers from RAM

    DetachAudioMixedProcessor(ProcessAudio);  // Disconnect audio processor

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
