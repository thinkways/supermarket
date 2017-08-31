#include <stdio.h>
#include <ctype.h>
#include "textmgr.h"

#define buflen 8
int buffp=-1;
int buff[buflen];

int igetch(FILE *fp){
	int ch;
	if(buffp>=0){
		ch=buff[buffp--];
	}else
		ch=fgetc(fp);
	return ch;
}

void iungetch(int ch){
	if(buffp<buflen-1)
		buff[++buffp]=ch;
	else
		puts("lack of buff");
}

int skipcomment(FILE *fp,int flag){
	int ch;
	if(flag=='/'){
		while(EOF!=(ch=igetch(fp))||'\n'!=ch)
			;
		return ch;
	}
	
	if(flag=='*'){
		int i,j;
		i=igetch(fp);
		j=igetch(fp);
		if(i==EOF||j==EOF)
			return EOF;
		while(i!='*'||j!='/'){
			i=j;
			j=igetch(fp);
			if(j==EOF)
				return EOF;
		}
		return igetch(fp);
	}
	return EOF;
}

int igetword(FILE *fp,char *word){
	int ch=0;
	int i=0;

	while(isspace(ch=igetch(fp)))//skip blank space
		;
	word[i++]=ch;
	if(ch==EOF)
		return ch;

	if(isdigit(ch)){
		for(ch=igetch(fp);isdigit(ch);ch=igetch(fp))
			word[i++]=ch;
		if(ch=='.'||ch=='e'||ch=='E'){
			word[i++]=ch;			
			for(ch=igetch(fp);isdigit(ch);ch=igetch(fp))
				word[i++]=ch;
		}
		iungetch(ch);
		word[i]='\0';
		return word[0];
	}

	if(isalpha(ch)||ch=='_'){
		for(ch=igetch(fp);isalpha(ch);ch=igetch(fp))
			word[i++]=ch;
		iungetch(ch);
		word[i]='\0';
		return word[0];
	}

	if(ch=='"'||ch=='\''){
		int flag=ch;
		for(ch=igetch(fp);ch!=flag;ch=igetch(fp)){
			word[i++]=ch;
		}
		word[i++]=flag;
		word[i]='\0';
		return word[0];
	}

	if(ch=='/'){
		ch=igetch(fp);
		if(ch=='/'||ch=='*'){
			word[--i]='\0';
			ch=skipcomment(fp,ch);
		}
		iungetch(ch);
		word[i]='\0';
		return word[0];
	}
	word[i]='\0';
	return word[0];
}