/*----------------------------------------------------
**
**		Date: 2019
**		Author: Alexis LAFONT
**		Project: Particules
**		File: AssertHdl header file
**
----------------------------------------------------*/

extern "C" {
	void assertionHandler(const char* pFileName, int pLine);
}

#define doAssert(Condition) ((Condition) ? (void)0 : assertionHandler(__FILE__,__LINE__ ))