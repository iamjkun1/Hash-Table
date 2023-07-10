#include <bits/stdc++.h>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 2
#define ERROR -2

typedef int Status;


/////���Ա��ʼ����
#define INIT_SIZE 1024

////���Ա�������
#define INCREASE_SIZE 10

////�ļ�����󳤶�
#define FILE_NAME_LEN 20

/////�ַ�������󳤶�
#define MAX_STR 255

//// ����������󳤶�
#define WORD_LEN 20

#define WORD 21


/************* �ṹ�嶨�� ************/

/* ����˳�� */
typedef struct {
	char ch[MAX_STR];
	int length;
} String;

/* �������ʽṹ */
typedef struct {
	char word[INIT_SIZE];	// �洢���ʣ���������ʮ���ַ�
	int count;			// ���ʳ��ֵĴ���
} ElementType;

/* �����б� */
typedef struct {
	ElementType *et;	// ָ��洢���ʵ�ͷָ��
	int length;
	int listSize;
} SeqList;


/************* �������� ************/

///// ��ʼ�������б�
Status init_SeqList(SeqList *sq, ElementType *et);

////// ��ӵ���
Status add_SeqList(SeqList *sq, ElementType *et, char *word);

/////ͳ�Ƶ������ʸ���
Status count_SeqList(SeqList *sq, ElementType *et);

//// �����ı��ļ�
Status create_File();


/////BF��ģʽƥ���㷨
Status BF_Compare(String s1, String s2, int length);

////����ƥ�亯����
void get_next(String s2, int *next);

//// �������ʳ��ֵĸ���
Status searchWordCount();

////���������ڸ��г��ֵĴ������кš����λ��
Status searchWordLine();

//// ���������˵�
void searchWord_Menu();

//// ���˵�
Status main_Menu();

///��������
void clear();

///�����������
void continueStart();


/* ��ʼ�������б� */
Status init_SeqList(SeqList *sq, ElementType *et) {
	sq->et = et;
	sq->length = 0;
	return TRUE;
}

/* ��ӵ��� -  */
Status add_SeqList(SeqList *sq, ElementType *et, char *word) {
	int i = 0, j = 0;
	for (int i = 0; i < sq->length; i++) {
		// �����ǰ���ʺ�Ҫ����ĵ�����ͬ��ֱ��ͳ��
		if (strcmp(et[i].word, word) == 0) {
			et[i].count++;
			return OK;
		}
	}

	if (sq->length == INIT_SIZE) {
		printf("\n�ռ䲻�㣬���ʡ�%s������ʧ�ܣ�\n", word);
		continueStart();
		return FALSE;
	}

	for (j = sq->length; j > i; j--) {
		memcpy(et + j, et + j - 1, sizeof(ElementType));
	}
	sq->length++;
	strcpy(et[i].word, word);
	et[i].count = 1;

	return TRUE;
}

/* ͳ�Ƶ������ʸ��� */
Status count_SeqList(SeqList *sq, ElementType *et) {
	int i, j = 0;
	for (i = 0; i < sq->length; i++) {
		j = j + et[i].count;
	}
	return j;
}

//// �����ı��ļ�
Status create_File() {
	ElementType et;		// ���ʽṹ��
	SeqList sq;			// �����б�
	char choose;
	char fileName[FILE_NAME_LEN + 1];
	FILE *fp;			// �ļ�ָ��

	printf("������Ҫ�������ļ������ظ����ļ����ƽ������ã�����");
	scanf("%s", fileName);
	fp = fopen(fileName, "w");
	choose = 'n';
	while (choose == 'n' || choose == 'N') {
		// ��ȡ�û����������
		printf("������һ���ı���");
		gets(et.word);
		gets(et.word);
		sq.length = strlen(et.word);
		fwrite(&et, sq.length, 1, fp);	// ������д�뵽�ļ�
		// д�뻻�з�
		fprintf(fp, "%c", 10);
		printf("�Ƿ��������?(y/n)��");
		choose = getchar();
	}
	fclose(fp);
	printf("\n�����ļ��ɹ���");
	continueStart();
	return TRUE;
}


//////BF��ģʽƥ���㷨
Status BF_Compare(String s1, String s2, int length) {
	int i, j;
	i = length - 1;       /* ɨ��s1���±꣬��Ϊc�������±��Ǵ�0��ʼ������������1 */
	j = 0; /* ɨ��s2�Ŀ�ʼ�±� */
	int next[255];
	get_next(s2, next);
	while (i < s1.length && j < s2.length) {
		if (s1.ch[i] == s2.ch[j]) {
			i++;
			j++;  /* ����ʹ�±�������һ���ַ�λ�� */
		} else {
			i = i - j + 1;
			j = 0;
			//j = next[j];
		}
	}
	if (j >= s2.length)
		return i - s2.length;
	else
		return -1; /* ��ʾs1�в�����s2������-1 */
}

/////����ƥ�亯����
void get_next(String s2, int *next) {
	int i = 0;
	int j = -1;
	next[0] = -1;   //�����õ�
	s2.length = strlen(s2.ch);
	while (i < s2.length) {
		if (j == -1 || s2.ch[i] == s2.ch[j]) {
			++i;
			++j;
			next[i] = j;
		} else {
			j = next[j];
		}
	}
}

