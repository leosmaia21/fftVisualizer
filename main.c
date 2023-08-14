#include <stdio.h>
#include "raylib/src/raylib.h"

int main(){

	InitWindow(800, 600, "Susana");
	SetTargetFPS(60);

	InitAudioDevice();
	Music music = LoadMusicStream("Sultans of Swing.mp3");

	PlayMusicStream(music);

	while (!WindowShouldClose()) {
		UpdateMusicStream(music);

		BeginDrawing();
		ClearBackground(RED);
		EndDrawing();
	}
}
