#include <stdio.h>
#include <generic_ui.H>
int main()
{
	printf("zzzzz\n");
	GenericUI::loadUI(string(getenv("PWD"))+"/fbdump_ui.so");
	if(GenericUI::attach("main").attach("checkUI").invoke(NULL)!=NULL) {
		printf("sssss\n");
	}
	return 0;
}

