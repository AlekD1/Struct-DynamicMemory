#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <conio.h>
#include <fstream>

//--------------- описание типа инф. поля -----------------
struct marsh {
	int id; //порядковый номер
	int number; // номер маршрута
	char nap[15]; // начальный пункт
	char kop[15]; // конечный пункт
};
//--------------- описание списка -------------------------
struct my_list {
	struct marsh inf;
	struct my_list* next;
};
//-----------------прототипы функций------------------------
struct my_list* del_first(struct my_list* beg);
struct marsh read_data(int k);
struct my_list* add_first(struct my_list* beg, struct marsh z);
struct my_list* add_last(struct my_list* beg, struct marsh z);
struct my_list* create_my_list(struct my_list* beg);
struct my_list* sort(struct my_list* beg);
void print_my_list(my_list* beg);
void save_to_file(my_list* beg, int len, FILE* f);
struct my_list* get_from_file(FILE* f, my_list* beg);
//----------------основная функция-------------------------------
int main() {
	setlocale(LC_ALL, "Russian");
	struct my_list* beg, * end, * t;
	beg = end = NULL;
	int answer;
	int len = 0;
	int k = 0;
	struct marsh z;
	FILE* f;
	f = fopen("data.dat", "rb+"); // Открытие файла
	if (!f) {
		f = fopen("data.dat", "wb+");
		if (!f) {
			printf("Ошибка открытия файла");
			return 0;
		}
	}


	while (1) {
		printf("\n1. добавление в начало\n"
			"2. добавление после последнего\n"
			"3. организация списка\n"
			"4. просмотр списка\n"
			"5. удаление первого\n"
			"6. сохранение в файл\n"
			"7. загрузка с файла\n"
			"8. сортировка\n"
			"0. выход\n");
		printf("\n введите - >"), scanf("%d", &answer);
		switch (answer) {
		case 0:
			return 0;
		case 1: {
			if (beg == NULL) {
				printf("\n Добавление невозможно, так как список не создан");
				break;
			}
			else {
				z = read_data(k);
				beg = add_first(beg, z);
				break;
			}
		}
		case 2:
			if (beg == NULL) {
				printf("\n Добавление невозможно, так как список не создан");
				break;
			}
			else {
				beg = add_last(beg, read_data(k));
				break;
			}
		case 3:
			beg = create_my_list(beg);
			break;
		case 4:
			if (beg == NULL) {
				printf("\n Вывов невозможен, так как список не создан");
				break;
			}
			else {
				print_my_list(beg);
				break;
			}
		case 5:
			if (beg == NULL) {
				printf("\n Удаление невозможно, так как список не создан");
				break;
			}
			else {
				beg = del_first(beg);
				break;
			}
		case 6:
			f = fopen("data.dat", "wb+");
			save_to_file(beg, len, f);
			break;
		case 7:
			beg = get_from_file(f, beg);
			break;
		case 8:
			beg = add_last(beg, read_data(k));
			beg = sort(beg);
			break;
		default:
			printf("Команада не распознана\n");
		}
	}
}
//------------ удаление первого элемента ----------------
struct my_list* del_first(struct my_list* beg) {
	if (beg) {
		my_list* t = beg;
		beg = t->next;
		free(t);
	}
	else printf("список пуст");
	return beg;
}
//--------------- чтение данных ---------------------------
struct marsh read_data(int k) {
	struct marsh z;
	printf(" введите порядковый номер, номер маршрута, начальный и конечный пункт\n");
	scanf("%d %d %s %s", &z.id, &z.number, &z.nap, &z.kop);
	return z;
}
//----------добавление в начало списка -----------------------
struct my_list* add_first(struct my_list* beg, struct marsh z) {
	my_list* t;
	t = (struct my_list*)malloc(sizeof(struct my_list));
	t->inf = z;
	t->next = beg;
	beg = t;
	return beg;
}
//----------- добавление в конец списка ------------------------
struct my_list* add_last(struct my_list* beg, struct marsh z) {
	my_list* t, * end;
	t = (struct my_list*)malloc(sizeof(struct my_list));
	t->inf = z;
	t->next = NULL;
	if (beg == NULL)
		beg = t;
	else {
		end = beg;
		for (; end->next != NULL; end = end->next);
		end->next = t;
	}
	return beg;
}
//------------- создание списка ---------------------------------
struct my_list* create_my_list(struct my_list* beg) {
	int fl = 0;
	int k = 0;
	struct marsh z;
	do {
		if (beg == NULL) { //если список пуст, то создать список
			z = read_data(k);
			beg = add_first(beg, z);
			k++;
		}
		else { // иначе, добавить новый элемент в конец списка
			z = read_data(k);
			add_last(beg, z);
			k++;
		}
		printf("еще 1-да 0-нет");
		scanf("%d", &fl);
	} while (fl);
	return beg;
}
//------------ просмотр списка ----------------------------------
void print_my_list(my_list* beg) {
	my_list* t;
	if (beg == NULL) { // если список пуст, то вывести сообщение
		printf("список пуст");
		return;
	} // иначе вывести инф. поля элем. списка 
	puts("| Порядковый номер | Номер маршрута | Начальный пункт | Конечный пункт |");
	t = beg;
	while (t != NULL) {
		printf("| %-16d | %-15d| %-16s| %-15s|\n", t->inf.id, t->inf.number, t->inf.nap, t->inf.kop);
		t = t->next;
	}
	printf("________________________________________\n");
	return;
}
//------------ Сохранение в файл ----------------------------------
void save_to_file(my_list* beg, int len, FILE* f) {
	my_list* t;
	t = beg;
	marsh d;
	fseek(f, 0, SEEK_SET);
	while (t != NULL) {
		d = t->inf;
		fwrite(&d, sizeof(marsh), 1, f);
		t = t->next;
	}
}


//------------- Сортировка ---------------------------------
struct my_list* sort(my_list* beg) {
	struct my_list* end, * t;
	struct marsh temp;
	int len = 0;
	end = beg;
	for (; end->next != NULL; end = end->next) len++;
	for (int write = 0; write < len; write++) {
		t = beg;
		for (int sort = 0; sort < len; sort++) {
			if (t->inf.number > t->next->inf.number) {
				printf("%i", len);
				temp = t->next->inf;
				t->next->inf = t->inf;
				t->inf = temp;
			}
			t = t->next;
		}
	}
	return beg;
}

//------------ Загрузка с файла ----------------------------------
struct my_list* get_from_file(FILE* f, my_list* beg) {
	marsh d;
	int ns;
	struct my_list* t, * end, * k;
	t = (struct my_list*)malloc(sizeof(struct my_list));

	rewind(f);

	ns = fread(&d, sizeof(marsh), 1, f);

	if (beg == NULL) {
		beg = (struct my_list*)malloc(sizeof(struct my_list));
	}

	beg->inf = d;
	beg->next = NULL;

	t = beg;

	do {
		ns = fread(&d, sizeof(marsh), 1, f);

		if (ns < 1) break;

		k = (struct my_list*)malloc(sizeof(struct my_list));

		t->next = k;
		k->inf = d;
		t = k;

	} while (1);
	t->next = NULL;
	return beg;
}
