#pragma once

int windowInit(int window_width, int window_height, char* window_title);
int windowShouldClose();
void windowFlush();
void windowPollEvents();
void windowTerminate();
