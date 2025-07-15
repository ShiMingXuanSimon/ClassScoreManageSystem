#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>
#include <vector> // 添加 vector 头文件

using namespace std;

struct Student {
    int id;
    string name;
    string gender;
    long long old_score, score;
    int old_rank, rank;
};

vector<Student> students;  // 使用 vector 存储学生

map<string, string> loadConfig(const string& filename) {
    map<string, string> config;
    ifstream file(filename);
    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != string::npos) {
                config[line.substr(0, pos)] = line.substr(pos + 1);
            }
        }
    }
    return config;
}

void load_students() {
    ifstream fin("students.dat");
    if (!fin) return;

    string line;
    while (getline(fin, line)) {
        size_t pos1 = line.find('|');
        if (pos1 == string::npos) continue;

        Student student;
        student.id = stoi(line.substr(0, pos1));

        size_t pos2 = line.find('|', pos1 + 1);
        if (pos2 == string::npos) continue;
        student.name = line.substr(pos1 + 1, pos2 - pos1 - 1);

        size_t pos3 = line.find('|', pos2 + 1);
        if (pos3 == string::npos) continue;
        student.gender = line.substr(pos2 + 1, pos3 - pos2 - 1);

        size_t pos4 = line.find('|', pos3 + 1);
        if (pos4 == string::npos) continue;
        student.old_score = stoll(line.substr(pos3 + 1, pos4 - pos3 - 1));

        size_t pos5 = line.find('|', pos4 + 1);
        if (pos5 == string::npos) continue;
        student.score = stoll(line.substr(pos4 + 1, pos5 - pos4 - 1));

        size_t pos6 = line.find('|', pos5 + 1);
        if (pos6 == string::npos) continue;
        student.old_rank = stoi(line.substr(pos5 + 1, pos6 - pos5 - 1));

        student.rank = stoi(line.substr(pos6 + 1));
        students.push_back(student);
    }
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
}

void renew_oldscore() {
    cout << "开始更新旧积分和活动值" << endl;
    for (auto& student : students) {
        student.old_score = student.score;
    }
}

void renew_oldrank() {
    cout << "开始更新旧排名和活动值" << endl;
    for (auto& student : students) {
        student.old_rank = student.rank;
    }
}

int main() {
    // 从配置读取更新间隔
    int renewtime = 604800; // 默认值
    map<string, string> config = loadConfig("config.ini");
    if (config.count("renewtime")) {
        try {
            renewtime = stoi(config["renewtime"]);
        } catch (...) {}
    }
	//检查是否需要更新 
    time_t nowtime;
    time(&nowtime);
    time_t lasttime = 0;
    
    ifstream inFile("lastrenew.dat");
    if (inFile) inFile >> lasttime;
    
    if (nowtime - lasttime >= renewtime) {
    	//开始更新 
        load_students();
        if (!students.empty()) {
            renew_oldscore();
            renew_oldrank();
            save_students();
            cout << "完成" << endl;
        }
        
        ofstream fout("lastrenew.dat");
        fout << nowtime;
    }
    
    return 0;
}
