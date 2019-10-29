/*
	Student Name: Italo Peirano
	
	PID: 5660995

	Program Description: This program receives a text file in the command line 
	arguments. it will insert the lines of the files into a binary search tree.
	if there is a -c flag then it is case sensitive, otherwise its not case 
	sensitive. if there is -o flag with its argument, it means it will write the
	tree into the file in an inorder fashion. if there is no -o flag, then the
	output is in stdout. if there is no file then the input comes from stdin.

	Affirmation of Originality: "I affirm that I wrote this program myself without
	any help from any other people or sources from the internet."	 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int debug = 0;

int myStrcmp(const char * x, const char * y){
	int flag = 0;
	while(*x && flag == 0){
		if(*x != *y){
			flag = 1;
		}
		else{
			x++;
			y++;
		}
	}
	return *(const unsigned char*)x -*(const unsigned char*)y;
}
int myStrcasecmp(const char * x, const char *y){
/* not case sensitive compare */
	char resultx;
	char resulty;
	int flag = 0;
	while(*x && flag == 0){
		if(*x >= 'A' && *x <= 'Z'){
			resultx = tolower(*x);
						
		}else{
			resultx = *x;
		}
		if(*y >= 'A' && *y <= 'Z'){
			resulty = tolower(*y);
		}
		else{
			resulty = *y;
		}
		if(resultx != resulty){
			flag = 1;
		}
		else{
			x++;
			y++;
		}
	}
	return *(const unsigned char *)x - *(const unsigned char *)y;
}
struct node
{
	int instance;
	char * key;
	struct node * left;
	struct node * right;
};
struct node * newNode(char * item){
/* generates the node */
	int numChars;
	struct node * temp = (struct node *)malloc(sizeof(struct node));
	char * tempStr = (char *)malloc(101*sizeof(char));/*string allocation */
	strcpy(tempStr, item);
	temp->key = tempStr;
	temp->instance++;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}
void inorder(struct node *root){
	if(root != NULL){
		inorder(root->left);
		printf("%d %s \n",root->instance, root->key);
		inorder(root->right);
	}
}
void recur(struct node * root, FILE * outFile){
/* helps the function printToFile */
	if(root != NULL){
		recur(root->left,outFile);
		fprintf(outFile,"%d %s \n", root->instance, root->key);
		recur(root->right, outFile);
	}
} 
void printToFile(struct node * root, char * name){
	FILE *outFile;
	outFile = fopen(name,"w");
	recur(root, outFile);
	fclose(outFile);
}

void freePointer(struct node * root){
	if(root != NULL){
		freePointer(root->left);
		freePointer(root->right);
		free(root->key);
		free(root);
	}
}
struct node * insert(struct node* nod, char* key){
	if(nod == NULL){
		return newNode(key);	
	}
	int result;
	result = myStrcmp(key,nod->key);
	if(result < 0){
		nod->left = insert(nod->left,key);
	}
	else if(result > 0){
		nod->right = insert(nod->right,key);
	}
	else if(result == 0){
		nod->instance++;
	}
	return nod;
}
struct node * insertNotSensitive(struct node* nod, char * key){
	if(nod == NULL){
		return newNode(key);
	}
	int result;
	result = myStrcasecmp(key,nod->key);
	if(result < 0){
		nod->left = insert(nod->left,key);
	}
	else if (result > 0){
		nod->right = insert(nod->right,key);
	}
	else if(result == 0){
		nod->instance++;
	}
	return nod;	
}
int main(int argc, char **argv)
{
	extern char *optarg; /* special variable that gets the argument of a flag */
	extern int optind; /* number of arguments in the command line
				with some exceptions  */
	int c, err = 0; 
	int cflag = 0, oflag = 0, fflag = 0; /* control flags for processing */
	static char usage[] = "usage: bstsort [-c] [-o output_file_name] [input_file_name]\n";
	char * oname;
	FILE * ptrFile; /* point to the file, will be used for read */
	char text[1000]; /* store the file text in this variable */ 	
	int size;
	int count = 0;
	char line2[100];
	struct node * root;
	
	/*	
	"co:" means: "s" we have an s_flag, with no argument
	"c" we have an c_flag, with no argument
	"o" we have an o_flag, ":" with argument for o_flag
	*/
	while ((c = getopt(argc, argv, "co:")) != -1)
		switch (c) {
		case 'c':
			cflag = 1;
			break;
		case 'o':
			oflag = 1;
			oname = optarg; /* 0_flag argument */
			break;
		case '?':
			err = 1;
			break;
		}
 
	 if ((optind+1) > argc) {	
		/* 
 		need at least one argument (change +1 to +2 for two, etc. as needeed).
		if a flag is missing its corresponding argument an error message prints.
	        */
                if(err){ /*detects that the oflag is missing its argument*/
			printf("optind = %d, argc = %d\n", optind, argc);
			fprintf(stderr, "%s: missing flag_o argument\n",argv[0]);
			fprintf(stderr, usage, argv[0]);
			exit(0);	
		}
		//input comes from stdin, because there is no input file
		// in this case optind == argc
		int len;
		if(cflag == 1){
			printf("Enter lines (case sensitive),to put on the tree until empty line\n");
			root = NULL;
			do{
				fgets(line2,100,stdin);
				root = insert(root, line2);
			}while(line2[0] != '\n');		
		}else{
			printf("Enter lines (not case sensitive),to put on the tree until empty line\n");
			root = NULL;
			do{
				fgets(line2,100,stdin);
				root = insertNotSensitive(root,line2);
			}while(line2[0] != '\n');
			
		}
		
	}
	else if (err) {
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}

	
	if (optind < argc){
		/* these are the arguments, after the flags with their arguments */
		/* in other words we process the filename  taken from command line */
		for ( ; optind < argc; optind++){
			printf("argument: \"%s\"\n", argv[optind]);
	
			if(cflag == 1){
				ptrFile = fopen(argv[optind], "r");
				root = NULL;
				char line[100];
				int len;
				char line3[100];
				if(ptrFile != NULL){
					while(fgets(line, sizeof(line),ptrFile) != NULL){
						len = strlen(line);
						line[len - 1] = 0; /*removes newline*/
						root = insert(root,line);
					}
					fclose(ptrFile);
				} 			
			}
			else{
				ptrFile = fopen(argv[optind], "r");
				root = NULL;
				char line[100];
				int len;
				if(ptrFile != NULL){
					while(fgets(line, sizeof(line), ptrFile) != NULL){
						len = strlen(line);
						line[len - 1] = 0;
						root = insertNotSensitive(root,line);	
					}
					fclose(ptrFile);

				}
			}
	
		}
	}

	else {
		printf("no arguments left to process\n");
	}

	if(oflag == 1){
		/* output to a file */
		printToFile(root,oname);
	}
	else{
		/* output to the screen */
		inorder(root);
	}
	/* free memory of each node */
	freePointer(root);
	exit(0);
}
