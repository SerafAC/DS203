#include <stdio.h>
#include <string.h>

char buf[1<<20];


int main(int argc, char **argv)
{
	const char *repl=argv[1];
	int line=0;
	int found=0;
	char *l; 
	printf("#pragma once\n");
	printf("#ifndef %s\n",repl);
	printf("#define %s\n",repl);
	for(;;)
	{		
		l=fgets(buf,sizeof(buf),stdin);
		line++;
		if (!l) break;
		if ((line==1) && strstr(buf,"#pragma once")) 
		{
			line=0;
		}
		else if ((line==1) && strstr(buf,"#ifndef")) 
		{
			found=1;
		}
		else if ((line==2) && strstr(buf,"#define")) 
		{
			if (!found) fputs(buf,stdout);
		}
		else 
		{
			fputs(buf,stdout);
		}
	}
	if (!found) printf("#endif\n");
}
