// BST Search with graphics (files)
#include <conio.h>
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include <iostream.h>

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
int cx = 450, cy = 50, length, p = 0, f = 0, indent;
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

/*
void writeFile()
{
	FILE *fp;
	char ch[100], c;
	fp = fopen("userNodes.txt", "w");
	if (fp == NULL)
		perror("fopen");
	// printf("File not created!\n");
	printf("\nEnter numbers in the tree: (Press ENTER once you are done!)\n");
	fgets(ch, 100, stdin);
	fputs(ch, fp);
	printf("file created!\n");
	fclose(fp);
}
*/

node *createBST(FILE *fp)
{
	int val;
	node *root = NULL;
	if (!fp)
		;
	while ((val = readFile(fp)) != 0)
	{
		length++;
		root = insertNode(root, val);
	}
	return root;
}

void printTree1(node *temp)
{
	// in-order print of tree temp
	if (temp != NULL)
	{
		printTree1(temp->left);
		printf("%d\t", temp->value);
		printTree1(temp->right);
	}
}

void printTree2(node *temp)
{
	// pre-order print of tree temp
	if (temp != NULL)
	{
		printf("%d\t", temp->value);
		printTree2(temp->left);
		printTree2(temp->right);
	}
}

void printTree3(node *temp)
{
	// post-order print of tree temp
	if (temp != NULL)
	{
		printTree3(temp->left);
		printTree3(temp->right);
		printf("%d\t", temp->value);
	}
}

int BSTSearch(node *tree, int val)
{
	if (tree == NULL)
		return 0;
	printf("%d %d \t", tree->value, val);
	if (p == 1)
		return p;
	if (val <= tree->value)
	{
		if (val == tree->value)
			p = 1;
		else
			BSTSearch(tree->left, val);
	}
	else
	{
		if (val == tree->value)
			p = 1;
		else
			BSTSearch(tree->right, val);
	}
	return p;
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
	cy = 50;
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
	// floodfill(cx,cy,WHITE);
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

void main(void)
{
	clrscr();
	node *tree;
	int val, present;
	FILE *fp;
	fp = fopen("treeNodes.txt", "r");
	initgraph(&gd, &gm, "");
	cleardevice();
	tree = createBST(fp);
	fclose(fp);
	reset();
	drawTree(tree);
	reset();
	printf("\nEnter the number to be searched:\t");
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
}