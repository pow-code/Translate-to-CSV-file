#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <set>
#include <cctype>

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::isalnum;
using std::map;
using std::list;
using std::pair;
using std::set;

// проверка на корректность введенных параметров
int check_arg(int cnt_arg, char **val_arg, string& input_file, string& output_file){
    switch (cnt_arg) {
        case 1:
            cout << "The input file entered incorrectly" << endl;
            cout << "Please specify the full path to the input file" << endl;
            return 1;
        case 2:
            input_file = val_arg[1];
            output_file = "output_file.csv";
            return 0;
        case 3:
            input_file = val_arg[1];
            output_file = val_arg[2];
            return 0;
        default:
            cout << "Too many arguments" << endl;
            return 1;
    }
}

// добавление слов в мапу и подсчет количества каждого слова
int count_word_frequency(ifstream& input_file_stream, map<string, int>& word_frequency, set<char>& delimeters){
    int cnt = 0;
    string cur_line; // переменная, где будет храниться текущая строка

    while (getline(input_file_stream, cur_line)){ // пока можем считывать строку из инпута
        string cur_word; // переменная для хранения текущего слова

        for (char c : cur_line){ // посимвольно проходимся по строке
            if (delimeters.find(c) == delimeters.end()) // если нет среди разделителей
                cur_word.push_back(c); //пушим символ в текущее слово
            else{
                if (!cur_word.empty()){ // иначе если слово не пустое
                    word_frequency[cur_word]++; // добавляем его в мапу и увеличиваем его частоту
                    cnt++;
                    cur_word.clear(); // очищаем текущее слово для следующего
                }
            }
        }
        if (!cur_word.empty()) { // проверяем последнее слово в строке
            word_frequency[cur_word]++;
            cnt++;
        }
    }
    return cnt;
}

// критерий сортировки по убыванию для метода sort
bool sort_by_frequency(const pair<string, int>& a, const pair<string, int>& b){
    return a.second > b.second;
}

int main(int argc, char *argv[]) {
    string input_file, output_file;
    int cnt; // счётчик общего количества слов

    set<char> delimeters = {',', '.', ' ', '!', '?', '\n', '\t', '\r', ';', ':', '(', ')', '{', '}', '-', '_', '*', '#', '%', '^', '$', '@'};

    if(check_arg(argc, argv, input_file, output_file) == 1) {
        return 1;
    }

    // открытие файла и проверка на то, что он открылся
    ifstream input_file_stream;
    input_file_stream.open(input_file, fstream::in);
    if(!input_file_stream.is_open()) {
        cout << "Error opening input file!" << endl;
        return 1;
    }

    map<string, int> word_frequency; // словарь для подсчёта частоты слова

    cnt = count_word_frequency(input_file_stream, word_frequency, delimeters);

    input_file_stream.close();

    list<pair<string, int>> word_frequency_list(word_frequency.begin(), word_frequency.end());  // лист из пар, копированием значений из мапы

    word_frequency_list.sort(sort_by_frequency); // сортировка по убыванию

    //открытие output файла
    ofstream output_file_stream;
    output_file_stream.open(output_file, fstream::out);


    output_file_stream << "Слово, Частота, Частота в процентах" << endl;

    // создание итератора для пробегания по листу
    auto it = word_frequency_list.begin();

    //вывод значений в файл
    while (it != word_frequency_list.end()){
        output_file_stream << it->first << "," << it->second << "," << double (it->second)/cnt * 100.0 << endl;
        it++;
    }

    output_file_stream.close();

    return 0;
}
