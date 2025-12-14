#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

extern "C"{
    EM_JS(void, SaveScoreToWeb, (int score),{ window.saveScore(score); });
    EM_JS(void, GetHighScoresFromWeb, (), { window.getHighScores(); });
}
#endif
