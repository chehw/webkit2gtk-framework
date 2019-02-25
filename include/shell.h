#ifndef _SHELL_H_
#define _SHELL_H_


#include <stdio.h>
#ifdef _cplusplus
extern "C" {
#endif

typedef struct shell_ctx
{
	void * priv_data;
	void * user_data;
	
	int (* init)(struct shell_ctx * shell, void * user_data);
	int (* run)(struct shell_ctx * shell, int flags);
	void (* stop)(struct shell_ctx * shell);
	
}shell_ctx_t;

shell_ctx_t * shell_ctx_new(int * p_argc, char *** p_argv, void * user_data);
void shell_ctx_cleanup(shell_ctx_t * shell);


#ifdef _cplusplus
}
#endif
#endif
