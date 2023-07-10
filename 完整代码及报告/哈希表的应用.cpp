#include <bits/stdc++.h>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 2
#define ERROR -2

typedef int Status;


/////线性表初始长度
#define INIT_SIZE 1024

////线性表长度增量
#define INCREASE_SIZE 10

////文件名最大长度
#define FILE_NAME_LEN 20

/////字符数组最大长度
#define MAX_STR 255

//// 单个单词最大长度
#define WORD_LEN 20

#define WORD 21


/************* 结构体定义 ************/

/* 定义顺序串 */
typedef struct {
	char ch[MAX_STR];
	int length;
} String;

/* 单个单词结构 */
typedef struct {
	char word[INIT_SIZE];	// 存储单词，不超过二十个字符
	int count;			// 单词出现的次数
} ElementType;

/* 单词列表 */
typedef struct {
	ElementType *et;	// 指向存储单词的头指针
	int length;
	int listSize;
} SeqList;


/************* 函数定义 ************/

///// 初始化单词列表
Status init_SeqList(SeqList *sq, ElementType *et);

////// 添加单词
Status add_SeqList(SeqList *sq, ElementType *et, char *word);

/////统计单个单词个数
Status count_SeqList(SeqList *sq, ElementType *et);

//// 创建文本文件
Status create_File();


/////BF串模式匹配算法
Status BF_Compare(String s1, String s2, int length);

////部分匹配函数表
void get_next(String s2, int *next);

//// 检索单词出现的个数
Status searchWordCount();

////检索单词在该行出现的次数、行号、相关位置
Status searchWordLine();

//// 单词索引菜单
void searchWord_Menu();

//// 主菜单
Status main_Menu();

///清屏函数
void clear();

///按任意键继续
void continueStart();


/* 初始化单词列表 */
Status init_SeqList(SeqList *sq, ElementType *et) {
	sq->et = et;
	sq->length = 0;
	return TRUE;
}

