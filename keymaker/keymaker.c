// keymaker.cpp: 定义应用程序的入口点。
//

#include "keygen.h"

int main()
{
	char* mathid;
	char* key;
	char* passwd;
	int ver = 12;
	int hash = 0;

	printf("Input Version of Mathematica(11|12): ");
	if (scanf("%d", &ver) != 1)
	{
		printf("Wrong Input!\n");
		PAUSE;
		return 0;
	}
	switch (ver)
	{
	case 11:
		hash = HASH11[3];
		break;
	case 12:
		hash = HASH12[0];
		break;
	default:
		printf("Unsupport Version!\n");
		PAUSE;
		return 0;
	}

	mathid = (char*)malloc(256);
	printf("Input MathID: ");
	if (scanf("%s", mathid) != 1)
	{
		printf("Wrong Input!\n");
		PAUSE;
		exit(0);
	}

	if (!checkMathID(mathid))
	{
		printf("Wrong MathID!\n");
		PAUSE;
		exit(0);
	}
	
	key = randKey();
	passwd = genPasswd(mathid,key,hash);

	printf("MathID: \n%s\n", mathid);
	printf("Activation Key: \n%s\n", key);
	printf("Password: \n%s\n", passwd);
	PAUSE;
	return 0;
}
