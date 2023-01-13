#include <conio.h>
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include <time.h>
#include <iostream.h>

int F = 0, count = 0;
int present;
clock_t t;
//int timeTaken;

struct node
{
	int value;
	struct node *left;
	struct node *right;
};
typedef struct node node;

int BSTSearchG(node *, int);
void drawTree(node *);

typedef struct
{
	int x1, y1, x2, y2;
} r;
r right;

typedef struct
{
	int x1, y1, x2, y2;
} l;
l left;

typedef struct
{
	int cx, cy;
	l left;
	r right;
} data;
data n1[20], n2[20];

void saveData(data[], int i);
void retrieveData(data[], int);
void reset();
void updateRight();
void updateLeft();

int gd = DETECT, gm;
int cx = 450, cy = 50, p = 0, f = 0, indent;
char *ch;

node *createNode(int x)
{
	node *temp = (node *)malloc(sizeof(node));
	temp->value = x;
	temp->left = '\0';
	temp->right = '\0';
	return temp;
}

node *insertNode(node *temp, int val)
{
	node *newNode = createNode(val);
	if (!temp)
		return newNode;
	if (temp->value <= val)
	{
		insertNode(temp->right, val);
		if (!(temp->right))
			temp->right = newNode;
	}
	if (temp->value > val)
	{
		insertNode(temp->left, val);
		if (!(temp->left))
			temp->left = newNode;
	}
	return temp;
}

int readFile(FILE *fp1)
{
	int num, m;
	while ((m = fgetc(fp1)) != EOF)
	{
		if (m >= 48 && m <= 57)
		{
			m = (int)m - 48;
			if (m < 0)
				continue;
			num = m;
			while ((m = fgetc(fp1)) != ' ')
			{
				if (m == EOF)
					break;
				if (m >= 48 && m <= 57)
				{
					m = (int)m - 48;
					num = (num * 10) + m;
				}
			}
			return num;
		}
	}
	return NULL;
}

void writeFile()
{
	FILE *fp;
	char word[100], c;
	clrscr();
	fp = fopen("userNodes.txt", "w");
	if (fp == NULL)
		perror("fopen");
	// printf("File not created!\n");
	printf("\nEnter numbers in the tree: (Press ENTER once you are done!)\n");
	fgets(word, 100, stdin);
	fputs(word, fp);
	printf("file created!\n");
	fclose(fp);
	getch();
}

node *createBSTg(FILE *fp)
{
	int val;
	node *root = NULL;
	if (!fp)
		;
	while ((val = readFile(fp)) != NULL)
	{
		root = insertNode(root, val);
	}
	return root;
}

node *createBST()
{
	int val;
	node *root;
	FILE *fp = fopen("node.txt", "r");
	while ((val = readFile(fp)) != NULL)
	{
		root = insertNode(root, val);
	}
	fclose(fp);
	return root;
}

void updateLeft()
{
	indent = (left.x1 + left.x2) / 2;
	left.x1 = left.x2;
	left.x2 = left.x1 - (indent - left.x1);
	left.y1 = left.y2 + 30;
	left.y2 = left.y1 + 30;
	cx = left.x1;
	cy = left.y1 - 15;
	right.x1 = cx;
	right.y1 = cy + 15;
	right.x2 = indent;
	right.y2 = right.y1 + 30;
}

void updateRight()
{
	indent = (right.x1 + right.x2) / 2;
	right.x1 = right.x2;
	right.x2 = right.x1 + (right.x1 - indent);
	right.y1 = right.y2 + 30;
	right.y2 = right.y1 + 30;
	cx = right.x1;
	cy = right.y1 - 15;
	left.x1 = cx;
	left.y1 = cy + 15;
	left.x2 = indent;
	left.y2 = left.y1 + 30;
}

void saveData(data n[], int i)
{
	n[i].left.x1 = left.x1;
	n[i].left.x2 = left.x2;
	n[i].left.y1 = left.y1;
	n[i].left.y2 = left.y2;
	n[i].right.x1 = right.x1;
	n[i].right.x2 = right.x2;
	n[i].right.y1 = right.y1;
	n[i].right.y2 = right.y2;
	n[i].cx = cx;
	n[i].cy = cy;
}

