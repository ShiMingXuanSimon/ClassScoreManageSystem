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

// �ļ��������� -----------------------------------------------------------

void load_students() {
    ifstream fin("students.dat");
    if (!fin) {
        cout << "δ�ҵ�ѧ�������ļ������������ļ�" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        size_t pos1 = line.find('|');
        if (pos1 == string::npos) {
            cout << "���ݸ�ʽ���������У�" << line << endl;
            continue;
        }

        int id = stoi(line.substr(0, pos1));
        students[id].id = id;

        size_t pos2 = line.find('|', pos1 + 1);
        if (pos2 == string::npos) {
            cout << "�����ֶθ�ʽ���������У�" << line << endl;
            continue;
        }
        students[id].name = line.substr(pos1 + 1, pos2 - pos1 - 1);

        size_t pos3 = line.find('|', pos2 + 1);
        if (pos3 == string::npos) {
            cout << "�Ա��ֶθ�ʽ���������У�" << line << endl;
            continue;
        }
        students[id].gender = line.substr(pos2 + 1, pos3 - pos2 - 1);

        size_t pos4 = line.find('|', pos3 + 1);
        if (pos4 == string::npos || pos4 + 1 > line.length()) {
            cout << "�ɻ����ֶθ�ʽ���������У�" << line << endl;
            continue;
        }
        students[id].old_score = stoll(line.substr(pos3 + 1, pos4 - pos3 - 1));

        size_t pos5 = line.find('|', pos4 + 1);
        if (pos5 == string::npos || pos5 + 1 > line.length()) {
            cout << "�����ֶθ�ʽ���������У�" << line << endl;
            continue;
        }
        students[id].score = stoll(line.substr(pos4 + 1, pos5 - pos4 - 1)); // �����˴�

        size_t pos6 = line.find('|', pos5 + 1);
        if (pos6 == string::npos || pos6 + 1 > line.length()) {
            cout << "�������ֶθ�ʽ���������У�" << line << endl;
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
        cout << "δ�ҵ�����ļ������������ļ�" << endl;
        return;
    }

    string line;
    while (getline(fin, line)) {
        size_t pos = line.find_last_of('|');
        if (pos == string::npos || pos + 1 >= line.length()) {
            cout << "��Ч�����ʽ��" << line << endl;
            continue;
        }
        Rule r;
        r.desc = line.substr(0, pos);
        try {
            r.delta = stoi(line.substr(pos + 1));
        } catch (...) {
            cout << "��ֵת������" << line << endl;
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

// ���ܺ��� -----------------------------------------------------------

bool verify_password() {
    cout << "��������";
    string input;
    getline(cin, input);
	system("cls");
    return (input == "3076670390551234");
    
	
}

void show_rules() {
    cout << "\n��ǰ����б�" << endl;
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

    cout << "��������ֱ䶯ֵ��";
    string delta_str;
    getline(cin, delta_str);
    try {
        new_rule.delta = stoi(delta_str);
    } catch (...) {
        cout << "��Ч����ֵ���룡" << endl;
        return;
    }

    rules.push_back(new_rule);
    cout << "�����ӳɹ���" << endl;
}

void show_ranking() {
    cout << "���ҿ�����" << endl;
	Student* sorted[49];
    for (int i = 0; i < 49; i++) sorted[i] = &students[i + 1];

    sort(sorted, sorted + 49, [](Student* a, Student* b) {
        if (a->score != b->score) return a->score > b->score;
        return a->id < b->id;
    });

    cout << "\n����������" << endl;
    cout << "����\tѧ��\t����\t�Ա�\t����\t�����仯\t���ֱ仯" << endl;
    int rank = 1;
    for (int i = 0; i < 49; i++) {
        if (i > 0 && sorted[i]->score != sorted[i - 1]->score) {
            rank = i + 1;
        }
        long long score_change = sorted[i]->score - sorted[i]->old_score;
        int rank_change = sorted[i]->old_rank - sorted[i]->rank;
		
        cout << rank << "\t"
             << sorted[i]->id << "\t"
             << (sorted[i]->name.empty() ? "δ����" : sorted[i]->name) << "\t"
             << (sorted[i]->gender.empty() ? "δ֪" : sorted[i]->gender) << "\t"
             << sorted[i]->score << "\t"
             << (rank_change > 0 ? "+" : "") << rank_change << "��\t"
             << (score_change >= 0 ? "+" : "") << score_change << "��" << endl;
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

    cout << "ѧ��\t����\t�Ա�" << endl;
    int rank = 1;
    for (int i = 0; i < 49; i++) {
        if (i > 0 && sorted[i]->score != sorted[i - 1]->score) {
            rank = i + 1;
        }
		
        cout << sorted[i]->id << "\t"
             << (sorted[i]->name.empty() ? "δ����" : sorted[i]->name) << "\t"
             << (sorted[i]->gender.empty() ? "δ֪" : sorted[i]->gender) << "\t" << endl;
             
    save_students();
}
}

void togetherapply_rule() {
    if (rules.empty()) {
        cout << "��ǰû�а�棡" << endl;
        return;
    }

    int rule_num;
    cout << "��ѡ��ҪӦ�õĹ����ţ�";
    cin >> rule_num;
    cin.ignore();

    if (rule_num < 1 || rule_num > static_cast<int>(rules.size())) {
        cout << "��Ч�Ĺ����ţ�" << endl;
        return;
    }
    
    int stu_id; 
    while (true) {
        for(int i = 0;1,5;i++){
		cout << "\n[ѧ��].�����޸� \n0. ����\n��ѡ��";    
        cout << "������ѧ��ѧ�ţ�1-49����";
        cin >> stu_id;
        cin.ignore();
        
        if(stu_id == 0) break;
        else if (stu_id < 1 || stu_id > 49) {
            cout << "��Ч��ѧ�ţ�" << endl;
            continue;
        }
        
        students[stu_id].score = students[stu_id].score + rules[rule_num - 1].delta;
        cout << "�����ɹ�����ǰ���֣�" << students[stu_id].score;
        show_students();
    }}
}

void apply_rule() {
    if (rules.empty()) {
        cout << "��ǰû�а�棡" << endl;
        return;
    }

    int rule_num;
    cout << "��ѡ��ҪӦ�õĹ����ţ�";
    cin >> rule_num;
    cin.ignore();

    if (rule_num < 1 || rule_num > static_cast<int>(rules.size())) {
        cout << "��Ч�Ĺ����ţ�" << endl;
        return;
    }

    int stu_id;
    cout << "������ѧ��ѧ�ţ�1-49����";
    cin >> stu_id;
    cin.ignore();

    if (stu_id < 1 || stu_id > 50) {
        cout << "��Ч��ѧ�ţ�" << endl;
        return;
    }

    students[stu_id].score = students[stu_id].score + rules[rule_num - 1].delta;
    cout << "�����ɹ�����ǰ���֣�" << students[stu_id].score;
     
}


void modify_student() {
    int id;
    cout << "������ѧ�ţ�1-49����";
    cin >> id;
    cin.ignore();

    if (id < 1 || id > 49) {
        cout << "��Чѧ�ţ�" << endl;
        return;
    }

    cout << "��ǰ��Ϣ��\n"
         << "������" << (students[id].name.empty() ? "δ����" : students[id].name) << "\n"
         << "�Ա�" << (students[id].gender.empty() ? "δ֪" : students[id].gender) << "\n"
         << "���֣�" << students[id].score << endl;

    cout << "��������������ֱ�ӻس�����ԭ������";
    string name;
    getline(cin, name);
    if (!name.empty()) students[id].name = name;

    cout << "�������Ա�ֱ�ӻس�����ԭ������";
    string gender;
    getline(cin, gender);
    if (!gender.empty()) students[id].gender = gender;

    cout << "�������»��֣�ֱ�ӻس�����ԭ������";
    string score_str;
    getline(cin, score_str);
    if (!score_str.empty()) {
        try {
            students[id].score = stoll(score_str);
        } catch (...) {
            cout << "��Ч�Ļ������룡" << endl;
        }
    }

    cout << "��Ϣ�޸ĳɹ���" << endl;
}


void apply_group_scores() {
    ifstream infile("group_scores.txt");
    if (!infile) {
        cout << "δ�ҵ�С��÷��ļ�����������С��������" << endl;
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
                    stringstream ss(ids_str); // ���ڿ�������ʹ��stringstream
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
            cout << "ѧ��" << id << "(" << students[id].name 
                 << ") ���ӻ���: " << add_scores[i] 
                 << "���»���: " << students[id].score << endl;
        }
    }

    save_students();
    cout << "С��ӷ�����ɲ����棡" << endl;
}

void renew_oldscore(){ 
	cout <<"��ʼ���»��ֱ䶯ֵ" <<endl	;
    for(int i=0;i<=49;i++)
	students[i].old_score = students[i].score;
	cout <<"���\n";
	} 
	
void renew_oldrank(){ 
	cout <<"��ʼ���������䶯ֵ" <<endl	;
    for(int i=0;i<=49;i++)
	students[i].old_rank = students[i].rank;
	cout <<"���\n";
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

    cout << "\n===== �������ϵͳ�������޸������̣����� =====" << endl;

    while (true) {
        show_ranking();
        show_rules();
        cout << "\n1. ������\n2. Ӧ�ð��\n3. �鿴����\n4. �޸�ѧ����Ϣ\n5. �����˳�\n6. ���½���\n7.С����㡢�ӷ�\n��ѡ�������";
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                if (!verify_password()) {
                    cout << "�������" << endl;
                    break;
                }
                while (true) {
                    cout << "\n1. ��Ӱ��\n2. ɾ�����\n3. ����\n��ѡ��";
                    int sub_choice;
                    cin >> sub_choice;
                    cin.ignore();
                    if (sub_choice == 1) add_rule();
                    else if (sub_choice == 2) {
                        show_rules();
                        cout << "����Ҫɾ���Ĺ����ţ�";
                        int num;
                        cin >> num;
                        cin.ignore();
                        if (num >= 1 && num <= static_cast<int>(rules.size())) {
                            rules.erase(rules.begin() + num - 1);
                            cout << "ɾ���ɹ���" << endl;
                        } else {
                            cout << "��Ч��ţ�" << endl;
                        }
                    } else break;
                }
                save_rules();
                break;

            case 2:                                //Ӧ�ð�� 
                if (!verify_password()) {       //������֤ 
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

            case 5:
                save_students();
                save_rules();
                cout << "�����ѱ��棬��лʹ�ã�" << endl;
                return 0;
                
            case 6:
            	renew_oldscore();
            	renew_oldrank() ;
            	break;
            	
            case 7: { 
                if (!verify_password()) {
                    cout << "�������" << endl;
                    break;
                }
                
                // ����С��������
                system("calculate_group_score.exe");
                
                // ���С��������ļ�
                ifstream testfile("group_scores.txt");
                if (testfile) {
                    testfile.close();
                    cout << "��⵽С����������Ƿ�Ӧ�üӷ֣�(1=��, 0=��): ";
                    int apply_choice;
                    cin >> apply_choice;
                    cin.ignore();
                    
                    if (apply_choice == 1) {
                        apply_group_scores();
                    } else {
                        cout << "�������ӷֲ���" << endl;
                    }
                } else {
                    cout << "δ�ҵ�С�������" << endl;
                }
                break;
            }
            
            default:
                cout << "��Ч��ѡ�" << endl;
                break;
        }
    }
}


