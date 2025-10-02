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
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <climits>
#include <sstream>
#include <map>
#include <cstdint>
#include <ctime>
#include <windows.h>

using namespace std;

// 设置控制台编码为中文GBK
void setConsoleEncoding() {
    SetConsoleOutputCP(936);  // GBK编码
    SetConsoleCP(936);        // 输入也使用GBK
}

struct Student {
    int id;
    string name;
    string gender;
    long long old_score, score;
    int old_rank, rank;
};

struct Rule {
    string desc;
    int delta;
};

vector<Student> students;
vector<Rule> rules;

// 文件操作函数
void load_students() {
    ifstream fin("students.dat");
    if (!fin) {
        cout << "未找到学生数据文件，将自动创建该文件" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        size_t pos1 = line.find('|');
        if (pos1 == string::npos) {
            cout << "数据格式错误，请检查：" << line << "行" << endl;
            continue;
        }

        int id = stoi(line.substr(0, pos1));
        Student student;
        student.id = id;

        size_t pos2 = line.find('|', pos1 + 1);
        if (pos2 == string::npos) {
            cout << "姓名字段格式错误，请检查：" << line << "行" << endl;
            continue;
        }
        student.name = line.substr(pos1 + 1, pos2 - pos1 - 1);

        size_t pos3 = line.find('|', pos2 + 1);
        if (pos3 == string::npos) {
            cout << "性别字段格式错误，请检查：" << line << "行" << endl;
            continue;
        }
        student.gender = line.substr(pos2 + 1, pos3 - pos2 - 1);

        size_t pos4 = line.find('|', pos3 + 1);
        if (pos4 == string::npos || pos4 + 1 > line.length()) {
            cout << "旧积分字段格式错误，请检查：" << line << "行" << endl;
            continue;
        }
        student.old_score = stoll(line.substr(pos3 + 1, pos4 - pos3 - 1));

        size_t pos5 = line.find('|', pos4 + 1);
        if (pos5 == string::npos || pos5 + 1 > line.length()) {
            cout << "积分字段格式错误，请检查：" << line << "行" << endl;
            continue;
        }
        student.score = stoll(line.substr(pos4 + 1, pos5 - pos4 - 1));

        size_t pos6 = line.find('|', pos5 + 1);
        if (pos6 == string::npos || pos6 + 1 > line.length()) {
            cout << "旧排名字段格式错误，请检查：" << line << "行" << endl;
            continue;
        }
        student.old_rank = stoi(line.substr(pos5 + 1, pos6 - pos5 - 1));

        student.rank = stoi(line.substr(pos6 + 1));
        students.push_back(student);
    }
    fin.close();
}

void save_students() {
    ofstream fout("students.dat");
    for (const auto& student : students) {
        fout << student.id << "|"
             << student.name << "|"
             << student.gender << "|"
             << student.old_score << "|"
             << student.score << "|"
             << student.old_rank << "|"
             << student.rank << "\n";
    }
    fout.close();
}

void load_rules() {
    ifstream fin("rules.dat");
    if (!fin) {
        cout << "未找到规则文件，将自动创建该文件" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        size_t pos = line.find_last_of('|');
        if (pos == string::npos || pos + 1 >= line.length()) {
            cout << "无效规则格式：" << line << "行" << endl;
            continue;
        }
        Rule r;
        r.desc = line.substr(0, pos);
        try {
            r.delta = stoi(line.substr(pos + 1));
        } catch (...) {
            cout << "分值转换错误：" << line << "行" << endl;
            continue;
        }
        rules.push_back(r);
    }
    fin.close();
}

void save_rules() {
    ofstream fout("rules.dat");
    for (size_t i = 0; i < rules.size(); i++) {
        fout << rules[i].desc << "|" << rules[i].delta << endl;
    }
    fout.close();
}

map<string, string> loadConfig(string filename) {
    map<string, string> config;

    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 1);
                config[key] = value;
            }
        }
        file.close();
    }
    return config;
}

uint64_t hash_password(const string& str) {
    uint64_t hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return hash;
}

