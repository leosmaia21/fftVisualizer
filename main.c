#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "raylib/src/raylib.h"

void	callback(void *bufferData, unsigned int frames) {
	int16_t *frame = bufferData;
	for (size_t i = 0; i < frames; i++) {
		float value =(float)frame[i] / INT16_MAX;
		// if (value != 0)
		// printf("%.4f ", value);
	}
	// printf("\n");
}

int main(){

	// char *fileName = "Sultans of Swing.mp3";
	// char *fileName = "The Tower of Dreams (new).ogg";
	char *fileName = "raylib/examples/audio/resources/country.mp3";

	InitWindow(800, 600, "Susana");
	SetTargetFPS(60);

	InitAudioDevice();
	Music music = LoadMusicStream(fileName);

	PlayMusicStream(music);
	AttachAudioStreamProcessor(music.stream, callback);

	while (!WindowShouldClose()) {
		UpdateMusicStream(music);

		BeginDrawing();
		ClearBackground(RED);
		EndDrawing();
	}
}
