#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <fstream> // 添加文件操作支持
#include <cmath>   // 添加数学函数支持

using namespace std;

const int REWARD[] = {3, 4, 5, 8, 15}; // 段0→1到4→5的奖励

// 段位判断函数
int getSegment(int rank) {
    if (rank > 1500) return 0;       // 段0：1501名及之后
    else if (rank > 1000) return 1;  // 段1：1001-1500名（包含1500）
    else if (rank > 800) return 2;   // 段2：801-1000名
    else if (rank > 500) return 3;   // 段3：501-800名
    else if (rank > 200) return 4;   // 段4：201-500名
    else return 5;                   // 段5：前200名
}

int calculateCoreScore(int new_rank) {
    if (new_rank <= 200) return 15;
    else if (new_rank <= 500) return 8;
    else if (new_rank <= 800) return 5;
    else if (new_rank <= 1000) return 4;
    else if (new_rank <= 1500) return 3;
    else return -3;
}

double calculateLeapScore(int old_rank, int new_rank) {
    int old_seg = getSegment(old_rank);
    int new_seg = getSegment(new_rank);
    if (new_seg <= old_seg) return 0.0;
    
    double reward_sum = 0.0;
    for (int i = old_seg; i < new_seg; ++i) {
        if (i >= 5) break;
        reward_sum += REWARD[i];
    }
    if (new_seg - old_seg >= 2) reward_sum *= 1.5;
    return reward_sum;
}

struct Group {
    int n;
    vector<int> student_ids; // 存储学生学号
    vector<int> old_ranks, new_ranks;
    vector<double> scores;
    double core_raw, combat_raw, leap_raw;
    double core_score, combat_score, leap_score, total_score;
};

// 保存小组得分结果到文件
void saveGroupScores(const vector<Group>& groups, const string& filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "无法打开文件保存小组得分！" << endl;
        return;
    }
    
    for (int g = 0; g < groups.size(); ++g) {
        // 计算每个小组成员的加分值（总分除以3向上取整）
        int add_score = static_cast<int>(ceil(groups[g].total_score / 3.0));
        
        // 写入小组信息
        outfile << "小组" << g+1 << "总分:" << groups[g].total_score 
                << ", 每人加分:" << add_score << "\n";
        
        // 写入小组成员学号
        outfile << "成员学号:";
        for (int id : groups[g].student_ids) {
            outfile << id << " ";
        }
        outfile << "\n\n";
    }
    outfile.close();
}

int main() {
    while(true) {
        int grade_1000th, group_count;
        cout << "请输入年级第1000名的分数：";
        cin >> grade_1000th;
        cout << "请输入小组数量：";
        cin >> group_count;

        vector<Group> groups(group_count);
        vector<double> combat_raww;

        for (int g = 0; g < group_count; ++g) {
            Group &grp = groups[g];
            cout << "请输入小组" << g+1 << "的人数：";
            cin >> grp.n;

            grp.student_ids.resize(grp.n); // 初始化学号向量
            grp.old_ranks.resize(grp.n);
            grp.new_ranks.resize(grp.n);
            grp.scores.resize(grp.n);

            cout << "请输入每个成员的学号 旧排名 新排名 分数（空格分隔）：\n";
            for (int i = 0; i < grp.n; ++i) {
                cin >> grp.student_ids[i] 
                    >> grp.old_ranks[i] 
                    >> grp.new_ranks[i] 
                    >> grp.scores[i];
            }

            // 核心进步分计算
            grp.core_raw = 0;
            for (int rank : grp.new_ranks) {
                grp.core_raw += calculateCoreScore(rank);
            }

            // 战斗力分计算
            double sum_score = 0;
            for (double s : grp.scores) sum_score += s;
            grp.combat_raw = (sum_score / grp.n - grade_1000th) * grp.n;
            combat_raww.push_back(grp.combat_raw);

            // 飞跃奖励分计算
            grp.leap_raw = 0;
            for (int i = 0; i < grp.n; ++i) {
                grp.leap_raw += calculateLeapScore(grp.old_ranks[i], grp.new_ranks[i]);
            }
        }

        // 战斗力分折算
        double max_combat = *max_element(combat_raww.begin(), combat_raww.end());
        if (max_combat > 0) {
            for (auto &grp : groups) {
                grp.core_score = grp.core_raw * 0.5;
                grp.leap_score = grp.leap_raw * 0.2;
                grp.combat_score = (grp.combat_raw / max_combat) * 15 * 0.3;
                grp.total_score = grp.core_score + grp.combat_score + grp.leap_score;
            }
        } else {
            for (auto &grp : groups) {
                grp.core_score = grp.core_raw * 0.7;
                grp.leap_score = grp.leap_raw * 0.3;
                grp.combat_score = 0;
                grp.total_score = grp.core_score + grp.leap_score;
            }
        }

        // 输出结果
        for (int g = 0; g < group_count; ++g) {
            cout << "\n小组" << g+1 << "得分详情：" << endl;
            cout << "总得分：" << groups[g].total_score << endl;
            cout << "核心进步分：" << groups[g].core_score << endl;
            cout << "战斗力分：" << groups[g].combat_score << endl;
            cout << "飞跃奖励分：" << groups[g].leap_score << endl;
        }
        
        cout << "\n按1确认加分并保存结果，其他按键重新计算: ";
        int sub_choice;
        cin >> sub_choice;
        cin.ignore();
        
        if (sub_choice == 1) {
            // 保存小组得分结果
            saveGroupScores(groups, "group_scores.txt");
            cout << "小组得分结果已保存到 group_scores.txt 文件" << endl;
            break;
        } else {
            system("cls");
        }
    }
    return 0;
}
