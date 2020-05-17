#include <iostream>
#include<string>
#include<fstream>
#include <stack>
#include <queue>
#include <map>

using namespace std;

stack<string> st;	//	��ȣüũ ����
stack<int> cal;		//	��� ����
queue<int> q1;		//	����üũ ť
queue<int> q2;		//	������� ť
map<string, vector<string>> FUN;	//	FUN ���

string postfix = "";				//	prefix to postfix 
int times_check = 0;				//	times ���� ���� üũ
int checkerr = 0;					//	������ ���� üũ

int inter(string *total);


struct node {
	string op = "";					//	������
	string left = "", right = "";		//	left ���ڿ�, right ���ڿ�
	int value;						//	���� ���
	node *left_node, *right_node;	//	�ڽ� ��� ������
	node(string op, string left, string right) {
		this->op += op;
		this->left += left;
		this->right += right;
		value = 0;
	}
};

int maketree(string total, node** parent) {				//Ʈ�� ���� �Լ� - ���� �߻��� ���� ����
														// total = ��ü string, parent = ������ ��� ������
	int t_size, start_check = 0, check = 0, oper_type = 0;	// t_size = ��ü string ����
														//	start_check = ��ü ��ȣ ���� üũ ����
														//	check = ���� ������ ���� ����, operator, left, right ���� ����
														//	oper_type = ������ Ÿ�� ���� ���� 1->minus, 2->if
	string op_new = "", left_new = "", right_new = "";	//	��ü string �Ľ��ѵ� ������ string

	t_size = total.size();

	if (total[0] == '(')								//	���۹��ڰ� '('�� ���
		start_check = 1;

	for (int i = 0 + start_check; i < t_size - start_check; i++) {	//	��ü string Ž��  ���۹��ڰ� ��ȣ�� ��� �� ��, �� �� ��ȣ �����ϰ�
																//	Ž��
		if (!(											// ���ĺ�, '-'��ȣ, ���ڸ� ������ ���ڰ� ������ ���� ó��
			(total[i] >= 'A' && total[i] <= 'Z') ||		// �Ǽ��� ��� '.'�� ���ԵǾ� �����Ƿ� ���� ���� ����
			(total[i] >= 'a' && total[i] <= 'z') ||
			(total[i] >= '0' && total[i] <= '9') ||
			total[i] == '-' || total[i] == ' ' ||
			total[i] == '(' || total[i] == ')' ||
			total[i] == '/')) {
			check = -3;									// �߸��� �Է�(�Ǽ�, �ٸ� ����) -> ������ȣ -3
			return check;								// ������ȣ �Է�
		}

		if (total[i] == '(') {							// ������ �̿��Ͽ� ��ȣ �˻�
			st.push("(");
		}
		else if (total[i] == ')') {
			if (st.empty()) {							// �߸��� ��ȣ ")"
				check = -2;								// ���� ��ȣ -> -2
				return check;
			}
			st.pop();
		}

		if (total[i] == ' ' && st.empty()) {	// ���鹮�� -> ��ȣ ������ ����ִ� ��� op, left, right ��ȯ
			check++;							// 0 -> operator, 1 -> left, 2 -> right
		}

		else if (check == 0) {		// operator
			op_new += total[i];
		}
		else if (check == 1) {	// left
			left_new += total[i];
		}
		else {					// right
			right_new += total[i];
		}

	}

	if (!st.empty()) {				// �Ľ��� �������� ��ȣ�� ���ÿ� ��ȣ�� �����ִ� ���
		check = -1;					// �߸��� ��ȣ "(", ������ȣ -> -1
		while (!st.empty())
			st.pop();
		return check;
	}

	if (check > 0) {								// �Ľ̰�� operator, left, right�� �������
		if (op_new == "TIMES") {						//	operator�� times�� ���ԵǾ� �ִ� ���
			op_new = "IF";								//	���������� times ���� TIMES(0 X) -> 0�̹Ƿ�
														//	if �����ڷ� ��ü�Ͽ� ���
		}
		else if (op_new != "MINUS" && op_new != "IF" &&	// operator�� �˸����� �˻� 
			op_new != "minus" && op_new != "if") {
			check = -5;								// operator �ڸ��� �ٸ� ���ڰ� ����, ������ȣ -> -5
			return check;
		}
		
	}
	else if (check == 0) {							// �Ľ̰�� ���ںκ� �ϳ��� �ִ� ���------------------------------------------------------------------------------------------------------------------------
		if (op_new[1] == '-') {						// '-'�� �������� �������� �˻� - �ι�° ���ڰ� '-'�̸� �ߺ�----------------------------------------------------------------------------------------------------
			check = -4;								// ������ȣ -> -4
			return check;
		}
	}

	*parent = new node(op_new, left_new, right_new);	// ��� ����

	if (check == 0) {									// ������ ���
		postfix += op_new+" ";							// prefix to postfix string ����
		(*parent)->value = stoi(op_new);				// �ش� �� value������ ����
		return 1;										// �������� ���� 1����
	}


	check = maketree(left_new, &((*parent)->left_node));	// left ��� ����
	if (check < 0)											// left ��� ���� ���� ���� �˻�
		return check;
	cal.push((*parent)->left_node->value);					// left ��� ����� ���ÿ� ����


	check = maketree(right_new, &((*parent)->right_node));	// right ��� ����
	if (check < 0)											// right ��� ���� ���� ���� �˻�
		return check;
	cal.push((*parent)->right_node->value);					// right ��� ����� ���ÿ� ����


	if (op_new[0] == 'M' || op_new[0] == 'm')				// operator Ÿ�� �˻�
		oper_type = 1;										// minus -> 1
	else
		oper_type = 2;										// if -> 2

	postfix += op_new + " ";								// prefix to postfix string ����

	if (oper_type == 1) {	// minus
		(*parent)->value = cal.top();	//	right ����� value ���ÿ��� ������ ���� ��� value�� ����
		cal.pop();
		(*parent)->value -= cal.top();	//	left ����� value ���ÿ��� ������ ���� ��� value������ ��
		cal.pop();
		(*parent)->value *= -1;			//	value = right - left  ->  -value = left - right
	}
	else {					// if
		(*parent)->value = cal.top();	//	right ����� value ���ÿ��� ������ ���� ��� value�� ����
		cal.pop();
		if (cal.top() <= 0) {			//	left ����� value �˻�
			(*parent)->value = 0;		//	left ����� value�� 0�����̸� ���� ��� value 0���� ����
		}
		cal.pop();
	}

	return check;						//	�������� Ȯ�� check ���� ����
}

