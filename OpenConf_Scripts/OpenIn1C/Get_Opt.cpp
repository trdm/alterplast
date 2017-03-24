#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "get_opt.h"

void debug(char *msg, ...);

char *SkipSpace(char *str)
{
	while( isspace(*(unsigned char*)str) ) str++;
	return str;
}

char **StrToArgv(char *CmdLine, int *Argc)
{
	char open_quote = 0;
	short slash = 0;
	int argc = 0;
	int arg = 0, argv_len = 0;
	char **argv = NULL, *buf = NULL;
	int i;
	#define PUT_CHAR(c) buf[argv_len++]=c;

	*Argc = 0;
	if( CmdLine==NULL ) return NULL;
	CmdLine = SkipSpace(CmdLine);

	for( i=strlen(CmdLine)-1; isspace((unsigned char)(CmdLine[i])); i-- )   // strip trailing blanks
		CmdLine[i]='\0';

	debug("\tdup commandline: ");
	buf = strdup(CmdLine);
	debug("%s\n", buf);
	if( buf==NULL) return NULL;

	for( ; *CmdLine!='\0'; CmdLine++ )
	{
		if( slash==1 )
		{
			switch( *CmdLine )
			{
				case '\"': PUT_CHAR('\"'); break;
				case '\'': PUT_CHAR('\''); break;
				/*
				case 'a': PUT_CHAR('\a'); break;
				case 'b': PUT_CHAR('\b'); break;
				case 'f': PUT_CHAR('\f'); break;
				case 'r': PUT_CHAR('\r'); break;
				case 't': PUT_CHAR('\t'); break;
				case 'v': PUT_CHAR('\v'); break;
				*/
				default: PUT_CHAR('\\'); PUT_CHAR(*CmdLine); break;
			}
			slash = 0;
			continue;
		}

		switch( *CmdLine )
		{
			case '\\': slash++; continue; break;
			case '\"':
			case '\'':
				if( (open_quote!=0) && (*CmdLine==open_quote) ) open_quote = 0;  // end quoted part
				else open_quote = *CmdLine;  // begin quoted part
				continue;
				break;
		}

		if( open_quote!=0 ) {PUT_CHAR(*CmdLine); continue;}  // character inside quotes

		if( isspace((unsigned char)*CmdLine) )
		{
			argc++;
			debug("\targc = %i\n", argc);
			PUT_CHAR('\0');
			CmdLine = SkipSpace(CmdLine) - 1;
			continue;
		}

		PUT_CHAR(*CmdLine);
	}
	//last argument
	argc++;
	PUT_CHAR('\0');

	int len;
	char *dest, *src;
	debug("\talloc memory for argv[]\n");
	argv = (char**)malloc((argc+1)*sizeof(char*) + argv_len);
	if( argv!=NULL )
	{
		dest = ((char*)(argv + argc)) + 1;
		src = buf;
		for( i=0; i<argc; i++ )
		{
			len = strlen(src) + 1;
			strcpy(dest, src);
			argv[i] = dest;
			dest += len;
			src += len;
		}
		// argv[argc] must be empty string
		argv[argc] = NULL;
	}
	debug("\tfree buffer\n");
	free(buf);

	*Argc = argc;
	return argv;
}


// return:
//		on success - option index
//		on NULL or zero length string - -1
//		on wrong option - -2
//		on non-option - -3.
int FindOpt(TOption *opts, char *opt)
{
	short OptType = 0; // 1 - short, 2 - long
	int len;
	int i;

	if( opt==NULL ) return -1;
	len = strlen(opt);
	if( len<2 ) return -1;
	if( opt[0]=='-' )
		if( opt[1]=='-' )
			OptType = 2;
		else
			OptType = 1;
	if( OptType==0 ) return -3;
	if( (OptType==1) && (len!=2) ) return -2;

	i = 0;
	while( (opts[i].OptS!=0) || (opts[i].OptL!=NULL) )
	{
		if( OptType==1 )
		{
			if( opt[1]==opts[i].OptS ) return i;
		}
		else
		{
			if( strcmp(opt+2, opts[i].OptL)==0 ) return i;
		}
		i++;
	}
	return -2;
}

//===================================================================================
//===========  GetOpt  ==============================================================
//===================================================================================
BOOL GetOpt(TGetOptData *data)
{
	if( *(data->argv)==NULL ) return FALSE;
	data->option_index = FindOpt(data->options, *(data->argv));
	data->argv++;            // shift to next arg
	data->unprocessed_arg++;
	data->option_arg = NULL;

	if( data->option_index>0 )
	{
		if( data->options[data->option_index].HaveArg && (*(data->argv)!=NULL) )
		{
			if( FindOpt(data->options, *(data->argv))==-3 ) data->option_arg = *(data->argv);
		}
	}
	return TRUE;
}