void retrieveData(data n[], int i)
{
	left.x1 = n[i].left.x1;
	left.x2 = n[i].left.x2;
	left.y1 = n[i].left.y1;
	left.y2 = n[i].left.y2;
	right.x1 = n[i].right.x1;
	right.x2 = n[i].right.x2;
	right.y1 = n[i].right.y1;
	right.y2 = n[i].right.y2;
	cx = n[i].cx;
	cy = n[i].cy;
}

void reset()
{
	cx = 400;
	cy = 100;
	f = 0;
	right.x1 = left.x1 = cx;
	right.y1 = left.y1 = cy + 15;
	right.y2 = left.y2 = right.y1 + 30;
	right.x2 = right.x1 + 120;
	left.x2 = left.x1 - 120;
}

void drawTree(node *tree)
{
	if (tree != NULL)
	{
		setcolor(WHITE);
		circle(cx, cy, 15);
		sprintf(ch, "%d", tree->value);
		outtextxy(cx - 6, cy - 4, ch);
		if (tree->left != '\0')
		{
			saveData(n1, f++);
			line(left.x1, left.y1, left.x2, left.y2);
			updateLeft();
			drawTree(tree->left);
		}
		if (tree->right != '\0')
		{
			saveData(n1, f++);
			line(right.x1, right.y1, right.x2, right.y2);
			updateRight();
			drawTree(tree->right);
		}
		if (f > 0)
			retrieveData(n1, --f);
	}
}

void printTree(node *temp)
{
	if (temp != NULL)
	{
		printf("%d\t", temp->value);
		printTree(temp->left);
		printTree(temp->right);
	}
}

int height(node *root)
{
	int hleft, hright;
	if (root == NULL)
		return 0;
	hleft = height(root->left);
	hright = height(root->right);
	if (hleft > hright)
		return hleft + 1;
	else
		return hright + 1;
}

void checkGivenLevel(node *tree, int level, int val)
{
	if (tree == NULL)
		return;
	if (F == 1)
		return;
	if (level == 1)
	{
		// printf("%d %d \t",tree->value,val);
		count++;
		if (tree->value == val)
		{
			F = 1;
		}
	}
	else if (level > 1)
	{
		checkGivenLevel(tree->left, level - 1, val);
		checkGivenLevel(tree->right, level - 1, val);
	}
}

int BFS(node *tree, int val)
{
	int h = height(tree);
	// printf("height= %d\n ",h);
	for (int i = 1; i <= h; i++)
	{
		checkGivenLevel(tree, i, val);
		if (F == 1)
			break;
	}
	if (F == 1)
		return 1;
	else
		return 0;
}

int DFS(node *tree, int val)
{
	if (tree == NULL)
		return 0;
	if (F == 1)
		return F;
	// printf("%d %d \t",tree->value,val);
	count++;
	if (tree->value == val)
		F = 1;
	DFS(tree->left, val);
	DFS(tree->right, val);
	return F;
}

int BSTSearch(node *tree, int val)
{
	if (tree == NULL)
		return 0;
	if (F == 1)
		return F;
	// printf("%d %d \t",tree->value,val);
	count++;
	if (val <= tree->value)
	{
		if (val == tree->value)
			F = 1;
		else
			BSTSearch(tree->left, val);
	}
	else
	{
		if (val == tree->value)
			F = 1;
		else
			BSTSearch(tree->right, val);
	}
	return F;
}

void checkGivenLevelG(node *tree, int level, int val)
{
	if (tree == NULL)
		return;
	if (p == 1)
		return;
	if (level == 1)
	{
		setcolor(BLUE);
		setfillstyle(SOLID_FILL, BLUE);
		pieslice(cx, cy, 0, 360, 15);
		setcolor(WHITE);
		circle(cx, cy, 15);
		sprintf(ch, "%d", tree->value);
		outtextxy(cx - 6, cy - 4, ch);
		delay(800);
		if (tree->value == val)
		{
			p = 1;
		}
	}
	else if (level > 1)
	{
		saveData(n1, f++);
		updateLeft();
		checkGivenLevelG(tree->left, level - 1, val);
		retrieveData(n1, --f);
		saveData(n1, f++);
		updateRight();
		checkGivenLevelG(tree->right, level - 1, val);
		retrieveData(n1, --f);
	}
}

