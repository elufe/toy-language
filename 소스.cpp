#include <iostream>
#include<string>
#include<fstream>
#include <stack>
#include <queue>
#include <map>

using namespace std;

stack<string> st;	//	괄호체크 스택
stack<int> cal;		//	계산 스택
queue<int> q1;		//	오류체크 큐
queue<int> q2;		//	결과저장 큐
map<string, vector<string>> FUN;	//	FUN 목록

string postfix = "";				//	prefix to postfix 
int times_check = 0;				//	times 연산 종료 체크
int checkerr = 0;					//	연산자 오류 체크

int inter(string *total);


struct node {
	string op = "";					//	연산자
	string left = "", right = "";		//	left 문자열, right 문자열
	int value;						//	연산 결과
	node *left_node, *right_node;	//	자식 노드 포인터
	node(string op, string left, string right) {
		this->op += op;
		this->left += left;
		this->right += right;
		value = 0;
	}
};

int maketree(string total, node** parent) {				//트리 생성 함수 - 오류 발생시 음수 리턴
														// total = 전체 string, parent = 생성할 노드 포인터
	int t_size, start_check = 0, check = 0, oper_type = 0;	// t_size = 전체 string 길이
														//	start_check = 전체 괄호 유무 체크 변수
														//	check = 오류 검출을 위한 변수, operator, left, right 구분 변수
														//	oper_type = 연산자 타입 저장 변수 1->minus, 2->if
	string op_new = "", left_new = "", right_new = "";	//	전체 string 파싱한뒤 저장할 string

	t_size = total.size();

	if (total[0] == '(')								//	시작문자가 '('인 경우
		start_check = 1;

	for (int i = 0 + start_check; i < t_size - start_check; i++) {	//	전체 string 탐색  시작문자가 괄호인 경우 맨 앞, 맨 뒤 괄호 제외하고
																//	탐색
		if (!(											// 알파벳, '-'기호, 숫자를 제외한 문자가 들어오면 에러 처리
			(total[i] >= 'A' && total[i] <= 'Z') ||		// 실수인 경우 '.'이 포함되어 있으므로 같이 검출 가능
			(total[i] >= 'a' && total[i] <= 'z') ||
			(total[i] >= '0' && total[i] <= '9') ||
			total[i] == '-' || total[i] == ' ' ||
			total[i] == '(' || total[i] == ')' ||
			total[i] == '/')) {
			check = -3;									// 잘못된 입력(실수, 다른 문자) -> 오류번호 -3
			return check;								// 오류번호 입력
		}

		if (total[i] == '(') {							// 스택을 이용하여 괄호 검사
			st.push("(");
		}
		else if (total[i] == ')') {
			if (st.empty()) {							// 잘못된 괄호 ")"
				check = -2;								// 오류 번호 -> -2
				return check;
			}
			st.pop();
		}

		if (total[i] == ' ' && st.empty()) {	// 공백문자 -> 괄호 스택이 비어있는 경우 op, left, right 전환
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

	if (!st.empty()) {				// 파싱이 끝났을때 괄호의 스택에 괄호가 남아있는 경우
		check = -1;					// 잘못된 괄호 "(", 오류번호 -> -1
		while (!st.empty())
			st.pop();
		return check;
	}

	if (check > 0) {								// 파싱결과 operator, left, right로 나뉜경우
		if (op_new == "TIMES") {						//	operator에 times가 포함되어 있는 경우
			op_new = "IF";								//	종료조건의 times 연산 TIMES(0 X) -> 0이므로
														//	if 연산자로 대체하여 계산
		}
		else if (op_new != "MINUS" && op_new != "IF" &&	// operator가 알맞은지 검사 
			op_new != "minus" && op_new != "if") {
			check = -5;								// operator 자리에 다른 문자가 존재, 오류번호 -> -5
			return check;
		}
		
	}
	else if (check == 0) {							// 파싱결과 숫자부분 하나만 있는 경우------------------------------------------------------------------------------------------------------------------------
		if (op_new[1] == '-') {						// '-'가 연속으로 나오는지 검사 - 두번째 문자가 '-'이면 중복----------------------------------------------------------------------------------------------------
			check = -4;								// 오류번호 -> -4
			return check;
		}
	}

	*parent = new node(op_new, left_new, right_new);	// 노드 생성

	if (check == 0) {									// 숫자일 경우
		postfix += op_new+" ";							// prefix to postfix string 저장
		(*parent)->value = stoi(op_new);				// 해당 값 value변수에 저장
		return 1;										// 정상적인 종료 1리턴
	}


	check = maketree(left_new, &((*parent)->left_node));	// left 노드 생성
	if (check < 0)											// left 노드 정상 생성 여부 검사
		return check;
	cal.push((*parent)->left_node->value);					// left 노드 결과값 스택에 저장


	check = maketree(right_new, &((*parent)->right_node));	// right 노드 생성
	if (check < 0)											// right 노드 정상 생성 여부 검사
		return check;
	cal.push((*parent)->right_node->value);					// right 노드 결과값 스택에 저장


	if (op_new[0] == 'M' || op_new[0] == 'm')				// operator 타입 검사
		oper_type = 1;										// minus -> 1
	else
		oper_type = 2;										// if -> 2

	postfix += op_new + " ";								// prefix to postfix string 저장

	if (oper_type == 1) {	// minus
		(*parent)->value = cal.top();	//	right 노드의 value 스택에서 꺼내서 현재 노드 value값 갱신
		cal.pop();
		(*parent)->value -= cal.top();	//	left 노드의 value 스택에서 꺼내서 현재 노드 value값에서 뺌
		cal.pop();
		(*parent)->value *= -1;			//	value = right - left  ->  -value = left - right
	}
	else {					// if
		(*parent)->value = cal.top();	//	right 노드의 value 스택에서 꺼내서 현재 노드 value값 갱신
		cal.pop();
		if (cal.top() <= 0) {			//	left 노드의 value 검사
			(*parent)->value = 0;		//	left 노드의 value가 0이하이면 현재 노드 value 0으로 갱신
		}
		cal.pop();
	}

	return check;						//	오류여부 확인 check 변수 리턴
}

void start() {	// 게임시작 출력 함수
	cout << "=============================\n";
	cout << "1. Define DEFUN\n";
	cout << "2. Print DEFUN\n";
	cout << "3. Interpreter\n";
	cout << "4. Exit\n";
	cout << "=============================\n";
	cout << "메뉴를 선택하세요 >> ";
}

int defun(string input) {							//	연산자 등록 함수
	int input_size = input.size();

	string oper = "", parameter = "", result = "";	//	전체 string 파싱한뒤 저장할 string

	int start_check = 0, check = 0, p_num = 0;

	if (input[0] == '(')								//	시작문자가 '('인 경우
		start_check = 1;

	for (int i = 0 + start_check; i < input_size - start_check; i++) {	//	전체 string 탐색  시작문자가 괄호인 경우 맨 앞, 맨 뒤 괄호 제외하고
																//	탐색
		if (!(											// 알파벳, '-'기호, '/'기호, 숫자를 제외한 문자가 들어오면 에러 처리
			(input[i] >= 'A' && input[i] <= 'Z') ||		// 실수인 경우 '.'이 포함되어 있으므로 같이 검출 가능
			(input[i] >= 'a' && input[i] <= 'z') ||
			(input[i] >= '0' && input[i] <= '9') ||
			input[i] == '-' || input[i] == ' ' ||
			input[i] == '(' || input[i] == ')' ||
			input[i] == '/')) {
			check = -3;									// 잘못된 입력(실수, 다른 문자) -> 오류번호 -3
			return check;								// 오류번호 입력
		}

		if (input[i] == '(') {							// 스택을 이용하여 괄호 검사
			st.push("(");
		}
		else if (input[i] == ')') {
			if (st.empty()) {							// 잘못된 괄호 ")"
				check = -2;								// 오류 번호 -> -2
				return check;
			}
			st.pop();
		}

		if (input[i] == ' ' && st.empty()) {	// 공백문자 -> 괄호 스택이 비어있는 경우 op, parameter, result 전환
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

	if (!st.empty()) {				// 파싱이 끝났을때 괄호의 스택에 괄호가 남아있는 경우
		check = -1;					// 잘못된 괄호 "(", 오류번호 -> -1
		while (!st.empty())
			st.pop();
		return check;
	}

	if (check > 0) {								// 파싱결과 operator, parameter, result로 나뉜 경우
		if (FUN.count(oper) > 0) {
			check = -6;								// 이미 존재하는 연산자
			return check;
		}
		FUN[oper].push_back(parameter);		//	해당 연산자 파라미터 개수
		FUN[oper].push_back(result);				//	해당 연산자 내용
	}
	else if (check == 0) {							
		return -7;									// 파라미터 없는 경우
	}

	return check;						//	오류여부 확인 check 변수 리턴
}

int trans(string *total) {
	int total_size = (*total).size();

	vector<string> p;
	vector<string> FUN_p;

	string oper = "", parameter = "", result = "";	//	전체 string 파싱한뒤 저장할 string

	int start_check = 0, check = 0, p_num = 0;

	if ((*total)[0] == '(')								//	시작문자가 '('인 경우
		start_check = 1;

	for (int i = 0 + start_check; i < total_size - start_check; i++) {	//	전체 string 탐색  시작문자가 괄호인 경우 맨 앞, 맨 뒤 괄호 제외하고
																//	탐색
		if (!(											// 알파벳, '-'기호, 숫자를 제외한 문자가 들어오면 에러 처리
			((*total)[i] >= 'A' && (*total)[i] <= 'Z') ||		// 실수인 경우 '.'이 포함되어 있으므로 같이 검출 가능
			((*total)[i] >= 'a' && (*total)[i] <= 'z') ||
			((*total)[i] >= '0' && (*total)[i] <= '9') ||
			(*total)[i] == '-' || (*total)[i] == ' ' ||
			(*total)[i] == '(' || (*total)[i] == ')' ||
			(*total)[i] == '/')) {
			check = -3;									// 잘못된 입력(실수, 다른 문자) -> 오류번호 -3
			return check;								// 오류번호 입력
		}

		if ((*total)[i] == '(') {							// 스택을 이용하여 괄호 검사
			st.push("(");
		}
		else if ((*total)[i] == ')') {
			if (st.empty()) {							// 잘못된 괄호 ")"
				check = -2;								// 오류 번호 -> -2
				return check;
			}
			st.pop();
		}

		if ((*total)[i] == ' ' && st.empty()) {	// 공백문자 -> 괄호 스택이 비어있는 경우 op, left, right 전환
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
	if (!st.empty()) {				// 파싱이 끝났을때 괄호의 스택에 괄호가 남아있는 경우
		check = -1;					// 잘못된 괄호 "(", 오류번호 -> -1
		while (!st.empty())
			st.pop();
		return check;
	}
	
	if (check > 0) {								// 파싱결과 operator, parameter, result로 나뉜 경우
		if (FUN.count(oper) == 0) {
			check = -10;								// 존재하지 않는 연산자
			return check;
		}
	}
	else if (check == 0) {
		return -7;									// 파라미터 없는 경우
	}
	
	if (!st.empty()) {				// 파싱이 끝났을때 괄호의 스택에 괄호가 남아있는 경우
		check = -1;					// 잘못된 괄호 "(", 오류번호 -> -1
		while (!st.empty())
			st.pop();
		return check;
	}

	if (check > 0) {								// 파싱결과 operator, parameter, result로 나뉜 경우
		if (FUN.count(oper) == 0) {
			check = -8;								// 존재하지 않는 연산자
			return check;
		}

		parameter += FUN[oper][0];
		result += FUN[oper][1];
		
	}
	else if (check == 0) {
		return -7;									// 파라미터 없는 경우
	}

	if (oper == "TIMES") {
		while (inter(&p[0])) {}
		node *head = NULL;		//	트리 head 노드 포인터
		int temp_check = 0;
		temp_check = maketree(p[0], &head);
		if (head->value == 0) {
			times_check = 1;
		}
	}


	if (parameter[0] == '(')								//	시작문자가 '('인 경우
		start_check = 1;
	total_size = parameter.size();
	check = 0;
	FUN_p.push_back("");
	for (int i = 0 + start_check; i < total_size - start_check; i++) {	//	전체 string 탐색  시작문자가 괄호인 경우 맨 앞, 맨 뒤 괄호 제외하고

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
	int menu = -1, result = 0;	//	menu = 메뉴선택 변수, result = 오류검사 변수
	string filename;		//	입력 string
	node *head = NULL;		//	트리 head 노드 포인터
	
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

	while (1) {				//	게임시작
		start();			//	초기 출력
		while (menu < 1 || menu >4) {	//	메뉴 선택 -> 1,2,3을 제외한 입력이면 다시 입력받음

			cin >> menu;
			if (menu == 1) {
				cin.ignore();			//	개행 버퍼 제거
				getline(cin, filename);
			}
			else if (menu == 2) {

			}
			else if (menu == 3) {
				cout << "실행 파일명을 입력하세요.";
				cin >> filename;
			}
			else if (menu == 4) {	//	3번 선택
				cout << "프로그램을 종료합니다.";
			}
			else {					//	잘못된 메뉴 선택
				cout << "잘못 입력하셨습니다. 다시 입력하세요 >> ";
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
					cout << "이미 존재하는 연산자입니다.\n";
				}
				else {
					cout << "잘못 입력하였습니다.\n";
				}
			}
		}
		else if (menu == 2) {				//	defun 파일 불러오기
			ifstream openfile("defun.txt");	//	파일 읽기
			string buffer;					//	파일 내용 저장할 버퍼

			while (openfile.peek() != EOF) {		//	파일 출력 및 트리 생성
				getline(openfile, buffer);			//	1줄 읽음
				defun(buffer);
				cout << buffer << "\n";				//	1줄 출력
			}
			
		}
		else if (menu == 3) {	//파일 불러오기
			ifstream openfile(filename);	//	파일 읽기
			string buffer;					//	파일 내용 저장할 버퍼

			while (!openfile) {				//	파일이 없는 경우 재입력 받음
				cout << "파일이 존재하지 않습니다. 다시 입력하세요 >>";
				cin >> filename;
				openfile.open(filename, ios::in);
			}
			cout << "파일 내용은\n";
			cout << "-----------------------------\n";

			while (openfile.peek() != EOF) {		//	파일 출력 및 트리 생성
				getline(openfile, buffer);			//	1줄 읽음
				cout << buffer << " -> ";				//	1줄 출력
				while((result = inter(&buffer)) > 0){}
				if (checkerr == 0) {
					cout << "undefined\n";
				}
				else {
					checkerr = 0;
					times_check = 0;
					cout << buffer << "\n";
					
					if(result == 0)
						result = maketree(buffer, &head);	//	트리생성 및 연산
					
					if (result == -1) {								//	1번 선택과 동일하게 오류 검사 및 오류 메세지, 결과값 출력
						cout << "\"(\"의 위치가 잘못되었습니다.\n";
					}
					else if (result == -2) {
						cout << "\")\"의 위치가 잘못되었습니다.\n";
					}
					else if (result == -3) {
						cout << "입력이 잘못되었습니다.\n";
					}
					else if (result == -4) {
						cout << "\"-\"기호가 중복되었습니다.\n";
					}
					else if (result == -5) {
						cout << "undefined\n";
					}
					else if (result == -9) {
						cout << "매개변수가 맞지 않습니다.\n";
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
			cout << "입니다.\n\n";

		}
		else if (menu == 4) {	//종료

			return 0;

		}
		menu = -1;
	}
}
