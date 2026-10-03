#pragma once
// UI-thread lifetime, before settings and after the Lucky worker is joined.
bool vitaPlatformInit();
void vitaPlatformQuit();
void vitaPlatformPoll();

void vitaInputInit();
void vitaInputReset();
