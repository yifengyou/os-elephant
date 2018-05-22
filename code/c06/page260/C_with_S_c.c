extern void asm_print(char *, int);
void c_print(char* str)
{
	int len = 0;
	while (str[len++]);
	asm_print(str, len);
}
