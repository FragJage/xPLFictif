#include <cassert>
#include <thread>
#include "xPLLib/Schemas/SchemaObject.h"
#include "Plateforms/Plateforms.h"
#include "UnitTest/UnitTest.h"
#ifndef _GLIBCXX_HAS_GTHREADS
    #include "Thread/mingw.thread.h"
#else
    #include <thread>
#endif
#include "../src/xPLFictif.h"


class TestxPLFictif : public TestClass<TestxPLFictif>
{
public:
    TestxPLFictif();
    ~TestxPLFictif();
    static void ThreadStart(xPLFictif* pxPLDev);
    bool Start();
    bool StdConfig();
    bool SetAdvConfig();
    bool FictifControl();
    bool FictifRequest();
    bool GetAdvConfig();
    bool ModifyAdvConfig();
    bool Stop();
    bool ReStart();
    bool DelAdvConfig();
    bool ReStop();
    xPLFictif xPLDev;
};
