
#include "HighScore.h"

// constructor với filename
HighScoreManager::HighScoreManager(const std::string& filename): fileName(filename)
{
    highScores.clear(); // xóa vector điểm hiện tại
    std::ifstream infile(fileName);
    if (infile.is_open()) {
        int score;
        while (infile >> score) {
            highScores.push_back(score);
        }
        infile.close();
    }
    while (highScores.size() < 5) // nếu không đủ 5 thêm 0 vào
        highScores.push_back(0);
    ScoreMax = highScores[0];
}
// destructor, lưu lại điểm khi bị hủy
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
// add Score vào file
void HighScoreManager::addScore(int score)
{
    // sắp xếp giảm dần
    std::sort(highScores.begin(), highScores.end(), std::greater<int>());
    if (!highScores.empty() && score <= highScores.back())
        return;
    for(int i = 0; i < (int)highScores.size(); i++) // nếu tồn tại trong danh sách thì không thêm
    {
        if(score == highScores[i]) return ;
    }
    highScores.push_back(score); // thêm điểm vào danh sách
    std::sort(highScores.begin(), highScores.end(), std::greater<int>());
    if (highScores.size() > 5) // giới hạn chỉ lấy 5 phần tử
        highScores.resize(5);
    saveHighScores(); // lưu vào file
}
