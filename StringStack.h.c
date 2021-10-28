#ifndef STRING_STACK
#define STRING_STACK

char* getStringStack()
{
	static char* string_stack[9];
}
int* getCurrentStringStackNumAddr()
{
	static int string_num = 0;
	return &string_num;
}
int getStringNum()
{
	return *getCurrentStringStackNumAddr();
}
void setStringNum(int new_val)
{
	*getCurrentStringStackNumAddr() = new_val;
}
void pushStringStack(const char* text)
{
	(getStringStack())[getStringNum()] = text;
	setStringNum(getStringNum()+1);
}
void popStringStack()
{
	(getStringStack())[getStringNum()] = "empty";
	setStringNum(getStringNum()-1);
}

#endif