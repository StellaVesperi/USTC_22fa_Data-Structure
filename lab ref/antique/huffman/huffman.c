#include<stdio.h>
#include<stdlib.h>
#define MAX_CHARS 256
#define CODE_LENGTH 256
#define TRUE 1
#define FALSE 0
#define MAXINT 2147483647
#define status int
#define WINDOW_SIZE 8192

typedef struct
{
	int weight;
	int lchild;
	int rchild;
	int parent;
} HTNode;
HTNode node[2*MAX_CHARS];

int code[MAX_CHARS][CODE_LENGTH] = {0}; //each represent a string

unsigned char revmap[MAX_CHARS]; // map from array index to ASCII char
unsigned char map[MAX_CHARS]; //map from ASCII to array index in huffman tree
int num; //number of characters occured
int root; //the index of huffman tree root
unsigned int total_write_bytes = 0;

unsigned char readbuf[WINDOW_SIZE];
unsigned char writebuf[WINDOW_SIZE];
unsigned int read_count;
unsigned int read_bufsize;
unsigned int write_count;
unsigned int write_bufsize;

status read(unsigned char* data, FILE *fpi) {
    if (read_count == 0 || read_count == WINDOW_SIZE) {
        read_bufsize = fread(readbuf, WINDOW_SIZE, 1, fpi);
        read_count = 0;
    }
    if (read_count == read_bufsize) // the end of file
        return FALSE;
    *data = readbuf[read_count++];
    return TRUE;
}

status write(unsigned char* data, FILE *fpo) {
    writebuf[write_count++] = *data;
    if (write_count == WINDOW_SIZE) {
        if (!fwrite(writebuf, WINDOW_SIZE, 1, fpo))
           return FALSE;
        write_count = 0;
    }
    return TRUE;
}

status write_flush(FILE *fpo) {
    if (write_count != 0)
        if (fwrite(writebuf, write_count, 1, fpo))
           return TRUE;
        else return FALSE;
    else return TRUE;
}

/*status initialize() {
	int i, x[MAX_CHARS] = {0};
	unsigned char file[100], c;
	FILE *fp;
	printf("Input training file name for statistics.\n");
	scanf("%s",file);
	fp = fopen(file,"rb");
	if(!fp) {
		printf("File not exist!\n");
		return FALSE;
	}
	printf("Creating statistics...\n");
	while(fscanf(fp,"%c",&c) != EOF) {
		x[c]++;
	}
	fclose(fp);

	num = 0;
	for(i=0; i<MAX_CHARS;i++)
		if(x[i]) {
			map[i] = num;
			revmap[num] = i;
			node[num].weight = x[i];
			node[num].lchild = -1;
			node[num].rchild = -1;
			node[num].parent = -1;
			num++;
		}
		else map[i] = -1;
	return TRUE;
}*/

void readin_file(FILE *fpi) {
	int i, x[MAX_CHARS] = {0};
	unsigned char c;
	while (fread(&c, 1, 1, fpi))
		x[c]++;
	//num = 0;
	for(i=0; i<MAX_CHARS;i++) {
	//	if(x[i]) {
	//		map[i] = num;
	//		revmap[num] = i;
			node[i].weight = x[i];
			node[i].lchild = -1;
			node[i].rchild = -1;
			node[i].parent = -1;
	//		num++;
	//	}
	}
	num = MAX_CHARS;
}

void generate_tree(FILE *fpi) {
	readin_file(fpi);
	build_tree();
	create_code();
}

status build_tree() {
	int i,j,max1,maxj1,max2,maxj2;
//	printf("Building huffman tree...\n");
	for(i=num; i < 2*num - 1; i++) { //repeat for n-1 times
		max1 = MAXINT; max2 = MAXINT; maxj1 = maxj2 = 0;
		for(j=0;j<i;j++) { //search for two root nodes with min weight
			if(node[j].parent == -1) {
				if(node[j].weight < max1) {
					max2 = max1;
					maxj2 = maxj1;
					max1 = node[j].weight;
					maxj1 = j;
				}
				else if(node[j].weight < max2) {
					max2 = node[j].weight;
					maxj2 = j;
				}
			}
		}
		if(max1 == MAXINT || max2 == MAXINT) {
			printf("Unexpected Error!");
			return FALSE;
		}
		// consistent order of left and right: ascii(lchild) < ascii(rchild)
		if (maxj1 > maxj2) {
			int t;
			t = maxj1; maxj1 = maxj2; maxj2 = t;
		}
		node[i].lchild = maxj1;
		node[i].rchild = maxj2;
		node[i].weight = max1 + max2;
		node[i].parent = -1;
		node[maxj1].parent = i;
		node[maxj2].parent = i;
	}
	return TRUE;
}