int BFSG(node *tree, int val)
{
	int h = height(tree);
	for (int i = 1; i <= h; i++)
	{
		reset();
		checkGivenLevelG(tree, i, val);
		if (p == 1)
			break;
	}
	if (p == 1)
		return 1;
	else
		return 0;
}

int DFSG(node *temp, int val)
{
	if (p == 1)
		return p;
	if (temp == NULL)
	{
		retrieveData(n1, --f);
		return 0;
	}
	setcolor(BLUE);
	setfillstyle(SOLID_FILL, BLUE);
	pieslice(cx, cy, 0, 360, 15);
	setcolor(WHITE);
	circle(cx, cy, 15);
	sprintf(ch, "%d", temp->value);
	outtextxy(cx - 6, cy - 4, ch);
	delay(800);
	if (temp->value == val)
		p = 1;
	saveData(n1, f++);
	updateLeft();
	DFSG(temp->left, val);
	saveData(n1, f++);
	updateRight();
	DFSG(temp->right, val);
	if (f > 0)
		retrieveData(n1, --f);
	return f;
}

int BSTSearchG(node *temp, int val)
{
	if (p == 1)
		return p;
	if (temp == NULL)
		return 0;
	setcolor(BLUE);
	setfillstyle(SOLID_FILL, BLUE);
	pieslice(cx, cy, 0, 360, 15);
	setcolor(WHITE);
	circle(cx, cy, 15);
	sprintf(ch, "%d", temp->value);
	outtextxy(cx - 6, cy - 4, ch);
	delay(800);
	if (temp->value == val)
		p = 1;
	else if (temp->value < val)
	{
		saveData(n2, f++);
		updateRight();
		BSTSearchG(temp->right, val);
		if (!(temp->right))
			p = 0;
	}
	else if (temp->value >= val)
	{
		saveData(n2, f++);
		updateLeft();
		BSTSearchG(temp->left, val);
		if (!(temp->left))
			p = 0;
	}
	retrieveData(n2, --f);
	return p;
}

void compare_alg()
{
	node *tree;
	int val, PRESENT;

	clrscr();
	tree = createBST();
	printTree(tree);
	printf("\n--------------------------------------------------------------------------------\n");
	printf(" The comparative study is done using a TREE with 10000 nodes with values in the RANGE of 0 to 1000 GENERATED RANDOMLY\n Shown above are the values in the nodes of the Binary Search Tree; traversed in PRE-ORDER\n");
	printf("\nEnter the number to be searched:\t");
	scanf("%d", &val);

	clrscr();
	printf("\n\t\t\t\tComparative Study");
	printf("\n--------------------------------------------------------------------------------\n");

	printf("\t------BREADTH FIRST SEARCH AND DEPTH FIRST SEARCH------\n");
	F = 0;
	count = 0;
	t = clock();
	PRESENT = BFS(tree, val);
	t = clock() - t;
	//timeTaken = ((int)t) / CLOCKS_PER_SEC;
	//printf("\nTime taken : %e sec", timeTaken);
	printf("\nNumber of steps - [BFS] : %d", count);

	F = 0;
	count = 0;
	DFS(tree, val);
	printf("\nNumber of steps - [DFS] : %d", count);
	printf("\nTiming Complexity : O(|N|)\n\twhere, N is the number of nodes");
	printf("\nSpace Complexity - [BFS] : O(|w|)\n\twhere, w is the width of the tree");
	printf("\nSpace Complexity - [DFS] : O(|h|)\n\twhere, h is the height of the tree\n");

	printf("\n\t------BINARY SEARCH TREE - SEARCH------\n");
	F = 0;
	count = 0;
	t = clock();
	BSTSearch(tree, val);
	t = clock() - t;
	//timeTaken = ((int)t) / CLOCKS_PER_SEC;
	//printf("\nTime taken : %e sec", timeTaken);
	printf("\nNumber of steps : %d", count);
	printf("\nTiming Complexity (Balanced tree) : O(|log N|)\n\twhere, N is the number of nodes\n");

	if (PRESENT)
		printf("\n\t\tNUMBER [%d] IS PRESENT!", val);
	else
		printf("\n\t\tNUMBER [%d] IS NOT PRESENT!", val);
	getch();
}

