#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <climits>
#include <sstream>

using namespace std;

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

Student students[51];
vector<Rule> rules;

// 文件操作函数 -----------------------------------------------------------

void load_students() {
    ifstream fin("students.dat");
    if (!fin) {
        cout << "未找到学生数据文件，将创建新文件" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        size_t pos1 = line.find('|');
        if (pos1 == string::npos) {
            cout << "数据格式错误！跳过行：" << line << endl;
            continue;
        }

        int id = stoi(line.substr(0, pos1));
        students[id].id = id;

        size_t pos2 = line.find('|', pos1 + 1);
        if (pos2 == string::npos) {
            cout << "姓名字段格式错误！跳过行：" << line << endl;
            continue;
        }
        students[id].name = line.substr(pos1 + 1, pos2 - pos1 - 1);

        size_t pos3 = line.find('|', pos2 + 1);
        if (pos3 == string::npos) {
            cout << "性别字段格式错误！跳过行：" << line << endl;
            continue;
        }
        students[id].gender = line.substr(pos2 + 1, pos3 - pos2 - 1);

        size_t pos4 = line.find('|', pos3 + 1);
        if (pos4 == string::npos || pos4 + 1 > line.length()) {
            cout << "旧积分字段格式错误！跳过行：" << line << endl;
            continue;
        }
        students[id].old_score = stoll(line.substr(pos3 + 1, pos4 - pos3 - 1));

        size_t pos5 = line.find('|', pos4 + 1);
        if (pos5 == string::npos || pos5 + 1 > line.length()) {
            cout << "积分字段格式错误！跳过行：" << line << endl;
            continue;
        }
        students[id].score = stoll(line.substr(pos4 + 1, pos5 - pos4 - 1)); // 修正此处

        size_t pos6 = line.find('|', pos5 + 1);
        if (pos6 == string::npos || pos6 + 1 > line.length()) {
            cout << "旧排名字段格式错误！跳过行：" << line << endl;
            continue;
        }
        students[id].old_rank = stoi(line.substr(pos5 + 1, pos6 - pos5 - 1));

        students[id].rank = stoi(line.substr(pos6 + 1));
    }
    fin.close();
}

void save_students() {
    ofstream fout("students.dat");
    for (int i = 1; i <= 50; i++) {
        fout << students[i].id << "|"
             << students[i].name << "|"
             << students[i].gender << "|"
             << students[i].old_score << "|"
             << students[i].score << "|"
             << students[i].old_rank << "|"
             << students[i].rank << "\n";
    }
    fout.close();
}