void start() {	// ���ӽ��� ��� �Լ�
	cout << "=============================\n";
	cout << "1. Define DEFUN\n";
	cout << "2. Print DEFUN\n";
	cout << "3. Interpreter\n";
	cout << "4. Exit\n";
	cout << "=============================\n";
	cout << "�޴��� �����ϼ��� >> ";
}

int defun(string input) {							//	������ ��� �Լ�
	int input_size = input.size();

	string oper = "", parameter = "", result = "";	//	��ü string �Ľ��ѵ� ������ string

	int start_check = 0, check = 0, p_num = 0;

	if (input[0] == '(')								//	���۹��ڰ� '('�� ���
		start_check = 1;

	for (int i = 0 + start_check; i < input_size - start_check; i++) {	//	��ü string Ž��  ���۹��ڰ� ��ȣ�� ��� �� ��, �� �� ��ȣ �����ϰ�
																//	Ž��
		if (!(											// ���ĺ�, '-'��ȣ, '/'��ȣ, ���ڸ� ������ ���ڰ� ������ ���� ó��
			(input[i] >= 'A' && input[i] <= 'Z') ||		// �Ǽ��� ��� '.'�� ���ԵǾ� �����Ƿ� ���� ���� ����
			(input[i] >= 'a' && input[i] <= 'z') ||
			(input[i] >= '0' && input[i] <= '9') ||
			input[i] == '-' || input[i] == ' ' ||
			input[i] == '(' || input[i] == ')' ||
			input[i] == '/')) {
			check = -3;									// �߸��� �Է�(�Ǽ�, �ٸ� ����) -> ������ȣ -3
			return check;								// ������ȣ �Է�
		}

		if (input[i] == '(') {							// ������ �̿��Ͽ� ��ȣ �˻�
			st.push("(");
		}
		else if (input[i] == ')') {
			if (st.empty()) {							// �߸��� ��ȣ ")"
				check = -2;								// ���� ��ȣ -> -2
				return check;
			}
			st.pop();
		}

		if (input[i] == ' ' && st.empty()) {	// ���鹮�� -> ��ȣ ������ ����ִ� ��� op, parameter, result ��ȯ
			check++;							// 0 -> operator, 1 -> parameter, 2 -> result
		}

		else if (check == 0) {		// operator
			oper += input[i];
		}
		else if (check == 1) {		//	parameter
			parameter += input[i];
		}
		else {						//	result
			result += input[i];
		}

	}

	if (!st.empty()) {				// �Ľ��� �������� ��ȣ�� ���ÿ� ��ȣ�� �����ִ� ���
		check = -1;					// �߸��� ��ȣ "(", ������ȣ -> -1
		while (!st.empty())
			st.pop();
		return check;
	}

	if (check > 0) {								// �Ľ̰�� operator, parameter, result�� ���� ���
		if (FUN.count(oper) > 0) {
			check = -6;								// �̹� �����ϴ� ������
			return check;
		}
		FUN[oper].push_back(parameter);		//	�ش� ������ �Ķ���� ����
		FUN[oper].push_back(result);				//	�ش� ������ ����
	}
	else if (check == 0) {							
		return -7;									// �Ķ���� ���� ���
	}

	return check;						//	�������� Ȯ�� check ���� ����
}

