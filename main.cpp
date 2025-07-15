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

vector<Student> students;  // ʹ�� vector �洢ѧ����Ϣ
vector<Rule> rules;

// �ļ��������� -----------------------------------------------------------

void load_students() {
    ifstream fin("students.dat");
    if (!fin) {
        cout << "δ�ҵ�ѧ�������ļ������Զ��������ļ�" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        size_t pos1 = line.find('|');
        if (pos1 == string::npos) {
            cout << "���ݸ�ʽ�������飺" << line << "��" << endl;
            continue;
        }

        int id = stoi(line.substr(0, pos1));
        Student student;
        student.id = id;

        size_t pos2 = line.find('|', pos1 + 1);
        if (pos2 == string::npos) {
            cout << "�����ֶθ�ʽ�������飺" << line << "��" << endl;
            continue;
        }
        student.name = line.substr(pos1 + 1, pos2 - pos1 - 1);

        size_t pos3 = line.find('|', pos2 + 1);
        if (pos3 == string::npos) {
            cout << "�Ա��ֶθ�ʽ�������飺" << line << "��" << endl;
            continue;
        }
        student.gender = line.substr(pos2 + 1, pos3 - pos2 - 1);

        size_t pos4 = line.find('|', pos3 + 1);
        if (pos4 == string::npos || pos4 + 1 > line.length()) {
            cout << "�ɻ����ֶθ�ʽ�������飺" << line << "��" << endl;
            continue;
        }
        student.old_score = stoll(line.substr(pos3 + 1, pos4 - pos3 - 1));

        size_t pos5 = line.find('|', pos4 + 1);
        if (pos5 == string::npos || pos5 + 1 > line.length()) {
            cout << "�����ֶθ�ʽ�������飺" << line << "��" << endl;
            continue;
        }
        student.score = stoll(line.substr(pos4 + 1, pos5 - pos4 - 1));

        size_t pos6 = line.find('|', pos5 + 1);
        if (pos6 == string::npos || pos6 + 1 > line.length()) {
            cout << "�������ֶθ�ʽ�������飺" << line << "��" << endl;
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
        cout << "δ�ҵ������ļ������Զ��������ļ�" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        size_t pos = line.find_last_of('|');
        if (pos == string::npos || pos + 1 >= line.length()) {
            cout << "��Ч�����ʽ��" << line << "��" << endl;
            continue;
        }
        Rule r;
        r.desc = line.substr(0, pos);
        try {
            r.delta = stoi(line.substr(pos + 1));
        } catch (...) {
            cout << "��ֵת������" << line << "��" << endl;
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
    map<string, string> config;  // ����һ���յ��ֵ䣬�������ֵ��

    ifstream file(filename);     // ���Դ��ļ�
    if (file.is_open()) {        // ����ļ��������ܴ�
        string line;             // ������ÿһ�е�����
        while (getline(file, line)) {  // ���ж�ȡ�ļ�
            size_t pos = line.find('=');  // �ҵ��Ⱥŵ�λ��
            if (pos != string::npos) {    // ��������еȺ�
                string key = line.substr(0, pos);    // �Ⱥ�����Ǽ�����username��
                string value = line.substr(pos + 1); // �Ⱥ��ұ���ֵ����Alice��
                config[key] = value;      // �����ֵ�
            }
        }
        file.close();  // �ر��ļ�
    }
    return config;     // ���ؼ��غõ��ֵ�
}

uint64_t hash_password(const string& str) {
    uint64_t hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
    }
    return hash;
}

// ���Ĺ��ܺ��� -----------------------------------------------------------

bool verify_password(uint64_t stored_hash) {
    cout << "���������룺";
    string input;
    getline(cin, input);
    system("cls");
    return (hash_password(input) == stored_hash);
}

void show_rules() {
    cout << "\n��ǰ�����б�" << endl;
    for (size_t i = 0; i < rules.size(); i++) {
        cout << i + 1 << ". " << rules[i].desc
             << " (" << (rules[i].delta > 0 ? "+" : "")
             << rules[i].delta << "��)" << endl;
    }
}

void add_rule() {
    Rule new_rule;
    cout << "���������������";
    getline(cin, new_rule.desc);

    cout << "����������ֵ��������Ӹ��ţ���";
    string delta_str;
    getline(cin, delta_str);
    try {
        new_rule.delta = stoi(delta_str);
    } catch (...) {
        cout << "��Ч�ķ�ֵ���룡" << endl;
        return;
    }

    rules.push_back(new_rule);
    cout << "������ӳɹ���" << endl;
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

    cout << "�������а�" << endl;
    cout << "����\tѧ��\t����\t�Ա�\t����\t�����䶯\t���ֱ䶯" << endl;
    int rank = 1;
    for (size_t i = 0; i < sorted.size(); i++) {
        if (i > 0 && sorted[i]->score != sorted[i - 1]->score) {
            rank = static_cast<int>(i + 1);
        }
        long long score_change = sorted[i]->score - sorted[i]->old_score;
        int rank_change = sorted[i]->old_rank - sorted[i]->rank;

        cout << rank << "\t"
             << sorted[i]->id << "\t"
             << (sorted[i]->name.empty() ? "δ�Ǽ�" : sorted[i]->name) << "\t"
             << (sorted[i]->gender.empty() ? "δ֪" : sorted[i]->gender) << "\t"
             << sorted[i]->score << "\t"
             << (rank_change > 0 ? "+" : "") << rank_change << "��\t"
             << (score_change >= 0 ? "+" : "") << score_change << "��" << endl;
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

    cout << "ѧ��\t����\t�Ա�" << endl;
    for (const auto& studentPtr : sorted) {
        cout << studentPtr->id << "\t"
             << (studentPtr->name.empty() ? "δ�Ǽ�" : studentPtr->name) << "\t"
             << (studentPtr->gender.empty() ? "δ֪" : studentPtr->gender) << "\t" << endl;
    }
    save_students();
}

void togetherapply_rule() {
    if (rules.empty()) {
        cout << "��ǰû�й���" << endl;
        return;
    }

    int rule_num;
    cout << "��ѡ��ҪӦ�õĹ���ţ�";
    cin >> rule_num;
    cin.ignore();

    if (rule_num < 1 || rule_num > static_cast<int>(rules.size())) {
        cout << "��Ч�Ĺ���ţ�" << endl;
        return;
    }

    int stu_id;
    while (true) {
        cout << "\n[ѧ��]. �޸Ļ��� \n0. ����\n��ѡ��";
        cout << "������ѧ��ѧ�ţ�";
        cin >> stu_id;
        cin.ignore();

        if (stu_id == 0) break;

        auto it = find_if(students.begin(), students.end(), [stu_id](const Student& s) {
            return s.id == stu_id;
        });

        if (it == students.end()) {
            cout << "��Ч��ѧ�ţ�" << endl;
            continue;
        }

        it->score += rules[rule_num - 1].delta;
        cout << "�޸ĳɹ�����ǰ���֣�" << it->score;
        show_students();
    }
}

void apply_rule() {
    if (rules.empty()) {
        cout << "��ǰû�й���" << endl;
        return;
    }

    int rule_num;
    cout << "��ѡ��ҪӦ�õĹ���ţ�";
    cin >> rule_num;
    cin.ignore();

    if (rule_num < 1 || rule_num > static_cast<int>(rules.size())) {
        cout << "��Ч�Ĺ���ţ�" << endl;
        return;
    }

    int stu_id;
    cout << "������ѧ��ѧ�ţ�";
    cin >> stu_id;
    cin.ignore();

    auto it = find_if(students.begin(), students.end(), [stu_id](const Student& s) {
        return s.id == stu_id;
    });

    if (it == students.end()) {
        cout << "��Ч��ѧ�ţ�" << endl;
        return;
    }

    it->score += rules[rule_num - 1].delta;
    cout << "�޸ĳɹ�����ǰ���֣�" << it->score;
}

void modify_student() {
    int id;
    cout << "������ѧ�ţ�";
    cin >> id;
    cin.ignore();

    auto it = find_if(students.begin(), students.end(), [id](const Student& s) {
        return s.id == id;
    });

    if (it == students.end()) {
        cout << "��Чѧ�ţ�" << endl;
        return;
    }

    cout << "��ǰ��Ϣ��\n"
         << "������" << (it->name.empty() ? "δ�Ǽ�" : it->name) << "\n"
         << "�Ա�" << (it->gender.empty() ? "δ֪" : it->gender) << "\n"
         << "���֣�" << it->score << endl;

    cout << "��������������ֱ�ӻس�����ԭ��������";
    string name;
    getline(cin, name);
    if (!name.empty()) it->name = name;

    cout << "���������Ա�ֱ�ӻس�����ԭ�Ա𣩣�";
    string gender;
    getline(cin, gender);
    if (!gender.empty()) it->gender = gender;

    cout << "�������»��֣�ֱ�ӻس�����ԭ���֣���";
    string score_str;
    getline(cin, score_str);
    if (!score_str.empty()) {
        try {
            it->score = stoll(score_str);
        } catch (...) {
            cout << "��Ч�Ļ������룡" << endl;
        }
    }

    cout << "��Ϣ�޸ĳɹ���" << endl;
}

void apply_group_scores() {
    ifstream infile("group_scores.txt");
    if (!infile) {
        cout << "δ�ҵ�С��ӷ��ļ���������С��ӷ��ļ�" << endl;
        return;
    }

    string line;
    vector<int> student_ids;
    vector<int> add_scores;

    while (getline(infile, line)) {
        if (line.find("ÿ�˼ӷ�:") != string::npos) {
            size_t pos = line.find(":");
            int add_score = stoi(line.substr(pos + 1));

            if (getline(infile, line)) {
                if (line.find("��Աѧ��:") != string::npos) {
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
            cout << "ѧ��" << id << "(" << it->name
                 << ") ���ӻ���: " << add_scores[i]
                 << " �»���: " << it->score << endl;
        }
    }

    save_students();
    cout << "С��ӷֳɹ���ӣ�" << endl;
}


//������-----------------------------------------------------------

int main() {
    load_students();
    load_rules();
    
    system ("renew.exe");
    
    map<string, string> config = loadConfig("config.ini");

    uint64_t password_hash = 0;
    if (config.count("password")) {
        try {
            password_hash = stoull(config["password"]);
        } catch (...) {
            password_hash = hash_password("12345"); // Ĭ������
        }
    } else {
        password_hash = hash_password("12345"); // Ĭ������
    }
    
    cout << "\n===== �༶���ֹ���ϵͳ =====" << endl;

    while (true) {
        show_ranking();
        show_rules();
        cout << "\n1. �������\n2. Ӧ�ù���\n3. �޸�ѧ����Ϣ\n4. �ֶ�����仯ֵ��ÿ���ڻ��Զ�һ�Σ�\n5. С��ӷ�\n6.����\n7. �����˳�\n��ͨ������ѡ�������" << endl ;
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                // ���������ϣ����
                if (!verify_password(password_hash)) {
                    cout << "�������" << endl;
                    break;
                }
                while (true) {
                    cout << "\n1. ��ӹ���\n2. ɾ������\n3. ����\n��ѡ��";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1) add_rule();
                    else if (sub_choice == 2) {
                        show_rules();
                        cout << "������Ҫɾ���Ĺ���ţ�";
                        int num;
                        cin >> num;
                        cin.ignore();
                        if (num >= 1 && num <= static_cast<int>(rules.size())) {
                            rules.erase(rules.begin() + num - 1);
                            cout << "ɾ���ɹ���" << endl;
                        } else {
                            cout << "��Ч�ı�ţ�" << endl;
                        }
                    } else break;
                }
                save_rules();
                break;
            }

            case 2: {
                // ���������ϣ����
                if (!verify_password(password_hash)) {
                    cout << "�������" << endl;
                    break;
                }
                show_students();
                show_rules();
                while (true) {
                    cout << "\n1. �����޸�\n2. �����޸�\n3. ����\n��ѡ��";
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
                // ���������ϣ����
                if (!verify_password(password_hash)) {
                    cout << "�������" << endl;
                    break;
                }
                modify_student();
                while (true) {
                    cout << "\n1. �����޸�\n2. ����\n��ѡ��";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1) modify_student();
                    else break;
                }
                break;
            }

            case 4: {
                system ("renew.exe");
                break;
            }

            case 5: {
                // ���������ϣ����
                if (!verify_password(password_hash)) {
                    cout << "�������" << endl;
                    break;
                }

                // ����С��ӷּ������
                system("calculate_group_score.exe");

                // ���С��ӷ��ļ�
                ifstream testfile("group_scores.txt");
                if (testfile) {
                    testfile.close();
                    cout << "��⵽С��ӷ֣��Ƿ�Ӧ�üӷ֣�(1=��, 0=��): ";
                    int apply_choice;
                    cin >> apply_choice;
                    cin.ignore();

                    if (apply_choice == 1) {
                        apply_group_scores();
                    } else {
                        cout << "ȡ���ӷֲ���" << endl;
                    }
                } else {
                    cout << "δ�ҵ�С��ӷ��ļ�" << endl;
                }
                break;
            }
            
            case 6: {
                // ���������ϣ����
                if (!verify_password(password_hash)) {
                    cout << "�������" << endl;
                    break;
                }

                while (true) {
                    cout << "\n1. ��������\n2. ���ñ仯ֵ����Ƶ��\n3. ����\n��ѡ��";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1) {
                        cout << "����������: ";
                        string new_pass;
                        getline(cin, new_pass);
                        password_hash = hash_password(new_pass); // ���¹�ϣֵ
                        ofstream file("config.ini" , ios::app);
                        file << "password=" << password_hash << "\n"; // �洢��ϣ
                        file.close();
                        cout << "�����Ѹ��£�" << endl;
                    }
                    else if (sub_choice == 2) {
                        cout << "�����¹���Ƶ�ʣ���λ���졣�����봿���֣�: " << endl ;
                        string new_time_day_str;
                        getline(cin, new_time_day_str);
                        int new_time_num = stoi(new_time_day_str) * 86400 ;
                        string new_time = to_string(new_time_num) ;
                        ofstream file("config.ini" , ios::app);
                        file << "renewtime=" << new_time << "\n"; 
                        file.close();
                        cout << "�����Ѹ��£�" << endl;						 
					}
                    else break;
                }
                break;
            }
            
            case 7: {
                save_students();
                save_rules();
                cout << "�����ѱ��棬��лʹ�ã�" << endl;
                return 0;
            }

            default: {
                cout << "��Ч��ѡ��" << endl;
                break;
            }
        }
    }
}
