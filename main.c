#include <complex.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib/src/raylib.h"
#include <math.h>
#include <string.h>

typedef struct  {
	float real;
	float imag;
	float phase;
	float magnitude;
} t_complex;

Music music;
float data[900];
t_complex dataDFT[900];

_Atomic int numberofframes = 0;

void ProcessAudio(void *buffer, unsigned int frames)
{
    float *samples = (float *)buffer;   // Samples internally stored as <float>s

	for (int i = 0; i < frames; i++) {
		data[i] = samples[i * 2];
	}
	// memcpy(data, buffer, frames * 2 * 4);
	numberofframes = frames;
}

static inline float map(float x, float smin, float smax, float dmin, float dmax) {
	return ((x - smin) * (dmax - dmin) / (smax - smin)) + dmin;
}

void dft(float* x_real, int N, t_complex ret[]) {
	for (int k = 0; k < N; k++) {
		ret[k].real = 0.0f;
		ret[k].imag = 0.0f;
		for (int n = 0; n < N; n++) {
			float angle = 2 * PI * k * n / N;
			ret[k].real += x_real[n] * cos(angle) / sqrt(N);
			ret[k].imag -= x_real[n] * sin(angle) / sqrt(N);
		}
		ret[k].magnitude = sqrt(ret[k].magnitude * ret[k].magnitude + ret[k].phase * ret[k].phase);
		ret[k].phase = atan2(ret[k].phase, ret[k].magnitude);
	}
}


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
	t_complex dataDFT[900];
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
			dft(data, numberofframes, dataDFT);
			float deltaFs = (float)music.stream.sampleRate / numberofframes;
			int retSize = GetScreenWidth() / numberofframes;	
			float y, x = 0;
			for (int i = 0; i < numberofframes; i++) {
				float value = data[i];
				y = map(fabsf(value), -1, 1, GetScreenHeight() / 2.0f, 0);
				x += retSize;
				if (value < 0)
					y += GetScreenHeight() / 2.0f / 2.0f - y;
				DrawRectangle(x, y, retSize, fabsf(value) * GetScreenHeight() / 2.0f / 2, RED);

				value = dataDFT[i].magnitude;
				printf("magnitude:%f\n", value);
				// y = map(fabsf(value), -1, 1, GetScreenHeight() / 2.0f, 0);
				// x += retSize;
				// // if (value < 0)
				// // 	y += GetScreenHeight() / 2.0f / 2.0f - y;
				// DrawRectangle(x, y, retSize, fabsf(value) * GetScreenHeight() / 2.0f , RED);


			}
			printf("frames:%d\n", numberofframes);
			numberofframes = 0;

		}

		EndDrawing();
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