void output_code(FILE *fpo) {
	int i;
	for (i=0; i<2*MAX_CHARS-1; i++) {
		unsigned char buffer;
		if (node[i].parent == -1)
			buffer = i - MAX_CHARS;
		else	buffer = node[i].parent - MAX_CHARS; // to 0~255
		write(&buffer, fpo);
	}
}

void rebuild_tree(FILE *fpi) {
	int i,j;
	for (i=0; i<2*MAX_CHARS-1; i++) {
		unsigned char buffer;
		read(&buffer, fpi);
		if (buffer == i - MAX_CHARS)
			node[i].parent = -1;
		else	node[i].parent = buffer + MAX_CHARS; // to range 0~255
//		printf("%d ", node[i].parent);
	}

	for (i=0; i<2*MAX_CHARS-1; i++) {
		int l = -1, r = -1;
		for (j=0; j<2*MAX_CHARS-1; j++)
			if (node[j].parent == i) {
				if (l == -1)
					l = j;
				else if (r == -1)
					r = j;
				else {
					printf("Compressed file corrupted!\n");
					exit(0);
				}
			}
		// make sure l<r
		if (l>r) {
			int t;
			t=l; l=r; r=t;
		}
		node[i].lchild = l;
		node[i].rchild = r;
	}
}

status create_code() {
	int i, t, n;
	unsigned char x[CODE_LENGTH];
	root = 0; //find root of huffman tree
	while(node[root].parent != -1)
		root = node[root].parent;

//	for (i=MAX_CHARS;i<2*MAX_CHARS;i++)
//		printf("%d %d ", node[i].lchild, node[i].rchild);
	for(i=0;i<MAX_CHARS;i++) {
		t = i; n = 0;
		while(node[t].parent != -1) {
			if(node[node[t].parent].lchild == t)
				x[n++] = '0';
			else x[n++] = '1';
			t = node[t].parent;
		}
		for(t=0;t<n;t++)
			code[i][t] = x[n-t-1];
		code[i][n] = 0; // generate a string representing the huffman code
/*		printf("%d - ",i);
		for(t=0;t<n;t++)
			printf("%c",code[i][t]);
		printf("\n");
*/	}
	//printf("Completed\n");
	return TRUE;
}

char read_bit_c = 0;
int read_bits = 0;

status read_bit(FILE *fpi, char* data) { // bit is bool to return
//	static char c;
//	static int bit = 0;
	if (read_bits == 8 || read_bits == 0) {
		if (!read(&read_bit_c, fpi))
			return FALSE;
		read_bits = 0;
	}
	*data = ((read_bit_c & (1 << (read_bits++))) == 0) ? '0' : '1';
	return TRUE;
}

char write_bit_c = 0;
int write_bits = 0;

void write_bit(FILE *fpo, char data) { // bit is bool
//	static char c = 0;
//	static int bit = 0;
	if (data == '1')
		write_bit_c |= (1 << write_bits);
	write_bits++;

	if (write_bits == 8) {
		write(&write_bit_c, fpo);
		total_write_bytes++;
		write_bit_c = 0;
		write_bits = 0;
	}
}

void write_bit_flush(FILE *fpo) {
	if (write_bits != 0)
		write(&write_bit_c, fpo);
}

