#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

#include "Common.h"
#include <fstream>
#include <algorithm>
class HighScoreManager {
public:
    HighScoreManager(const std::string& filename);
    ~HighScoreManager();
    void saveHighScores();
    void addScore(int score);
    const std::vector<int>& getHighScores() const { return highScores; }
    int getMaxScore() {return ScoreMax;}
private:
    int ScoreMax ;
    std::string fileName;
    std::vector<int> highScores;
};

#endif //HIGHSCORE_H

