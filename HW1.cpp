// SP HW1 Lexical Analysis
// ��T�T�A 10827234 �^�ۺ�
//
// �u�ݿ�J�ɦW(�]�t���ɦW) �Y�i���� �A
// ��X���G��  "SIC_output.txt"
// �t�~��X table (5-7) .table ,

#include <iostream> // cout, endl
#include <fstream>	// open, is_open, close, write, ignore
#include <string>
#include <string.h>	// string, find_last_of, substr, clear
#include <cstdlib>	// atoi, system
#include <vector> 	// vector, push_back
#include "memory.h"
#include <algorithm>

#define HASHSIZE 100

using namespace std;

struct Index {
	int table = 0;
	int location = 0; // ��1 �}�l�p��
};

typedef struct hT {
	string word;
	int hashValue = 0;
	bool haveData = false;
}hashTable;

class Tokens {
public:
	vector<string> variables;
	vector<Index> allIndex;

	void initialize();
};

typedef class Table {
private:
	vector<string> table1_Instruction;
	vector<string> table2_Pseudo;
	vector<string> table3_Register;
	vector<string> table4_Delimiter;
	vector<hT> table5_Symbol;
	vector<hT> table6_Num;
	vector<hT> table7_String;

public:
	Table() {
		readTable("Table1.table", table1_Instruction);
		readTable("Table2.table", table2_Pseudo);
		readTable("Table3.table", table3_Register);
		readTable("Table4.table", table4_Delimiter);
	} // Ū�itable1-4

	void initialize(); // ��l�� 5-7 Table
	void readTable(string tableName, vector<string>& table);

	Index findTable(char* temp); // find table 1-7 except 4
	Index findDelimiterTable(char temp); // �� Table 4
	Index insertTable(string temp, int hashNum, int option); // insert table 5 6 7

	void output(); // �I�swriteFile
	void writeFile(vector<hT>, string);
} *Tableptr;


bool readFile(fstream& temp, vector<string>& words);
int isWhiteSpaceOrDelimiter(char ch);
string toUpper(string s);
string toLower(string s);
int calHash(char* temp);


int main() {
	fstream input, outputF;		// �ɮת���
	char temp = '\0';	// �Ȧs�@�Ӥ@�Ӧr��
	char pseudoBYTE = '\0'; // �s X �� C ( X'F1' or C'EOF' )
	char cont = 'n';	// �O�_�~������L�ɮ�
	int i = 0, j = 0;
	int option = -1, count = -1, hashNum = 0, loc = 0; // count: �p�� buffer ��m // loc: ������Xtoken��m

	vector<string> words; // �ɮת���T�����@��@��
	Tokens TokenItem;
	Tableptr TableItem = new Table();
	Index index;
	char buffer[100] = { '\0' };
	bool wasQuote = false; // ��Ū�� ' �N���]�� TRUE ,Ū��ĤG�� ' �ᰵ�@�ǨơA�A�N���]��FALSE


	cout << " # ��T�T�A 10827234 �^�ۺ� " << endl << endl;
	cout << "��������������������������������������������������������������������������" << endl;
	cout << "��  SP Homework1 : Lexical Analysis  ��" << endl;
	cout << "��������������������������������������������������������������������������" << endl;

	do {

		/* Initialize */ TokenItem.initialize(), TableItem->initialize();
		/* Initialize */ words.clear();
		/* Initialize */ loc = 0, count = -1;

		if (readFile(input, words)) { 	// read records from a file

			outputF.open("SIC_output.txt", ios::out | ios::trunc);

			for (i = 0; i < words.size(); i++) {
				outputF << words.at(i).substr(0, words.at(i).size() - 1) << endl;
				wasQuote = false;

				for (j = 0; j < words.at(i).size(); j++) {
					temp = words.at(i).at(j); // �N �@���ƪ� �� j �Ӧr�� assign �� temp

					option = isWhiteSpaceOrDelimiter(temp);
					switch (option) {
					case 0: // �s�i buffer
						buffer[++count] = temp;
						break;

					case 1: // �O Delimiter
						if (wasQuote && temp == '\'') { // ���eŪ�쪺�F��O�Ѩ�ӳ�޸� (') ���_�Ӫ�
							// insert string hash table
							if (strcmp(buffer, "\0")) { // buffer ���F��
								hashNum = calHash(buffer); // hash function

								if (pseudoBYTE == 'X')  // 16�i��Ʀr
									index = TableItem->insertTable(buffer, hashNum, 2);
								else if (pseudoBYTE == 'C')  // �r�� pseudoBYTE == 'C'
									index = TableItem->insertTable(buffer, hashNum, 3);
								else // or ��L symbol
									index = TableItem->insertTable(buffer, hashNum, 1);

								TokenItem.variables.push_back(buffer);
								TokenItem.allIndex.push_back(index);
							} // if

							index = TableItem->findDelimiterTable(temp);
							TokenItem.variables.push_back(buffer);
							TokenItem.allIndex.push_back(index);

							pseudoBYTE = '\0';
							wasQuote = false;
						} // else
						else {
							if (strcmp(buffer, "\0") != 0 && strcmp(buffer, "X") != 0 && strcmp(buffer, "C") != 0) {
								index = TableItem->findTable(buffer);
								TokenItem.variables.push_back(buffer);
								TokenItem.allIndex.push_back(index);
							} // if

							if (strcmp(buffer, "X") != 0 || strcmp(buffer, "C") != 0)
								pseudoBYTE = buffer[0];

							index = TableItem->findDelimiterTable(temp);
							TokenItem.variables.push_back(buffer);
							TokenItem.allIndex.push_back(index);


							if (temp == '.') // ����
								j = words.at(i).size(); // �᭱������Ū

							if (temp == '\'')
								wasQuote = true;
						} // else

						// �M��buffer
						count = -1;
						memset(buffer, '\0', sizeof(buffer)); // �Nbuffer ���Ҧ��줸�ܦ� '\0'
						break;

					case 2: // �O WhiteSpace
						// �I�s ��hashtable function
						if (strcmp(buffer, "\0") != 0) {
							index = TableItem->findTable(buffer);
							TokenItem.variables.push_back(buffer);
							TokenItem.allIndex.push_back(index);
						} // if

						// �M��buffer
						count = -1;
						memset(buffer, '\0', sizeof(buffer)); // �Nbuffer ���Ҧ��줸�ܦ� '\0'
						break;

					} // switch

				} // inner for

				for (loc; loc < TokenItem.allIndex.size(); loc++) { //
					outputF << "(" << TokenItem.allIndex.at(loc).table << "," << TokenItem.allIndex.at(loc).location << ")";
				} // for

				outputF << endl;
				//cout << endl;
			} // outter for

			// �O���O�J�� white space ���I�Ÿ�
			// �O���O���I�Ÿ� �O-> Ū�i�Ӫ��ন index
			//				 �_; ���� -> �� buffer �ন index �M�� buffer

			// find hash table function
			// �P�_ buffer �O�_����
			// �� index : �s�J vector (n,m)

			// �̫�L�X

		} // if

		outputF.close();
		TableItem->output(); // ��X table5-7
		cout << endl << "###### EXECUTION SUCCEED ######" << endl;
		cout << endl << "Do you want to process other files? (y/N): ";
		cin >> cont;
	} while (cont == 'y' || cont == 'Y');

	system("pause");
	return 0;
} // main