/////�������ʳ��ֵĸ���
Status searchWordCount() {
	FILE *fp;
	String s, t;	// ������������sΪĸ����tΪ�Ӵ�
	char fileName[FILE_NAME_LEN];
	int i = 0, j, k;

	printf("�����ļ�������database.txt����");
	scanf("%s", fileName);
	fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("\n�ļ���ʧ�ܣ�\n");
		continueStart();
		return FALSE;
	}

	printf("������Ҫͳ�Ƴ��ִ����ĵ��ʣ�");
	scanf("%s", t.ch);
	t.length = strlen(t.ch);

	while (!feof(fp)) {
		// ����Ĭ�� 110 ���ַ�Ϊһ��
		memset(s.ch, '\0', 110);
		fgets(s.ch, 110, fp);
		s.length = strlen(s.ch);
		// ��ʼ������λ��
		k = 0;
		// ������������ s
		while (k < s.length - 1) {
			j = BF_Compare(s, t, k);
			if (j < 0)
				break;
			else {
				i++;					// ���ʼ������ۼ�
				k = j + t.length;		// ������һ�Ӵ��ļ���
			}
		}
	}
	printf("\n���ʡ�%s�����ı��ļ� %s �У������� %d ��\n", t.ch, fileName, i);

	continueStart();
	return OK;
}

/////�������ʵ��кš����λ��
Status searchWordLine() {
	FILE *fp;
	String s, t;	// ����������
	char fileName[FILE_NAME_LEN];
	int wPos[20];	// ���һ�����ַ�ƥ��Ķ��λ��

	int i, j, k, l, m;

	printf("�����ļ�������database.txt����");
	scanf("%s", fileName);
	fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("\n�ļ���ʧ�ܣ�\n");
		continueStart();
		return FALSE;
	}

	printf("������Ҫ�����ĵ��ʣ�");
	scanf("%s", t.ch);
	t.length = strlen(t.ch);

	l = 0;	// ��ʼ������������ͳ�ƴ���λ��
	while (!feof(fp)) {
		// ɨ�������ļ�
		memset(s.ch, '\0', 110);
		fgets(s.ch, 110, fp);
		s.length = strlen(s.ch);
		l++;	// ����������
		k = 0;	// ��ʼ������λ��
		i = 0;	// ��ʼ�����ʼ�����

		// ������������s
		while (k < s.length - 1) {
			j = BF_Compare(s, t, k);	// ���ô�ƥ���㷨
			if (j < 0)
				break;
			else {
				i++;	// ���ʼ���������
				wPos[i] = j;		// ��¼����λ��
				k = j + t.length;	// ������һ�Ӵ�����
			}
		}

		if (i > 0) {
			printf("�кţ�%d��������%d����ʼλ�÷ֱ�Ϊ��\n", l, i);
			for (m = 1; m <= i; m++) {
				printf("�� %4d ���ַ�\n", wPos[m] + 1);
			}
		}
		printf("\n");
	}
	continueStart();
	return OK;
}

///////�˵�
void searchWord_Menu() {
	printf("\n----------------------------------------------------\n");
	printf("\t������������������  �˵�  ������������������\n\n");
	printf("\t   ��������������������������������������������\n");
	printf("\t   ��                    ��\n");
	printf("\t   �� ��1�����ʳ��ִ���  ��\n");
	printf("\t   ��                    ��\n");
	printf("\t   �� ��2�����ʳ���λ��  ��\n");
	printf("\t   ��                    ��\n");
	printf("\t   �� ��3������          ��\n");
	printf("\t   ��                    ��\n");
	printf("\t   ��������������������������������������������\n");
	printf("��ѡ��1-3��:");
	getchar();
	char choose = getchar();
	switch (choose) {
		case '1':
			searchWordCount();
			break;
		case '2':
			searchWordLine();
			break;
		case '3':
			break;
		default:
			break;
	}
	clear();
	main_Menu();
}

///// ���˵� /
Status main_Menu() {
	// ������غ��� - ��ʼ�������б�
	SeqList *sq = (SeqList *)malloc(sizeof(SeqList));
	ElementType et;
	init_SeqList(sq, &et);
	while (1) {
		printf("\t***    �q�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�r     ***\n");
		printf("\t*                                       *\n");
		printf("\t*      ��          ���˵�         ��      *\n");
		printf("\t*                                       *\n");
		printf("\t*      �t�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�s       *\n");
		printf("\t*                                       *\n");
		printf("\t*      ����������������������������������������������������       *\n");
		printf("\t*      ��                        ��       *\n");
		printf("\t*      �� ��1�������ı��ĵ�      ��       *\n");
		printf("\t*      ��                        ��       *\n");
		printf("\t*      �� ��2�����ʼ���          ��       *\n");
		printf("\t*      ��                        ��       *\n");
		printf("\t*      �� ��3���˳�              ��       *\n");
		printf("\t*      ��                        ��       *\n");
		printf("\t*      ����������������������������������������������������       *\n");
		printf("\t*                                       *\n");
		printf("\t*****************************************\n");
		printf("��ѡ��1-3����");
		char choose = getchar();
		switch (choose) {
			case '1':
				create_File();
				break;
			case '2':
				searchWord_Menu();
				break;
			case '3':
				exit(0);
				break;
			default:
				break;
		}
		clear();
	}
}

//////����
void clear() {

	//system("cls");

	system("clear");
}

//////�����������
void continueStart() {
	printf("\n�����������...\n");
	getchar();
	getchar();
}

int main() {
	main_Menu();
	return 0;
}