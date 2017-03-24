
#ifndef get_opt_h
#define get_opt_h
#include <windows.h>

#define OPT_NO_STRING  -1
#define OPT_WRONG      -2
#define OPT_NO_OPTION  -3


// option definition struct. Option may be in short, long or both forms.
// OptS==0 && OptL==NULL means end of options list.
typedef struct {
	char OptS;                           // Short option form. 0 - not defined
	char *OptL;                          // Long option form. NULL - not defined
	BOOL HaveArg;                        // If TRUE - option requires an argument
} TOption;

typedef struct {
	char **argv;                         // array of options. Shifts in each iteration
	int unprocessed_arg;                 // index of first unprocessed argument
	TOption *options;                    // list of available options
	int option_index;                    // option index, found in last iteration
	char *option_arg;                    // argument of last found option. Contains NULL if no argument found
} TGetOptData;
// Build argv from plain string
char **StrToArgv(char *CmdLine, int *Argc);

// Searches for option 'opt' in options list 'opts'
// return:
//		on success - option index
//		on NULL or zero length string - -1 (OPT_NO_STRING)
//		on wrong option - -2  (OPT_WRONG)
//		on non-option - -3    (OPT_NO_OPTION).
int FindOpt(TOption *opts, char *opt);

// One iteration of parsing option list.
// Return FALSE when end of 'data.argv' reached
// data.argv, data.unprocessed_arg and data.options must be initialized before first call to GetOpt
BOOL GetOpt(TGetOptData *data);
#endif