bool readFile(fstream& temp, vector<string>& words) { // read records from a file
	string fileName = "";
	do {
		cout << endl << "Input a file name (0:quit!): ";
		cin >> fileName;

		if (fileName == "0") // quit
			return false;

		temp.open(fileName, ios::in);

		if (!temp)  // �䤣���ɮ�!
			cout << endl << "### " << fileName << " does not exist! ###" << endl;
	} while (!temp);

	string  line; //�x�sŪ�J���C�@��
	while (getline(temp, line)) {//�|�۰ʧ�\n����ťh��
		line += "\n";
		words.push_back(line);
	} // while

	temp.close();
	return true;

} // readInputFile()

int isWhiteSpaceOrDelimiter(char ch) {
	if (ch == ',' || ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
		ch == ':' || ch == ';' || ch == '?' || ch == '.' || ch == '=' ||
		ch == '#' || ch == '@' || ch == '\'')
		return 1;
	else if (ch == ' ' || ch == '\t' || ch == '\n')
		return 2;
	else
		return 0;
} // isWhiteSpaceOrDelimiter()

string toUpper(string s) {
	transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return toupper(c); });
	return s;
} // toUpper(string s)

string toLower(string s) {
	transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return tolower(c); });
	return s;
} // toUpper(string s)

int calHash(char* temp) {
	int ASCIISum = 0;
	string t1(temp);

	for (int cc = 0; cc < t1.length(); cc++)  // ��X ASCII �X
		ASCIISum += temp[cc];
	return ASCIISum % HASHSIZE; // hash function
} // calHash

void Tokens::initialize() {
	this->allIndex.clear();
	this->variables.clear();
} // Tokens::initialize()

void Table::initialize() {
	this->table5_Symbol.clear();
	this->table5_Symbol.resize(HASHSIZE);
	this->table6_Num.clear();
	this->table6_Num.resize(HASHSIZE);
	this->table7_String.clear();
	this->table7_String.resize(HASHSIZE);

} // Table::initialize()

void Table::readTable(string tableName, vector<string>& table) {
	fstream input;
	input.open(tableName, ios::in);

	if (!input) { // �䤣���ɮ�!
		cout << endl << "### " << tableName << " does not exist! ###" << endl;
	} // if
	else {
		string  line; // �x�sŪ�J���@���T
		while (getline(input, line)) { // �|�۰ʧ�\n����ťh��
			table.push_back(line);
		} // while

		input.close();
	} // else
} // Table::readTable(string tableName, vector<string>& table)