int trans(string *total) {
	int total_size = (*total).size();

	vector<string> p;
	vector<string> FUN_p;

	string oper = "", parameter = "", result = "";	//	��ü string �Ľ��ѵ� ������ string

	int start_check = 0, check = 0, p_num = 0;

	if ((*total)[0] == '(')								//	���۹��ڰ� '('�� ���
		start_check = 1;

	for (int i = 0 + start_check; i < total_size - start_check; i++) {	//	��ü string Ž��  ���۹��ڰ� ��ȣ�� ��� �� ��, �� �� ��ȣ �����ϰ�
																//	Ž��
		if (!(											// ���ĺ�, '-'��ȣ, ���ڸ� ������ ���ڰ� ������ ���� ó��
			((*total)[i] >= 'A' && (*total)[i] <= 'Z') ||		// �Ǽ��� ��� '.'�� ���ԵǾ� �����Ƿ� ���� ���� ����
			((*total)[i] >= 'a' && (*total)[i] <= 'z') ||
			((*total)[i] >= '0' && (*total)[i] <= '9') ||
			(*total)[i] == '-' || (*total)[i] == ' ' ||
			(*total)[i] == '(' || (*total)[i] == ')' ||
			(*total)[i] == '/')) {
			check = -3;									// �߸��� �Է�(�Ǽ�, �ٸ� ����) -> ������ȣ -3
			return check;								// ������ȣ �Է�
		}

		if ((*total)[i] == '(') {							// ������ �̿��Ͽ� ��ȣ �˻�
			st.push("(");
		}
		else if ((*total)[i] == ')') {
			if (st.empty()) {							// �߸��� ��ȣ ")"
				check = -2;								// ���� ��ȣ -> -2
				return check;
			}
			st.pop();
		}

		if ((*total)[i] == ' ' && st.empty()) {	// ���鹮�� -> ��ȣ ������ ����ִ� ��� op, left, right ��ȯ
			if(p.size()>0 && p[p.size()-1] == "")
				continue;
			check++;
			p.push_back("");
		}

		else if (check == 0) {		// operator
			oper += (*total)[i];
		}
		else {	// left
			p[check - 1] += (*total)[i];
		}
	}
	if (!st.empty()) {				// �Ľ��� �������� ��ȣ�� ���ÿ� ��ȣ�� �����ִ� ���
		check = -1;					// �߸��� ��ȣ "(", ������ȣ -> -1
		while (!st.empty())
			st.pop();
		return check;
	}
	
	if (check > 0) {								// �Ľ̰�� operator, parameter, result�� ���� ���
		if (FUN.count(oper) == 0) {
			check = -10;								// �������� �ʴ� ������
			return check;
		}
	}
	else if (check == 0) {
		return -7;									// �Ķ���� ���� ���
	}
	
	if (!st.empty()) {				// �Ľ��� �������� ��ȣ�� ���ÿ� ��ȣ�� �����ִ� ���
		check = -1;					// �߸��� ��ȣ "(", ������ȣ -> -1
		while (!st.empty())
			st.pop();
		return check;
	}

	if (check > 0) {								// �Ľ̰�� operator, parameter, result�� ���� ���
		if (FUN.count(oper) == 0) {
			check = -8;								// �������� �ʴ� ������
			return check;
		}

		parameter += FUN[oper][0];
		result += FUN[oper][1];
		
	}
	else if (check == 0) {
		return -7;									// �Ķ���� ���� ���
	}

	if (oper == "TIMES") {
		while (inter(&p[0])) {}
		node *head = NULL;		//	Ʈ�� head ��� ������
		int temp_check = 0;
		temp_check = maketree(p[0], &head);
		if (head->value == 0) {
			times_check = 1;
		}
	}


	if (parameter[0] == '(')								//	���۹��ڰ� '('�� ���
		start_check = 1;
	total_size = parameter.size();
	check = 0;
	FUN_p.push_back("");
	for (int i = 0 + start_check; i < total_size - start_check; i++) {	//	��ü string Ž��  ���۹��ڰ� ��ȣ�� ��� �� ��, �� �� ��ȣ �����ϰ�

		if (parameter[i] == ' ') {
			if (FUN_p[0] == "") {
				continue;
			}
			else {
				check++;
				FUN_p.push_back("");
				continue;
			}
		}

		FUN_p[check] += parameter[i];
	}
	if (p.size() != FUN_p.size()) {
		check = -9;
		return check;
	}
	int index = -1;
	for (int i = 0; i < FUN_p.size(); i++) {
		while ((index = result.find(FUN_p[i])) > -1) {
			result.replace(index, FUN_p[i].size(), p[i]);
		}
	}
	*total = result;

	return check;
}

