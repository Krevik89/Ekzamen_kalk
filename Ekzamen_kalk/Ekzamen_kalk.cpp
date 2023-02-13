#include <iostream>
#include <string>
using namespace std;

char* mainArr;				 //основная строка
string print;				 //просто строчка для красоты вывода выражения в конце
int start_bracket_index;     //левая скобка
int end_bracket_index;		 //правая скобка
int start_num_index;	     //индекс начала числа 
int end_num_index;	         //индекс последнего числа 

//действия
double Plus(double a, double b) {
	return a + b;
}
double Minus(double a, double b) {
	return a - b;
}
double Div(double a, double b) {
	return a / b;
}
double Mult(double a, double b) {
	return a * b;
}
//поиск скобок
void Bracket_Search(char* arr);
//приоритет знака
void Priority_Sign(char* arr);
//выделение подстроки в скобках
void New_Str(char* arr, int a, int b);
//поиск чисел
void Num_Search(char* arr, int index);
//конвертация сhar* в double для чисел
void Conversion_Num(char* arr, int index);
//производим арифметическое действие
void Res_Num(char* arr, int index, double a, double b);
// заменяем подстроку на итоговый результат подсчетов
void Res_Str(char* arr, char* new_str);
// заменяем строку на новую с посчитанными значениями в скобках
void Res_Bracket_Str(char* arr, char* new_str, int a, int b);
// проверка на наличие еще скобок или знаков
bool Сheck_Bracket(char* res);

int main()
{
	string s;						//чтоб не проколоться с памятью и прочим делаем строку, а не чаровый массив изначально
	cin >> s;						//ввод выражения
	print = s;						//сохраняем ввод чтоб потом вывести красиво
	mainArr = (char*)s.c_str();		//с_str конвертирует string в указатель на чаровый массив
	Bracket_Search(mainArr);		//и идём искать скобки
}

void Bracket_Search(char* arr) {
	start_bracket_index = 0;						//проинициализируем нулями пока что
	end_bracket_index = 0;
	int error = 0, count_brackets = 0;

	for (int i = 0; i < strlen(arr) + 1; i++) {
		if (arr[i] == '(') {
			start_bracket_index = i;				//находим где начальная скобка 
			count_brackets++;
		}
		if (arr[i] == ')') {
			end_bracket_index = i;					//находим где конечная скобка
			count_brackets++;
		}
	}
	if (error == 1)exit(-1);										//проверка на (( и ))
	if (count_brackets == 0) Priority_Sign(arr);					//если скобок нет сразу идём искать знаки по приоритетам
	else  New_Str(arr, ++start_bracket_index, end_bracket_index);	//если есть то идём сначала выделять подстроку в скобках делаем инкремент для выделения правильной памяти
	//например 2*(2*2) скобки будут под индексами 2 и 6,но нам нужна память только для 2*2 соответственно 3 байта. некст комент внизу
}

void New_Str(char* arr, int a, int b) {
	char* newstr = new char[b - a];					//отсюда получится что 6-3=3 байта для подстроки
	strncpy_s(newstr, b, arr + a, b - a);			//newstr это наше выражение в скобках(скопировали из основного массива)
	Priority_Sign(newstr);							//идём искать знаки по приоритетам
}

void Priority_Sign(char* arr) {
	bool priority = false;									//ставим фолс для начала для поиска знаков
	int index = 0;
	for (int i = 0; i < strlen(arr); i++) {
		if (arr[i] == '/' || arr[i] == '*') {				//ищем '/' и '*'
			priority = true;								//если нашли ставим тру и присваиваем индекс где нашли, 
			index = i;										//завержаем цикл и идём разбираться с этим действием
			break;
		}
	}
	if (!priority) {										//если не нашли '/' и '*' идём сюда и ищем индексы '-' и '+'
		for (int i = 0; i < strlen(arr); i++) {
			if (arr[i] == '-' || arr[i] == '+') {
				priority = true;
				index = i;
				break;
			}
		}
	}
	if (priority) Num_Search(arr, index);											//когда нашли знак идем искать числа
	else  Res_Bracket_Str(mainArr, arr, --start_bracket_index, end_bracket_index);
}

void Num_Search(char* arr, int index) {
	//число слева
	for (int i = index - 1; i >= 0; i--) {
		if (isdigit(arr[i]) && i >= 0 || arr[i] == '.')	start_num_index = i;        //так же учитываем что число может быть с точкой (.........|| arr[i] == '.')
		else break;
	}
	//число справа
	for (int i = index + 1; i < strlen(arr); i++) {
		if (isdigit(arr[i]) && i >= 0 || arr[i] == '.')	end_num_index = i;
		else break;
	}
	Conversion_Num(arr, index);				//идём конвертировать наши числа из строки в тип double
}

void Conversion_Num(char* arr, int index) {
	int sizeA = index - start_num_index;                           //выделяем память для первого и 2 числа каким бы большим оно не было 
	int sizeB = end_num_index - index;
	char* firstNum = new char[sizeA];
	char* secondNum = new char[sizeB];
	strncpy_s(firstNum, sizeA + 1, arr + start_num_index, sizeA);   //перевели конкретное число в массив
	strncpy_s(secondNum, sizeB + 1, arr + index + 1, sizeB);
	double left = atof(firstNum);
	double right = atof(secondNum);									//перевели в тип double и идём выполнять с ними действия
	Res_Num(arr, index, left, right);
}

void Res_Num(char* arr, int index, double a, double b) {
	double result = 0;
	switch (arr[index])
	{
	case '+':
		result = Plus(a, b);
		break;
	case '-':
		result = Minus(a, b);
		break;
	case '*':
		result = Mult(a, b);
		break;
	case '/':
		result = Div(a, b);
		break;
	default:
		break;
	}
	result = round(result * 100) / 100;    //функция round округляет число в большую сторону, если следующая цифра после запятой больше или равна 5
	string f = to_string(result);		   //to_string преобразует значение double result в string  с помощью библиотеки string
	char* n = (char*)f.c_str();			   //с_str конвертирует string в указатель на чаровый массив
	Res_Str(arr, n);
}

void Res_Str(char* arr, char* new_str) {
	int size = strlen(arr) - (end_num_index - start_num_index) + strlen(new_str);
	char* res_arr = new char[size];
	strncpy_s(res_arr, size, arr, start_num_index);
	strncat_s(res_arr, size, new_str, strlen(new_str));
	strncat_s(res_arr, size, arr + end_num_index + 1, strlen(arr));
	Priority_Sign(res_arr);                      //идём искать ещё цифры если будут
}

void Res_Bracket_Str(char* arr, char* new_str, int a, int b) {
	int size = strlen(arr) - (b - a) + strlen(new_str);     //выделяем память под получившееся выражение 2*4.000000 (4.000000 это у нас new str)

	char* res_arr = new char[size];

	if (a >= 0) strncpy_s(res_arr, size, arr, a);
	else strncpy_s(res_arr, 1, "", 0);
	strncat_s(res_arr, size + 1, new_str, strlen(new_str));
	if (b > 0 && arr[0] != '-') strncat_s(res_arr, size, arr + b + 1, strlen(arr));
	mainArr = res_arr;
	if (Сheck_Bracket(res_arr) && arr[0] != '-') Bracket_Search(res_arr);
	else {
		system("cls");
		cout << print;
		printf("=%0.2f", atof(res_arr));
	}
}

bool Сheck_Bracket(char* res) {
	for (int i = 0; i < strlen(res); i++) {
		if (res[i] == 42 || res[i] == 43 || res[i] == 45 || res[i] == 47)
			return true;
	}
	return false;
}