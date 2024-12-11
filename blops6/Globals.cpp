#include "Globals.h"

float ScreenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2;
float ScreenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2;
float ScreenX = GetSystemMetrics(SM_CXSCREEN);
float ScreenY = GetSystemMetrics(SM_CYSCREEN);
std::mutex mutexlock;