int inter(string *total) {
	map<string, vector<string>>::iterator iter;
	string temp="";
	int index = -1, i = 0, check = 0, check2 = 0;
	for (iter = FUN.begin(); iter != FUN.end(); iter++) {
		
		while ((index = (*total).find(iter->first)) > -1) {
			checkerr = 1;
			if (iter->first == "IF") {
				if ((*total)[index + 2] == '/')
					break;
			}
			if (iter->first == "TIMES" && times_check) {
				break;
			}

			/*if (iter->first == "TIMES" && times_check) {
				return 1;
			}*/
			check++;
			i = index;
			while ((*total)[i] != ')' || (!st.empty())) {
				if ((*total)[i] == '(')
					st.push("(");
				else if ((*total)[i] == ')' && (!st.empty()))
					st.pop();
				temp += (*total)[i];
				i++;
				
			}
			check2 = trans(&temp);
			if (check2 < 0)
				return check2;
			if (iter->first == "MINUS" || iter->first == "minus" || iter->first == "IF" || iter->first == "if") {
				check = 0;
				temp.clear();
				break;
			}

			i = i - index;
			if (index > 0 && (*total)[index - 1] == '(') {
				(*total).replace(index - 1, i + 2, temp);
			}
			else {
				(*total).replace(index, i, temp);
			}
			temp.clear();
			i = 0;
			return check;
		}
		temp.clear();
	}
	return check;
}