Index Table::findTable(char* temp) {
	// �^��index ��m
	// 123table�j�p�g�ۦP!!!!!!
	Index index;
	string t1(temp); // convert char* to string
	string upperT = toUpper(t1); // �N��J�r���ন�j�g
	string lowerT = toLower(t1); // �N��J�r���ন�p�g
	int i = 0, ASCIISum = 0, hashNum = 0;
	bool found = false;

	if (!found) {
		for (i = 0; i < this->table1_Instruction.size(); i++) {
			if (upperT == this->table1_Instruction.at(i) || lowerT == this->table1_Instruction.at(i)) {
				index.table = 1;
				index.location = i + 1;
				found = true;
			} // if
		} // for
	} // if

	if (!found) {
		for (i = 0; i < this->table2_Pseudo.size(); i++) {
			if (upperT == this->table2_Pseudo.at(i) || lowerT == this->table2_Pseudo.at(i)) {
				index.table = 2;
				index.location = i + 1;
				found = true;
			} // if
		} // for
	} // if

	if (!found) {
		for (i = 0; i < this->table3_Register.size(); i++) {
			if (upperT == this->table3_Register.at(i) || lowerT == this->table3_Register.at(i)) {
				index.table = 3;
				index.location = i + 1;
				found = true;
			} // if
		} // for
	} // if


	if (!found) {

		hashNum = calHash(temp); // hash function

		if (temp[0] >= '0' && '9' >= temp[0])
			index = Table::insertTable(t1, hashNum, 2);// Num
		else
			index = Table::insertTable(t1, hashNum, 1); // Symbol

	} // if

	return index;
} // Table::findTable(char *temp)()

Index Table::insertTable(string temp, int hashNum, int option) {
	Index index;
	int i = 0, j = 0, location = hashNum; // count �p��񪺦�m  // hashNum ���� -1
	bool found = false;

	switch (option) {
	case 1: // Symbol
		for (i = 0; i < this->table5_Symbol.size(); i++) {
			if (temp == this->table5_Symbol.at(i).word) {
				index.table = 5;
				index.location = i;
				found = true;
			} // if
		} // for

		while (!found) {
			if (location >= HASHSIZE)
				location = 0;

			if (this->table5_Symbol.at(location).hashValue == false && location < HASHSIZE) { //����m����
				this->table5_Symbol.at(location).haveData = true;		// �o�ӳ]���D��
				this->table5_Symbol.at(location).word = temp;			// �N�r�s�J
				this->table5_Symbol.at(location).hashValue = hashNum;	// �N ���ƭȦs�J
				index.location = location;
				index.table = 5;
				break;
			} // if

			location++;
		} // while

		break;

	case 2: // Num
		for (i = 0; i < this->table6_Num.size(); i++) {
			if (temp == this->table6_Num.at(i).word) {
				index.table = 6;
				index.location = i;
				found = true;
			} // if
		} // for

		while (!found) {
			if (location >= HASHSIZE)
				location = 0;

			if (this->table6_Num.at(location).hashValue == false && location < HASHSIZE) { //����m����
				this->table6_Num.at(location).haveData = true;		// �o�ӳ]���D��
				this->table6_Num.at(location).word = temp;			// �N�r�s�J
				this->table6_Num.at(location).hashValue = hashNum;	// �N ���ƭȦs�J
				index.location = location;
				index.table = 6;
				break;
			} // if

			location++;
		} // while
		break;

	case 3: // String
		for (i = 0; i < this->table7_String.size(); i++) {
			if (temp == this->table7_String.at(i).word) {
				index.table = 7;
				index.location = i;
				found = true;
			} // if
		} // for
		while (!found) {
			if (location >= HASHSIZE)
				location = 0;

			if (this->table7_String.at(location).hashValue == false && location < HASHSIZE) { //����m����
				this->table7_String.at(location).haveData = true;		// �o�ӳ]���D��
				this->table7_String.at(location).word = temp;			// �N�r�s�J
				this->table7_String.at(location).hashValue = hashNum;	// �N ���ƭȦs�J
				index.location = location;
				index.table = 7;
				break;
			} // if

			location++;
		} // while
		break;

	default: index.location = 0;	index.table = 0;
	} // switch

	return index;
} // Table::insertTable()

Index Table::findDelimiterTable(char temp) {
	Index index;
	int i = 0;
	//cout << "table:" << temp << endl;

	for (i = 0; i < this->table4_Delimiter.size(); i++) {
		if (temp == this->table4_Delimiter.at(i).c_str()[0]) {
			index.table = 4;
			index.location = i + 1;
		} // if
	} // for

	return index;
} // Table::findDelimiterTable()

void Table::writeFile(vector<hT> temp, string fileName) {
	fstream outfile;
	outfile.open(fileName, ios::out | ios::trunc); // open a file

	if (outfile.is_open()) {
		for (int i = 0; i < temp.size(); i++) {
			outfile << temp.at(i).word << endl;
		} // for
	} // if

	outfile.close();
} // Table::writeFile(vector<hT> temp, string fileName)

void Table::output() {
	this->writeFile(this->table5_Symbol, "Table5.table");
	this->writeFile(this->table6_Num, "Table6.table");
	this->writeFile(this->table7_String, "Table7.table");
} // Table::output()