void main(void)
{
	clrscr();
	node *tree;
	int val, present;
	FILE *fp;
	char how;

	int exit = 1;
	do
	{
		int choice;
		clrscr();
		printf("\n\t\t\t\t MAIN MENU");
		printf("\n--------------------------------------------------------------------------------\n");
		printf("\n\t\t\t1. SEARCH USING ALGORITHMS\n\t\t\t2. COMPARE ALGORITHMS\n\t\t\t3. EXIT\n");
		printf("\n--------------------------------------------------------------------------------\n");
		printf("\n\t\tEnter your choice: ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 2:
			compare_alg();
			break;
		case 1:

			clrscr();
			printf("\n\t\t\t  EXISTING TREE / NEW TREE");
			printf("\n--------------------------------------------------------------------------------\n");
			printf("\nTo create your own tree press ANY KEY!\nPress ENTER to continue with pre existing tree\n");
			how = getch();
			fflush(stdin);
			if (how != 13)
			{
				writeFile();
				fp = fopen("userNodes.txt", "r");
			}
			else
				fp = fopen("treeNodes.txt", "r");

			clrscr();
			tree = createBSTg(fp);
			fclose(fp);

			int back = 1;
			do
			{
				clrscr();
				int what;
				printf("\n\t\t\t\t SEARCH MENU");
				printf("\n--------------------------------------------------------------------------------\n");
				printf("\n\t\t\t1. BFS ALGORITHM\n\t\t\t2. DFS ALGORITHM\n\t\t\t3. BST SEARCH\n\t\t\t4. RETURN TO MAIN MENU\n");
				printf("\n--------------------------------------------------------------------------------\n");
				printf("\n\t\tEnter your choice: ");
				scanf("%d", &what);

				switch (what)
				{
				case 1:
					initgraph(&gd, &gm, "");
					cleardevice();

					printf("\n\t\t\t Breadth First Search Algorithm");
					printf("\n--------------------------------------------------------------------------------\n");

					reset();
					drawTree(tree);
					reset();

					printf("\nEnter the number to be searched:\n");
					scanf("%d", &val);

					f = 0;
					p = 0;
					present = BFSG(tree, val);

					if (present)
						printf("\nSearch successful!");
					else
						printf("\nNumber not in the tree!");

					getch();
					closegraph();
					break;
				case 2:
					initgraph(&gd, &gm, "");
					cleardevice();

					printf("\n\t\t\t Depth First Search Algorithm");
					printf("\n--------------------------------------------------------------------------------\n");

					reset();
					drawTree(tree);
					reset();

					printf("\nEnter the number to be searched:\n");
					scanf("%d", &val);

					f = 0;
					p = 0;
					present = DFSG(tree, val);

					if (present)
						printf("\nSearch successful!");
					else
						printf("\nNumber not in the tree!");

					getch();
					closegraph();
					break;
				case 3:
					initgraph(&gd, &gm, "");
					cleardevice();

					printf("\n\t\t\t Searching in Binary Search Tree");
					printf("\n--------------------------------------------------------------------------------\n");

					reset();
					drawTree(tree);
					reset();

					printf("\nEnter the number to be searched:\n");
					scanf("%d", &val);

					f = 0;
					p = 0;
					present = BSTSearchG(tree, val);

					if (present)
						printf("\nSearch successful!");
					else
						printf("\nNumber not in the tree!");

					getch();
					closegraph();
					break;
				case 4:
					back = 0;
					break;
				default:
					printf("\n\t\t\t------Invalid Choice------");
					getch();
				}
			} while (back);
			break;
		case 3:
			exit = 0;
			break;
		default:
			printf("\n\t\t\t------Invalid Choice------");
			getch();
		}
	} while (exit);
}