int main() {
	int menu = -1, result = 0;	//	menu = �޴����� ����, result = �����˻� ����
	string filename;		//	�Է� string
	node *head = NULL;		//	Ʈ�� head ��� ������
	
	FUN["MINUS"].push_back("X Y");
	FUN["MINUS"].push_back("MINUS X Y");
	FUN["IF"].push_back("X Y");
	FUN["IF"].push_back("IF X Y");
	FUN["minus"].push_back("X Y");
	FUN["minus"].push_back("minus X Y");
	FUN["if"].push_back("X Y");
	FUN["if"].push_back("if X Y");

	//string temp = "ADD 1 3";
	//defun("ADD (X Y) (MINUS X (MINUS 0 Y))");
	//trans(&temp);
	//cout << temp;
	//string temp = "(MINUS (ADD (IF 1 3) 1) (ADD 2 3))";
	//inter(&temp);
	//cout << temp;

	while (1) {				//	���ӽ���
		start();			//	�ʱ� ���
		while (menu < 1 || menu >4) {	//	�޴� ���� -> 1,2,3�� ������ �Է��̸� �ٽ� �Է¹���

			cin >> menu;
			if (menu == 1) {
				cin.ignore();			//	���� ���� ����
				getline(cin, filename);
			}
			else if (menu == 2) {

			}
			else if (menu == 3) {
				cout << "���� ���ϸ��� �Է��ϼ���.";
				cin >> filename;
			}
			else if (menu == 4) {	//	3�� ����
				cout << "���α׷��� �����մϴ�.";
			}
			else {					//	�߸��� �޴� ����
				cout << "�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է��ϼ��� >> ";
			}
		}
		if (menu == 1) {	//	defun
			if ((result = defun(filename)) > 0) {
				ofstream out("defun.txt", std::ios::app);
				if (out.is_open()) {
					out <<"\n"<< filename;
				}
			}
			else {
				if (result == -6) {
					cout << "�̹� �����ϴ� �������Դϴ�.\n";
				}
				else {
					cout << "�߸� �Է��Ͽ����ϴ�.\n";
				}
			}
		}
		else if (menu == 2) {				//	defun ���� �ҷ�����
			ifstream openfile("defun.txt");	//	���� �б�
			string buffer;					//	���� ���� ������ ����

			while (openfile.peek() != EOF) {		//	���� ��� �� Ʈ�� ����
				getline(openfile, buffer);			//	1�� ����
				defun(buffer);
				cout << buffer << "\n";				//	1�� ���
			}
			
		}
		else if (menu == 3) {	//���� �ҷ�����
			ifstream openfile(filename);	//	���� �б�
			string buffer;					//	���� ���� ������ ����

			while (!openfile) {				//	������ ���� ��� ���Է� ����
				cout << "������ �������� �ʽ��ϴ�. �ٽ� �Է��ϼ��� >>";
				cin >> filename;
				openfile.open(filename, ios::in);
			}
			cout << "���� ������\n";
			cout << "-----------------------------\n";

			while (openfile.peek() != EOF) {		//	���� ��� �� Ʈ�� ����
				getline(openfile, buffer);			//	1�� ����
				cout << buffer << " -> ";				//	1�� ���
				while((result = inter(&buffer)) > 0){}
				if (checkerr == 0) {
					cout << "undefined\n";
				}
				else {
					checkerr = 0;
					times_check = 0;
					cout << buffer << "\n";
					
					if(result == 0)
						result = maketree(buffer, &head);	//	Ʈ������ �� ����
					
					if (result == -1) {								//	1�� ���ð� �����ϰ� ���� �˻� �� ���� �޼���, ����� ���
						cout << "\"(\"�� ��ġ�� �߸��Ǿ����ϴ�.\n";
					}
					else if (result == -2) {
						cout << "\")\"�� ��ġ�� �߸��Ǿ����ϴ�.\n";
					}
					else if (result == -3) {
						cout << "�Է��� �߸��Ǿ����ϴ�.\n";
					}
					else if (result == -4) {
						cout << "\"-\"��ȣ�� �ߺ��Ǿ����ϴ�.\n";
					}
					else if (result == -5) {
						cout << "undefined\n";
					}
					else if (result == -9) {
						cout << "�Ű������� ���� �ʽ��ϴ�.\n";
					}
					else {
						cout << "Prefix to Postfix : ";
						cout << postfix;

						cout << "\n";
						cout << "result : " << head->value << "\n";
					}
					postfix.clear();
					
				}
				

				
			}
			cout << "-----------------------------\n";
			cout << "�Դϴ�.\n\n";

		}
		else if (menu == 4) {	//����

			return 0;

		}
		menu = -1;
	}
}
