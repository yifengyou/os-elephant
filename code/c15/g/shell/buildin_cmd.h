#ifndef __SHELL_BUILDIN_CMD_H
#define __SHELL_BUILDIN_CMD_H
#include "stdint.h"
void buildin_ls(uint32_t argc, char** argv);
char* buildin_cd(uint32_t argc, char** argv);
int32_t buildin_mkdir(uint32_t argc, char** argv);
int32_t buildin_rmdir(uint32_t argc, char** argv);
int32_t buildin_rm(uint32_t argc, char** argv);
void make_clear_abs_path(char* path, char* wash_buf);
void buildin_pwd(uint32_t argc, char** argv);
void buildin_ps(uint32_t argc, char** argv);
void buildin_clear(uint32_t argc, char** argv);
#endif
