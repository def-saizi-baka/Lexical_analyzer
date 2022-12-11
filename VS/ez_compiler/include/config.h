#pragma once
#include <string>
#include <map>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "const.h"
#include <iostream>
using namespace std;


struct token{
	int line = 0;
	string value = "";
	int symbol = -1;
};

struct regex_exp{
	string regex;
	int endType = -1;
	bool raw = false;
} ;

class config
{
public:
	config();
	~config();
	void init();
	void log(string str);
	const map<string, int>& get_stop_symbols()const;
	const map<string, int>& get_unstop_symbols()const;
	const map<string, int>& get_keywords()const;
	const map<string, int>& get_operators()const;
	const map<string, int>& get_delimiters()const;
	const vector<regex_exp>& get_regex() const;
	string get_name(int value);
	const vector<pair<int, vector<int>>>& get_grammar()const { return this->grammar; };
	const map<string, int>& get_symbols() const { return this->dic_symbols.symbols; };
	map<int, string>& get__symbols(){ return this->dic_symbols._symbols; };

	string path_keyword		= "config\\keywords.txt";
	string path_operator	= "operator_symbol.txt";
	string path_delimiter	= "delimiter.txt";
	string path_unstop		= "unstop.txt";
	string log_path			= "parser.log";
	string grammar_path		= "grammar.txt";
private:
	struct {
		map<string, int> stop_symbols;
		map<string, int> unstop_symbols;
		map<string, int> keywords;
		map<string, int> operators;
		map<string, int> delimiters;
		map<string, int> symbols;
		map<int, string> _symbols;
		string end = Config::end; //������Լ����ֹ��
	}dic_symbols; //�����ֵ䣬��ÿһ������ӳ�䵽һ��intֵ 
	vector<pair<int, vector<int>>> grammar;//�����ķ���
	ofstream logFile;
	vector<regex_exp> regexList;
};


static void get_phases_list(vector<int>& res, config& con, string input)
{
	size_t i, j;
	bool flag = false;
	for (i = j = 0; j < input.size(); j++) {
		if (input[j] == '<' && flag == false) {
			i = j;
			flag = true;
		}
		else if (input[j] == '>' && flag == true) {
			flag = false;
			if (j < i + 2)
				continue;
			else {
				string temp = input.substr(i + 1, j - i - 1);//�õ�״̬��
				temp.erase(0, temp.find_first_not_of(" "));
				temp.erase(temp.find_last_not_of(" ") + 1);
				//������Щ�����Ǳ���ȥ���ո�
				while(true){
					int t = temp.find("large");
					if(t != string::npos){
						temp = temp.substr(t + 5);
						temp = ">" + temp;	
					}
					else
						break;
				}
				auto iter_stop = con.get_stop_symbols().find(temp);
				if (iter_stop != con.get_stop_symbols().end())
					res.push_back(iter_stop->second);
				else {
					auto iter_unstop = con.get_unstop_symbols().find(temp);
					if (iter_unstop != con.get_unstop_symbols().end())
						res.push_back(iter_unstop->second);
					else {
						con.log(string("[ERROR] û�����״̬ ") + temp);
					}
				}
			}
		}
	}
};