// 核心功能函数
bool verify_password(uint64_t stored_hash) {
    cout << "请输入密码：";
    string input;
    getline(cin, input);
    system("cls");
    return (hash_password(input) == stored_hash);
}

void show_rules() {
    cout << "\n当前规则列表：" << endl;
    for (size_t i = 0; i < rules.size(); i++) {
        cout << i + 1 << ". " << rules[i].desc
             << " (" << (rules[i].delta > 0 ? "+" : "")
             << rules[i].delta << "分)" << endl;
    }
}

void add_rule() {
    Rule new_rule;
    cout << "请输入规则描述：";
    getline(cin, new_rule.desc);

    cout << "请输入规则分值（减分请加负号）：";
    string delta_str;
    getline(cin, delta_str);
    try {
        new_rule.delta = stoi(delta_str);
    } catch (...) {
        cout << "无效的分值输入！" << endl;
        return;
    }

    rules.push_back(new_rule);
    cout << "规则添加成功！" << endl;
}

void show_ranking() {
    vector<Student*> sorted;
    for (auto& student : students) {
        sorted.push_back(&student);
    }

    sort(sorted.begin(), sorted.end(), [](Student* a, Student* b) {
        if (a->score != b->score) return a->score > b->score;
        return a->id < b->id;
    });

    cout << "积分排行榜" << endl;
    cout << "排名\t学号\t姓名\t性别\t积分\t排名变动\t积分变动" << endl;
    int rank = 1;
    for (size_t i = 0; i < sorted.size(); i++) {
        if (i > 0 && sorted[i]->score != sorted[i - 1]->score) {
            rank = static_cast<int>(i + 1);
        }
        long long score_change = sorted[i]->score - sorted[i]->old_score;
        int rank_change = sorted[i]->old_rank - sorted[i]->rank;

        cout << rank << "\t"
             << sorted[i]->id << "\t"
             << (sorted[i]->name.empty() ? "未登记" : sorted[i]->name) << "\t"
             << (sorted[i]->gender.empty() ? "未知" : sorted[i]->gender) << "\t"
             << sorted[i]->score << "\t"
             << (rank_change > 0 ? "+" : "") << rank_change << "名\t"
             << (score_change >= 0 ? "+" : "") << score_change << "分" << endl;
    }
    save_students();
}

void show_students() {
    vector<Student*> sorted;
    for (auto& student : students) {
        sorted.push_back(&student);
    }

    sort(sorted.begin(), sorted.end(), [](Student* a, Student* b) {
        if (a->score != b->score) return a->score > b->score;
        return a->id < b->id;
    });

    cout << "学号\t姓名\t性别" << endl;
    for (const auto& studentPtr : sorted) {
        cout << studentPtr->id << "\t"
             << (studentPtr->name.empty() ? "未登记" : studentPtr->name) << "\t"
             << (studentPtr->gender.empty() ? "未知" : studentPtr->gender) << "\t" << endl;
    }
    save_students();
}

void togetherapply_rule() {
    if (rules.empty()) {
        cout << "当前没有规则！" << endl;
        return;
    }

    int rule_num;
    cout << "请选择要应用的规则号：";
    cin >> rule_num;
    cin.ignore();

    if (rule_num < 1 || rule_num > static_cast<int>(rules.size())) {
        cout << "无效的规则号！" << endl;
        return;
    }

    int stu_id;
    while (true) {
        cout << "\n[学号]. 修改积分 \n0. 返回\n请选择";
        cout << "请输入学生学号：";
        cin >> stu_id;
        cin.ignore();

        if (stu_id == 0) break;

        auto it = find_if(students.begin(), students.end(), [stu_id](const Student& s) {
            return s.id == stu_id;
        });

        if (it == students.end()) {
            cout << "无效的学号！" << endl;
            continue;
        }

        it->score += rules[rule_num - 1].delta;
        cout << "修改成功，当前积分：" << it->score;
        show_students();
    }
}