status do_compress(FILE *fpi, FILE *fpo) {
	unsigned char c;
	while (read(&c, fpi)) {
//		printf("%d %d\t", c, node[c].parent);
	/*	if(map[c] == -1) {
			printf(" Invalid character. Output will not be saved.\n");
			return FALSE;
		}
		else {*/
			int t=0;
			while(code[c][t] != 0) // cancel map
				write_bit(fpo, code[c][t++]);
	/*	}*/
	}
	write_bit_flush(fpo);
	return TRUE;
}
/*
status encode() {
	unsigned char a[1000];
	int i=0;
	FILE *fp;
	fp = fopen("huffman.ini","w");
	if(!fp) {
		printf("Output file is read only or protected.\n");
		return FALSE;
	}
	printf("Input a string to be encoded.\n");
	scanf("%s",a);
	printf("Huffman code is: ");
	while(a[i] != '\0') {
		if(map[a[i]] == -1) {
			printf(" Invalid character. Output will not be saved.\n");
			return FALSE;
		}
		else {
			int t=0;
			while(code[map[a[i]]][t]) {
				printf("%c",code[map[a[i]]][t]);
				fprintf(fp,"%c",code[map[a[i]]][t]);
				t++;
			}
			printf(" ");
			i++;
		}
	}
	printf("\nOutput file has been generated.\n");
	fclose(fp);
	return TRUE;
}

status decode() {
	unsigned char c;
	int i,now;
	FILE *fp;
	fp = fopen("huffman.ini", "r");
	if(!fp) {
		printf("Output file does not exist. Please encode a string first.\n");
		return FALSE;
	}
	printf("Decoding Huffman code...\n");
	while(TRUE) {
		i = 0; now = root;
		while(node[now].lchild != -1 || node[now].rchild != -1) {
            		if(fscanf(fp,"%c",&c) == EOF) {
		                if(now == root) {
					printf("\nDecoding completed.\n");
					return TRUE;
				} else {
					printf("\nEncoded file is broken. Please check it.\n");
					return FALSE;
				}
            		}
			if(c == '0')
				now = node[now].lchild;
			else now = node[now].rchild;
		}
		printf("%c",revmap[now]);
	}
	//should never reach here.
}
*/
status do_uncompress(FILE* fpi, FILE* fpo) {
	while(TRUE) {
		int i = 0, now = root;
		unsigned char buffer, c;
		while(node[now].lchild != -1 || node[now].rchild != -1) {
            		if(!read_bit(fpi, &c)) {
		                //if(now == root) {
					return TRUE;
				/*} else {
					printf("Compress file is broken. Please check it.\n");
					return FALSE;
				}*/
            		}
			if(c == '0')
				now = node[now].lchild;
			else now = node[now].rchild;
		}
		//buffer = revmap[now];
		buffer = now;
		write(&buffer, fpo);
	}
	// should never reach here.
}

void compress(char* infile, char* outfile) {
	FILE *fpi, *fpo;
	int file_size;
	if (!(fpi = fopen(infile, "rb"))) {
		printf("Cannot open input file!\n");
		return;
	}
	read_count = 0;
	if (!(fpo = fopen(outfile, "wb"))) {
		printf("Cannot write output file!\n");
		return;
	}
	write_count = 0;
	generate_tree(fpi);
	fclose(fpi);
	output_code(fpo);
	// reopen
	if (!(fpi = fopen(infile, "rb"))) {
		printf("Cannot open input file!\n");
		return;
	}
	read_count = 0;
	do_compress(fpi, fpo);

	file_size = node[2*MAX_CHARS-2].weight;
	total_write_bytes += 2 * MAX_CHARS - 1;
	printf("OK - Original file size %d, compressed %d, compress rate = %lf\n", file_size, total_write_bytes, (double)total_write_bytes / (double)file_size);
	fclose(fpi);
	write_flush(fpo);
	fclose(fpo);
}

void uncompress(char* infile, char* outfile) {
	FILE *fpi, *fpo;
	if (!(fpi = fopen(infile, "rb"))) {
		printf("Cannot open input file!\n");
		return;
	}
	read_count = 0;
	if (!(fpo = fopen(outfile, "wb"))) {
		printf("Cannot write output file!\n");
		return;
	}
	write_count = 0;
	rebuild_tree(fpi);
	create_code();
	do_uncompress(fpi, fpo);
	fclose(fpi);
	write_flush(fpo);
	fclose(fpo);
}

void usage() {
		printf("Huffman Tree Demo (C) 2011-12-16 boj\n");
		printf("Usage: huffman [-c file] [-u file] output_file\n");
		printf("  -c Compress file.	e.g. huffman -c test.txt test.huff\n");
		printf("  -u Uncompress file.	e.g. huffman -u test.huff test.txt\n");
}

int main(int argc, char** argv) {
	char op;
	if (argc == 1) {
		usage();
	}
	else {
		if (strcmp(argv[1], "-c") == 0)
			compress(argv[2], argv[3]);
		else if (strcmp(argv[1], "-u") == 0)
			uncompress(argv[2], argv[3]);
		else usage();
		return 0;
	}
/*	
	while(1)
	{
		printf("huffman > ");
		while(1) {
			scanf("%c",&op);
			if(op!=' ' && op!='\n') 
				break;
		}
		if(op == 'q') break;
		switch(op)
		{
			case 'i':if(!initialize()) 
						 break;
					 if(!build_tree())
						 break;
					 create_code();break;
			case 'e':encode();break;
			case 'd':decode();break;
			default:printf("Invalid operation.\n");
		}
	}
	return 0;
*/
}
