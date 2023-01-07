#ifndef INTRECODE_H
#define INTERCODE_H

#pragma once
#include "parserTree.h"
#include "Exception.h" 
#include <string>
#include <vector>

//#include "config.h"
using namespace std;


class InterCode;

// ��Ԫʽ��
class Quadruple
{
private:
	int cur;		// ��ǰ�Ĵ����ַ
	string op;
	string left_num;
	string right_num;
	string res;
public:
	friend InterCode;
	Quadruple(const int cur, const string op, const string left_num, const string right_num, const string res);
	friend ostream& operator<<(ostream& out, const Quadruple& t);
	friend ofstream& operator<<(ofstream& out, const Quadruple& t);
};

// ���ս������
class VN
{
public:
	virtual vector<int>& getTrueList() = 0;
	virtual vector<int>& getFalseList() = 0;
	virtual vector<int>& getNextList() = 0;
	virtual int getNextquad() = 0;
};

//���ս�� E
class E : public VN
{
protected:
	string name;	// ��Ӧ������
	string value;	// ��Ӧ��ֵ��������Ϊ��ֵ��������Ϊ��������MΪquad

	vector<int> truelist;
	vector<int> falselist;
public:
	friend InterCode;
	E(string name);
	E(string name, string value);

	vector<int>& getTrueList();
	vector<int>& getFalseList();
	
	vector<int>& getNextList() { 
		return this->falselist;
	}

	int getNextquad() { return 0; }
};

// ���ս�� S
class S : public VN
{
protected:
	vector<int> nextlist;
	int nextquad;
public:
	friend InterCode;
	S(int nextquad = 0);

	vector<int>& getNextList();

	vector<int>& getTrueList() {
		return this->nextlist;
	}

	vector<int>& getFalseList() {
		return this->nextlist;
	}

	int getNextquad() { return this->nextquad; }
};

// ���ս�� M 
class M : public VN
{
protected:
	int quad;
	vector<int> null_vec;	// ��Ϊ�麯�����ز���ӿڣ�������
public:
	friend InterCode;
	M(int quad);

	int getNextquad();

	vector<int>& getTrueList() {
		return null_vec;
	}

	vector<int>& getFalseList() {
		return null_vec;
	}

	vector<int>& getNextList() {
		return null_vec;
	}
};

// ���ս�� N
class N : public VN
{
protected:
	vector<int> nextlist;
	vector<int> null_vec;	// ��Ϊ�麯�����ز���ӿڣ�������

public:
	friend InterCode;
	//N() {};
	vector<int>& getNextList();

	vector<int>& getTrueList() {
		return null_vec;
	}
	vector<int>& getFalseList() {
		return null_vec;
	}

	int getNextquad() { return 0; }
};


// �м��������
class InterCode
{
private:
	vector<Quadruple> code;
	int nextquad;				// ��ǰ��д��ַ
	vector<VN*> eleStack;		// Eջ
	vector<int> operand;		// ������ջ
	int line;					// ��ǰ��Ӧ�����������
	parserTree* pst;			// ���ô���
public:
	InterCode();

	InterCode(parserTree& pst);
	
	vector<int> merge(const vector<int>& L1,const vector<int>& L2); // �ϲ�������
	void merge(const vector<int>& L1, const vector<int>& L2, vector<int>& L3);
	void merge(vector<vector<int> >L, vector<int>& res);
	void backpatch(vector<int>& L,int quad);
	int findByAddr(int quad);
	void emit(Quadruple temp);		// 	

	// ����ʽ����
	void allExpression(const node& root,bool bool_flag = false);

	// ��������ʽ
	void operationExpression(const node& root);

	// ��������ʽ
	// void bool_expression(const node& root);
	
	// ��ֵ����ʽ
	void assignExpression(const node& root, bool bool_flag = false);

	// �������ʽ
	void calExpression(const node& root, bool bool_flag = false);

	// �߼�����ʽ
	void logicExpression(string logicOp, bool bool_flag = false);
	void andExpression(bool bool_flag = false);
	void orExpression(bool bool_flag = false);
	void notExpression(bool bool_flag = false);

	// ��ϵ����ʽ
	void relopExpression(const node& root, bool bool_flag = false);

	// �����Լ�����ʽ
	void selfChangeExpression(const node& root,bool is_front);

	// �����������
	void defineConst(const node& root);
	void defineVariable(const node& root);

	// ����ʽ���
	void expression2statment();

	void MStatement();	// ��д����ʽ��ĸ�������M
	void NStatement();	// ��д����ʽ��ĸ�������N

	// ������ -> ����ʽ
	void valNameExpression(const node& root,bool bool_flag = false);

	// ѡ�����
	void if_then_statement();
	void if_else_statement();

	// ѭ�����
	void while_do_statement();

	// �������List
	void statemmentList(const node& root);

	// ��������
	void statementBlock(const node& root);

	void genCode(const node& root,int line);

	void outputCode(const char* filename=NULL);
	
	
};
#endif // !INTRERCODE_H

