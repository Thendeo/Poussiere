/*----------------------------------------------------
**
**		Date: 2019
**		Author: Thendeo
**		Project: Particules
**		File: AssertHdl implementation file
**
----------------------------------------------------*/
#include "AssertHdl.h"

#include <stdio.h>

extern "C"
{
	void assertionHandler(const char* pFileName, int pLine)
	{
		printf("Assertion occured inside %s, line %d", pFileName, pLine);

		while (1); // Place breakpoint here for debug
	}
}
