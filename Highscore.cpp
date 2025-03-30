
#include "HighScore.h"

HighScoreManager::HighScoreManager(const std::string& filename): fileName(filename)
{
    highScores.clear();
    std::ifstream infile(fileName);
    if (infile.is_open()) {
        int score;
        while (infile >> score) {
            highScores.push_back(score);
        }
        infile.close();
    }
    while (highScores.size() < 5)
        highScores.push_back(0);
}

HighScoreManager::~HighScoreManager()
{
    saveHighScores();
}



void HighScoreManager::saveHighScores()
{
    std::ofstream outfile(fileName);
    if (outfile.is_open()) {
        for (int score : highScores) {
            outfile << score << std::endl;
        }
        outfile.close();
    }
}

void HighScoreManager::addScore(int score)
{
    std::sort(highScores.begin(), highScores.end(), std::greater<int>());
    if (!highScores.empty() && score <= highScores.back())
        return;
    for(int i = 0; i < (int)highScores.size(); i++)
    {
        if(score == highScores[i]) return ;
    }
    highScores.push_back(score);
    std::sort(highScores.begin(), highScores.end(), std::greater<int>());
    if (highScores.size() > 5)
        highScores.resize(5);
    saveHighScores();
}
