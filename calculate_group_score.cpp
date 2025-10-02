/*
    班级积分管理系统。在班级一体机上安装这个软件，用积分和学习小组激励同学们上进。Class Points Management System. Install this software on the computer in the classroom. Then use points and study groups to motivate students. 
    Copyright (C) 2025  ShiMingXuanSimon

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <fstream> // �����ļ�����֧��
#include <cmath>   // ������ѧ����֧��

using namespace std;

const int REWARD[] = {3, 4, 5, 8, 15}; // ��0��1��4��5�Ľ���

// ��λ�жϺ���
int getSegment(int rank) {
    if (rank > 1500) return 0;       // ��0��1501����֮��
    else if (rank > 1000) return 1;  // ��1��1001-1500��������1500��
    else if (rank > 800) return 2;   // ��2��801-1000��
    else if (rank > 500) return 3;   // ��3��501-800��
    else if (rank > 200) return 4;   // ��4��201-500��
    else return 5;                   // ��5��ǰ200��
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
    vector<int> student_ids; // �洢ѧ��ѧ��
    vector<int> old_ranks, new_ranks;
    vector<double> scores;
    double core_raw, combat_raw, leap_raw;
    double core_score, combat_score, leap_score, total_score;
};

// ����С��÷ֽ�����ļ�
void saveGroupScores(const vector<Group>& groups, const string& filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "�޷����ļ�����С��÷֣�" << endl;
        return;
    }
    
    for (int g = 0; g < groups.size(); ++g) {
        // ����ÿ��С���Ա�ļӷ�ֵ���ֳܷ���3����ȡ����
        int add_score = static_cast<int>(ceil(groups[g].total_score / 3.0));
        
        // д��С����Ϣ
        outfile << "С��" << g+1 << "�ܷ�:" << groups[g].total_score 
                << ", ÿ�˼ӷ�:" << add_score << "\n";
        
        // д��С���Աѧ��
        outfile << "��Աѧ��:";
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
        cout << "�������꼶��1000���ķ�����";
        cin >> grade_1000th;
        cout << "������С��������";
        cin >> group_count;

        vector<Group> groups(group_count);
        vector<double> combat_raww;

        for (int g = 0; g < group_count; ++g) {
            Group &grp = groups[g];
            cout << "������С��" << g+1 << "��������";
            cin >> grp.n;

            grp.student_ids.resize(grp.n); // ��ʼ��ѧ������
            grp.old_ranks.resize(grp.n);
            grp.new_ranks.resize(grp.n);
            grp.scores.resize(grp.n);

            cout << "������ÿ����Ա��ѧ�� ������ ������ �������ո�ָ�����\n";
            for (int i = 0; i < grp.n; ++i) {
                cin >> grp.student_ids[i] 
                    >> grp.old_ranks[i] 
                    >> grp.new_ranks[i] 
                    >> grp.scores[i];
            }

            // ���Ľ����ּ���
            grp.core_raw = 0;
            for (int rank : grp.new_ranks) {
                grp.core_raw += calculateCoreScore(rank);
            }

            // ս�����ּ���
            double sum_score = 0;
            for (double s : grp.scores) sum_score += s;
            grp.combat_raw = (sum_score / grp.n - grade_1000th) * grp.n;
            combat_raww.push_back(grp.combat_raw);

            // ��Ծ�����ּ���
            grp.leap_raw = 0;
            for (int i = 0; i < grp.n; ++i) {
                grp.leap_raw += calculateLeapScore(grp.old_ranks[i], grp.new_ranks[i]);
            }
        }

        // ս����������
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

        // ������
        for (int g = 0; g < group_count; ++g) {
            cout << "\nС��" << g+1 << "�÷����飺" << endl;
            cout << "�ܵ÷֣�" << groups[g].total_score << endl;
            cout << "���Ľ����֣�" << groups[g].core_score << endl;
            cout << "ս�����֣�" << groups[g].combat_score << endl;
            cout << "��Ծ�����֣�" << groups[g].leap_score << endl;
        }
        
        cout << "\n��1ȷ�ϼӷֲ��������������������¼���: ";
        int sub_choice;
        cin >> sub_choice;
        cin.ignore();
        
        if (sub_choice == 1) {
            // ����С��÷ֽ��
            saveGroupScores(groups, "group_scores.txt");
            cout << "С��÷ֽ���ѱ��浽 group_scores.txt �ļ�" << endl;
            break;
        } else {
            system("cls");
        }
    }
    return 0;
}