void load_rules() {
    ifstream fin("rules.dat");
    if (!fin) {
        cout << "未找到班规文件，将创建新文件" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        size_t pos = line.find_last_of('|');
        if (pos == string::npos || pos + 1 >= line.length()) {
            cout << "无效规则格式：" << line << endl;
            continue;
        }
        Rule r;
        r.desc = line.substr(0, pos);
        try {
            r.delta = stoi(line.substr(pos + 1));
        } catch (...) {
            cout << "数值转换错误：" << line << endl;
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

// 功能函数 -----------------------------------------------------------

bool verify_password() {
    cout << "请输入口令：";
    string input;
    getline(cin, input);
	system("cls");
    return (input == "3076670390551234");
    
	
}

void show_rules() {
    cout << "\n当前班规列表：" << endl;
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

    cout << "请输入积分变动值：";
    string delta_str;
    getline(cin, delta_str);
    try {
        new_rule.delta = stoi(delta_str);
    } catch (...) {
        cout << "无效的数值输入！" << endl;
        return;
    }

    rules.push_back(new_rule);
    cout << "班规添加成功！" << endl;
}

void show_ranking() {
    cout << "让我看看！" << endl;
	Student* sorted[49];
    for (int i = 0; i < 49; i++) sorted[i] = &students[i + 1];

    sort(sorted, sorted + 49, [](Student* a, Student* b) {
        if (a->score != b->score) return a->score > b->score;
        return a->id < b->id;
    });

    cout << "\n积分排名：" << endl;
    cout << "排名\t学号\t姓名\t性别\t积分\t排名变化\t积分变化" << endl;
    int rank = 1;
    for (int i = 0; i < 49; i++) {
        if (i > 0 && sorted[i]->score != sorted[i - 1]->score) {
            rank = i + 1;
        }
        long long score_change = sorted[i]->score - sorted[i]->old_score;
        int rank_change = sorted[i]->old_rank - sorted[i]->rank;
		
        cout << rank << "\t"
             << sorted[i]->id << "\t"
             << (sorted[i]->name.empty() ? "未命名" : sorted[i]->name) << "\t"
             << (sorted[i]->gender.empty() ? "未知" : sorted[i]->gender) << "\t"
             << sorted[i]->score << "\t"
             << (rank_change > 0 ? "+" : "") << rank_change << "名\t"
             << (score_change >= 0 ? "+" : "") << score_change << "分" << endl;
    }
    save_students();
}

void show_students() {
	Student* sorted[49];
    for (int i = 0; i < 49; i++) sorted[i] = &students[i + 1];

    sort(sorted, sorted + 49, [](Student* a, Student* b) {
        if (a->score != b->score) return a->score > b->score;
        return a->id < b->id;
    });

    cout << "学号\t姓名\t性别" << endl;
    int rank = 1;
    for (int i = 0; i < 49; i++) {
        if (i > 0 && sorted[i]->score != sorted[i - 1]->score) {
            rank = i + 1;
        }
		
        cout << sorted[i]->id << "\t"
             << (sorted[i]->name.empty() ? "未命名" : sorted[i]->name) << "\t"
             << (sorted[i]->gender.empty() ? "未知" : sorted[i]->gender) << "\t" << endl;
             
    save_students();
}
}

void togetherapply_rule() {
    if (rules.empty()) {
        cout << "当前没有班规！" << endl;
        return;
    }

    int rule_num;
    cout << "请选择要应用的规则编号：";
    cin >> rule_num;
    cin.ignore();

    if (rule_num < 1 || rule_num > static_cast<int>(rules.size())) {
        cout << "无效的规则编号！" << endl;
        return;
    }
    
    int stu_id; 
    while (true) {
        for(int i = 0;1,5;i++){
		cout << "\n[学号].继续修改 \n0. 返回\n请选择：";    
        cout << "请输入学生学号（1-49）：";
        cin >> stu_id;
        cin.ignore();
        
        if(stu_id == 0) break;
        else if (stu_id < 1 || stu_id > 49) {
            cout << "无效的学号！" << endl;
            continue;
        }
        
        students[stu_id].score = students[stu_id].score + rules[rule_num - 1].delta;
        cout << "操作成功！当前积分：" << students[stu_id].score;
        show_students();
    }}
}

void apply_rule() {
    if (rules.empty()) {
        cout << "当前没有班规！" << endl;
        return;
    }

    int rule_num;
    cout << "请选择要应用的规则编号：";
    cin >> rule_num;
    cin.ignore();

    if (rule_num < 1 || rule_num > static_cast<int>(rules.size())) {
        cout << "无效的规则编号！" << endl;
        return;
    }

    int stu_id;
    cout << "请输入学生学号（1-49）：";
    cin >> stu_id;
    cin.ignore();

    if (stu_id < 1 || stu_id > 50) {
        cout << "无效的学号！" << endl;
        return;
    }

    students[stu_id].score = students[stu_id].score + rules[rule_num - 1].delta;
    cout << "操作成功！当前积分：" << students[stu_id].score;
     
}


void modify_student() {
    int id;
    cout << "请输入学号（1-49）：";
    cin >> id;
    cin.ignore();

    if (id < 1 || id > 49) {
        cout << "无效学号！" << endl;
        return;
    }

    cout << "当前信息：\n"
         << "姓名：" << (students[id].name.empty() ? "未命名" : students[id].name) << "\n"
         << "性别：" << (students[id].gender.empty() ? "未知" : students[id].gender) << "\n"
         << "积分：" << students[id].score << endl;

    cout << "请输入新姓名（直接回车保持原样）：";
    string name;
    getline(cin, name);
    if (!name.empty()) students[id].name = name;

    cout << "请输入性别（直接回车保持原样）：";
    string gender;
    getline(cin, gender);
    if (!gender.empty()) students[id].gender = gender;

    cout << "请输入新积分（直接回车保持原样）：";
    string score_str;
    getline(cin, score_str);
    if (!score_str.empty()) {
        try {
            students[id].score = stoll(score_str);
        } catch (...) {
            cout << "无效的积分输入！" << endl;
        }
    }

    cout << "信息修改成功！" << endl;
}


void apply_group_scores() {
    ifstream infile("group_scores.txt");
    if (!infile) {
        cout << "未找到小组得分文件！请先运行小组计算程序。" << endl;
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
                    stringstream ss(ids_str); // 现在可以正常使用stringstream
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
        if (id >= 1 && id <= 50) {
            students[id].score += add_scores[i];
            cout << "学生" << id << "(" << students[id].name 
                 << ") 增加积分: " << add_scores[i] 
                 << "，新积分: " << students[id].score << endl;
        }
    }

    save_students();
    cout << "小组加分已完成并保存！" << endl;
}

void renew_oldscore(){ 
	cout <<"开始更新积分变动值" <<endl	;
    for(int i=0;i<=49;i++)
	students[i].old_score = students[i].score;
	cout <<"完成\n";
	} 
	
void renew_oldrank(){ 
	cout <<"开始更新排名变动值" <<endl	;
    for(int i=0;i<=49;i++)
	students[i].old_rank = students[i].rank;
	cout <<"完成\n";
	} 

//-----------------------------------------------------------

int main() {
    for (int i = 1; i <= 50; i++) {
        students[i].id = i;
        students[i].score = 0;
        students[i].old_score = 0;
        students[i].rank = 0;
        students[i].old_rank = 0;
    }

    load_students();
    load_rules();

    cout << "\n===== 贰柒管理系统，擅自修改判死刑！！！ =====" << endl;

    while (true) {
        show_ranking();
        show_rules();
        cout << "\n1. 管理班规\n2. 应用班规\n3. 查看排名\n4. 修改学生信息\n5. 保存退出\n6. 更新进步\n7.小组计算、加分\n请选择操作：";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                if (!verify_password()) {
                    cout << "口令错误！" << endl;
                    break;
                }
                while (true) {
                    cout << "\n1. 添加班规\n2. 删除班规\n3. 返回\n请选择：";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1) add_rule();
                    else if (sub_choice == 2) {
                        show_rules();
                        cout << "输入要删除的规则编号：";
                        int num;
                        cin >> num;
                        cin.ignore();
                        if (num >= 1 && num <= static_cast<int>(rules.size())) {
                            rules.erase(rules.begin() + num - 1);
                            cout << "删除成功！" << endl;
                        } else {
                            cout << "无效编号！" << endl;
                        }
                    } else break;
                }
                save_rules();
                break;

            case 2:                                //应用班规 
                if (!verify_password()) {       //口令验证 
                    cout << "口令错误！" << endl;
                    break;
                }
                show_students();

				show_rules();
                 while (true) {
					cout << "\n1. 单个修改\n2. 批量修改\n3. 返回\n请选择：";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1)
					{
					apply_rule(); 
                    }
					else{
							if (sub_choice == 2)togetherapply_rule();
							else break;
						}
				}
				break;
                
					 

            case 3:
                show_ranking();
                break; 

            case 4:
                if (!verify_password()) {
                    cout << "口令错误！" << endl;
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

            case 5:
                save_students();
                save_rules();
                cout << "数据已保存，感谢使用！" << endl;
                return 0;
                
            case 6:
            	renew_oldscore();
            	renew_oldrank() ;
            	break;
            	
            case 7: { 
                if (!verify_password()) {
                    cout << "口令错误！" << endl;
                    break;
                }
                
                // 运行小组计算程序
                system("calculate_group_score.exe");
                
                // 检查小组计算结果文件
                ifstream testfile("group_scores.txt");
                if (testfile) {
                    testfile.close();
                    cout << "检测到小组计算结果，是否应用加分？(1=是, 0=否): ";
                    int apply_choice;
                    cin >> apply_choice;
                    cin.ignore();
                    
                    if (apply_choice == 1) {
                        apply_group_scores();
                    } else {
                        cout << "已跳过加分操作" << endl;
                    }
                } else {
                    cout << "未找到小组计算结果" << endl;
                }
                break;
            }
            
            default:
                cout << "无效的选项！" << endl;
                break;
        }
    }
}