void apply_rule() {
    if (rules.empty()) {
        cout << "当前没有规则！" << endl;
        return;
    }

    int rule_num;
    cout << "请选择要应用的规则号：";
    cin >> rule_num;
    cin.ignore();

    if (rule_num < 1 || rule_num > static_cast<int>(rules.size())) {
        cout << "无效的规则号！" << endl;
        return;
    }

    int stu_id;
    cout << "请输入学生学号：";
    cin >> stu_id;
    cin.ignore();

    auto it = find_if(students.begin(), students.end(), [stu_id](const Student& s) {
        return s.id == stu_id;
    });

    if (it == students.end()) {
        cout << "无效的学号！" << endl;
        return;
    }

    it->score += rules[rule_num - 1].delta;
    cout << "修改成功，当前积分：" << it->score;
}

void modify_student() {
    int id;
    cout << "请输入学号：";
    cin >> id;
    cin.ignore();

    auto it = find_if(students.begin(), students.end(), [id](const Student& s) {
        return s.id == id;
    });

    if (it == students.end()) {
        cout << "无效学号！" << endl;
        return;
    }

    cout << "当前信息：\n"
         << "姓名：" << (it->name.empty() ? "未登记" : it->name) << "\n"
         << "性别：" << (it->gender.empty() ? "未知" : it->gender) << "\n"
         << "积分：" << it->score << endl;

    cout << "请输入新姓名（直接回车保留原姓名）：";
    string name;
    getline(cin, name);
    if (!name.empty()) it->name = name;

    cout << "请输入新性别（直接回车保留原性别）：";
    string gender;
    getline(cin, gender);
    if (!gender.empty()) it->gender = gender;

    cout << "请输入新积分（直接回车保留原积分）：";
    string score_str;
    getline(cin, score_str);
    if (!score_str.empty()) {
        try {
            it->score = stoll(score_str);
        } catch (...) {
            cout << "无效的积分输入！" << endl;
        }
    }

    cout << "信息修改成功！" << endl;
}

void apply_group_scores() {
    ifstream infile("group_scores.txt");
    if (!infile) {
        cout << "未找到小组加分文件，请生成小组加分文件" << endl;
        return;
    }

    string line;
    vector<int> student_ids;
    vector<int> add_scores;

    while (getline(infile, line)) {
        if (line.find("每人加分:") != string::npos) {
            size_t pos = line.find(":");
            int add_score = stoi(line.substr(pos + 1));

            if (getline(infile, line)) {
                if (line.find("成员学号:") != string::npos) {
                    size_t start = line.find(":") + 1;
                    string ids_str = line.substr(start);
                    stringstream ss(ids_str);
                    int id;

                    while (ss >> id) {
                        student_ids.push_back(id);
                        add_scores.push_back(add_score);
                    }
                }
            }
        }
    }
    infile.close();

    for (size_t i = 0; i < student_ids.size(); i++) {
        int id = student_ids[i];
        auto it = find_if(students.begin(), students.end(), [id](const Student& s) {
            return s.id == id;
        });

        if (it != students.end()) {
            it->score += add_scores[i];
            cout << "学号" << id << "(" << it->name
                 << ") 增加积分: " << add_scores[i]
                 << " 新积分: " << it->score << endl;
        }
    }

    save_students();
    cout << "小组加分成功添加！" << endl;
}

void renew_score_and_rank() {
    if (!students.empty()) {
        cout << "开始更新旧分数..." << endl;
        for (auto& student : students) {
            student.old_score = student.score;
        }
        cout << "开始更新旧排名..." << endl;
        for (auto& student : students) {
            student.old_rank = student.rank;
        }
        cout << "更新完成" << endl;
    }
}

void auto_renew_score_and_rank() {
    int renewtime = 604800; // 默认7天
    map<string, string> config = loadConfig("config.ini");
    if (config.count("renewtime")) {
        try {
            renewtime = stoi(config["renewtime"]);
        } catch (...) {}
    }

    time_t nowtime;
    time(&nowtime);
    time_t lasttime = 0;
    
    ifstream inFile("lastrenew.dat");
    if (inFile) inFile >> lasttime;
    inFile.close();
    
    if (nowtime - lasttime >= renewtime) {
        renew_score_and_rank();
    }
}