/* 添加单词 -  */
Status add_SeqList(SeqList *sq, ElementType *et, char *word) {
	int i = 0, j = 0;
	for (int i = 0; i < sq->length; i++) {
		// 如果当前单词和要加入的单词相同，直接统计
		if (strcmp(et[i].word, word) == 0) {
			et[i].count++;
			return OK;
		}
	}

	if (sq->length == INIT_SIZE) {
		printf("\n空间不足，单词【%s】插入失败！\n", word);
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

/* 统计单个单词个数 */
Status count_SeqList(SeqList *sq, ElementType *et) {
	int i, j = 0;
	for (i = 0; i < sq->length; i++) {
		j = j + et[i].count;
	}
	return j;
}

//// 创建文本文件
Status create_File() {
	ElementType et;		// 单词结构体
	SeqList sq;			// 单词列表
	char choose;
	char fileName[FILE_NAME_LEN + 1];
	FILE *fp;			// 文件指针

	printf("请输入要创建的文件名（重复的文件名称将被重置！）：");
	scanf("%s", fileName);
	fp = fopen(fileName, "w");
	choose = 'n';
	while (choose == 'n' || choose == 'N') {
		// 获取用户输入的内容
		printf("请输入一行文本：");
		gets(et.word);
		gets(et.word);
		sq.length = strlen(et.word);
		fwrite(&et, sq.length, 1, fp);	// 将内容写入到文件
		// 写入换行符
		fprintf(fp, "%c", 10);
		printf("是否结束输入?(y/n)：");
		choose = getchar();
	}
	fclose(fp);
	printf("\n建立文件成功！");
	continueStart();
	return TRUE;
}


//////BF串模式匹配算法
Status BF_Compare(String s1, String s2, int length) {
	int i, j;
	i = length - 1;       /* 扫描s1的下标，因为c中数组下标是从0开始，串中序号相差1 */
	j = 0; /* 扫描s2的开始下标 */
	int next[255];
	get_next(s2, next);
	while (i < s1.length && j < s2.length) {
		if (s1.ch[i] == s2.ch[j]) {
			i++;
			j++;  /* 继续使下标移向下一个字符位置 */
		} else {
			i = i - j + 1;
			j = 0;
			//j = next[j];
		}
	}
	if (j >= s2.length)
		return i - s2.length;
	else
		return -1; /* 表示s1中不存在s2，返回-1 */
}

/////部分匹配函数表
void get_next(String s2, int *next) {
	int i = 0;
	int j = -1;
	next[0] = -1;   //不会用到
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

/////检索单词出现的个数
Status searchWordCount() {
	FILE *fp;
	String s, t;	// 定义两个串，s为母串，t为子串
	char fileName[FILE_NAME_LEN];
	int i = 0, j, k;

	printf("输入文件名（如database.txt）：");
	scanf("%s", fileName);
	fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("\n文件打开失败！\n");
		continueStart();
		return FALSE;
	}

	printf("请输入要统计出现次数的单词：");
	scanf("%s", t.ch);
	t.length = strlen(t.ch);

	while (!feof(fp)) {
		// 设置默认 110 个字符为一行
		memset(s.ch, '\0', 110);
		fgets(s.ch, 110, fp);
		s.length = strlen(s.ch);
		// 初始化检索位置
		k = 0;
		// 遍历整个主串 s
		while (k < s.length - 1) {
			j = BF_Compare(s, t, k);
			if (j < 0)
				break;
			else {
				i++;					// 单词计数器累加
				k = j + t.length;		// 继续下一子串的检索
			}
		}
	}
	printf("\n单词【%s】在文本文件 %s 中，共出现 %d 次\n", t.ch, fileName, i);

	continueStart();
	return OK;
}

/////检索单词的行号、相关位置
Status searchWordLine() {
	FILE *fp;
	String s, t;	// 定义两个串
	char fileName[FILE_NAME_LEN];
	int wPos[20];	// 存放一行中字符匹配的多个位置

	int i, j, k, l, m;

	printf("输入文件名（如database.txt）：");
	scanf("%s", fileName);
	fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("\n文件打开失败！\n");
		continueStart();
		return FALSE;
	}

	printf("请输入要检索的单词：");
	scanf("%s", t.ch);
	t.length = strlen(t.ch);

	l = 0;	// 初始化计数器用来统计次数位置
	while (!feof(fp)) {
		// 扫描整个文件
		memset(s.ch, '\0', 110);
		fgets(s.ch, 110, fp);
		s.length = strlen(s.ch);
		l++;	// 计数器自增
		k = 0;	// 初始化检索位置
		i = 0;	// 初始化单词计数器

		// 检索整个主串s
		while (k < s.length - 1) {
			j = BF_Compare(s, t, k);	// 调用串匹配算法
			if (j < 0)
				break;
			else {
				i++;	// 单词计数器自增
				wPos[i] = j;		// 记录单词位置
				k = j + t.length;	// 继续下一子串检索
			}
		}

		if (i > 0) {
			printf("行号：%d，次数：%d，起始位置分别为：\n", l, i);
			for (m = 1; m <= i; m++) {
				printf("第 %4d 个字符\n", wPos[m] + 1);
			}
		}
		printf("\n");
	}
	continueStart();
	return OK;
}

///////菜单
void searchWord_Menu() {
	printf("\n----------------------------------------------------\n");
	printf("\t─────────  菜单  ─────────\n\n");
	printf("\t   ┌────────────────────┐\n");
	printf("\t   │                    │\n");
	printf("\t   │ 【1】单词出现次数  │\n");
	printf("\t   │                    │\n");
	printf("\t   │ 【2】单词出现位置  │\n");
	printf("\t   │                    │\n");
	printf("\t   │ 【3】返回          │\n");
	printf("\t   │                    │\n");
	printf("\t   └────────────────────┘\n");
	printf("请选择（1-3）:");
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

///// 主菜单 /
Status main_Menu() {
	// 调用相关函数 - 初始化单词列表
	SeqList *sq = (SeqList *)malloc(sizeof(SeqList));
	ElementType et;
	init_SeqList(sq, &et);
	while (1) {
		printf("\t***    ╭════════════════════════╮     ***\n");
		printf("\t*                                       *\n");
		printf("\t*      │          主菜单         │      *\n");
		printf("\t*                                       *\n");
		printf("\t*      ╰════════════════════════╯       *\n");
		printf("\t*                                       *\n");
		printf("\t*      ┌────────────────────────┐       *\n");
		printf("\t*      │                        │       *\n");
		printf("\t*      │ 【1】建立文本文档      │       *\n");
		printf("\t*      │                        │       *\n");
		printf("\t*      │ 【2】单词检索          │       *\n");
		printf("\t*      │                        │       *\n");
		printf("\t*      │ 【3】退出              │       *\n");
		printf("\t*      │                        │       *\n");
		printf("\t*      └────────────────────────┘       *\n");
		printf("\t*                                       *\n");
		printf("\t*****************************************\n");
		printf("请选择（1-3）：");
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

//////清屏
void clear() {

	//system("cls");

	system("clear");
}

//////按任意键继续
void continueStart() {
	printf("\n按任意键继续...\n");
	getchar();
	getchar();
}

int main() {
	main_Menu();
	return 0;
}