// 主程序
int main() {
    setConsoleEncoding(); // 设置控制台编码
    
    load_students();
    load_rules();
    
    auto_renew_score_and_rank();
    
    map<string, string> config = loadConfig("config.ini");

    uint64_t password_hash = 0;
    if (config.count("password")) {
        try {
            password_hash = stoull(config["password"]);
        } catch (...) {
            password_hash = hash_password("12345"); // 默认密码
        }
    } else {
        password_hash = hash_password("12345"); // 默认密码
    }
    
    cout << "\n====== 班级积分管理系统 ======" << endl;

    while (true) {
        show_ranking();
        show_rules();
        cout << "\n1. 管理规则\n2. 修改积分\n3. 修改学生信息\n4. 手动归零变化值\n5. 小组加分\n6. 设置\n7. 保存退出\n请通过键盘选择操作" << endl;
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                if (!verify_password(password_hash)) {
                    cout << "密码错误！" << endl;
                    break;
                }
                while (true) {
                    cout << "\n1. 添加规则\n2. 删除规则\n3. 返回\n请选择：";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1) add_rule();
                    else if (sub_choice == 2) {
                        show_rules();
                        cout << "请输入要删除的规则号：";
                        int num;
                        cin >> num;
                        cin.ignore();
                        if (num >= 1 && num <= static_cast<int>(rules.size())) {
                            rules.erase(rules.begin() + num - 1);
                            cout << "删除成功！" << endl;
                        } else {
                            cout << "无效的编号！" << endl;
                        }
                    } else break;
                }
                save_rules();
                break;
            }

            case 2: {
                if (!verify_password(password_hash)) {
                    cout << "密码错误！" << endl;
                    break;
                }
                show_students();
                show_rules();
                while (true) {
                    cout << "\n1. 单个修改\n2. 批量修改\n3. 返回\n请选择：";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1) {
                        apply_rule();
                    } else {
                        if (sub_choice == 2) togetherapply_rule();
                        else break;
                    }
                }
                break;
            }

            case 3: {
                if (!verify_password(password_hash)) {
                    cout << "密码错误！" << endl;
                    break;
                }
                modify_student();
                while (true) {
                    cout << "\n1. 继续修改\n2. 返回\n请选择：";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1) modify_student();
                    else break;
                }
                break;
            }

            case 4: {
                renew_score_and_rank();
                break;
            }

            case 5: {
                if (!verify_password(password_hash)) {
                    cout << "密码错误！" << endl;
                    break;
                }

                system("calculate_group_score.exe");

                ifstream testfile("group_scores.txt");
                if (testfile) {
                    testfile.close();
                    apply_group_scores();
                    }
                else cout << "未找到计算小组分数后生成的数据文件文件，无法加分" << endl;
                break;
            }
            
            case 6: {
                if (!verify_password(password_hash)) {
                    cout << "密码错误！" << endl;
                    break;
                }

                while (true) {
                    cout << "\n1. 设置密码\n2. 设置变化值归零频率\n3. 返回\n请选择：";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1) {
                        cout << "输入新密码: ";
                        string new_pass;
                        getline(cin, new_pass);
                        password_hash = hash_password(new_pass);
                        ofstream file("config.ini", ios::app);
                        file << "password=" << password_hash << "\n";
                        file.close();
                        cout << "密码已更新！" << endl;
                    }
                    else if (sub_choice == 2) {
                        cout << "输入新归零频率（单位：天。请输入纯数字）: ";
                        string new_time_day_str;
                        getline(cin, new_time_day_str);
                        int new_time_num = stoi(new_time_day_str) * 86400;
                        string new_time = to_string(new_time_num);
                        ofstream file("config.ini", ios::app);
                        file << "renewtime=" << new_time << "\n"; 
                        file.close();
                        cout << "归零频率已更新！" << endl;						 
                    }
                    else break;
                }
                break;
            }
            
            case 7: {
                save_students();
                save_rules();
                cout << "数据已保存，感谢使用！" << endl;
                return 0;
            }

            default: {
                cout << "无效的选择！" << endl;
                break;
            }
        }
    }
}