#include "TestxPLFictif.h"

using namespace std;

TestxPLFictif::TestxPLFictif() : TestClass("xPLFictif", this)
{
	addTest("Start", &TestxPLFictif::Start);
	addTest("StdConfig", &TestxPLFictif::StdConfig);
	addTest("SetAdvConfig", &TestxPLFictif::SetAdvConfig);
	addTest("FictifControl", &TestxPLFictif::FictifControl);
	addTest("FictifRequest", &TestxPLFictif::FictifRequest);
	addTest("GetAdvConfig", &TestxPLFictif::GetAdvConfig);
	addTest("ModifyAdvConfig", &TestxPLFictif::ModifyAdvConfig);
	addTest("Stop", &TestxPLFictif::Stop);
	addTest("ReStart", &TestxPLFictif::ReStart);
	addTest("DelAdvConfig", &TestxPLFictif::DelAdvConfig);
	addTest("ReStop", &TestxPLFictif::ReStop);

    if(remove("config")==0)
        cout << termcolor::yellow << "Remove old config file" << endl << termcolor::grey;
}

TestxPLFictif::~TestxPLFictif()
{
    if(remove("config")!=0)
        cout << termcolor::red << "Unable to remove config file" << endl << termcolor::grey;
}

void TestxPLFictif::ThreadStart(xPLFictif* pxPLDev)
{
    char exeName[] = "test";
    char mode[] = "/Console";
    char confName[] = "config";
    char* argv[3];

    argv[0]= exeName;
    argv[1]= mode;
    argv[2]= confName;
    pxPLDev->ServiceStart(3, argv);
}

bool TestxPLFictif::Start()
{
    string msg;
    xPL::SchemaObject sch;


    thread integrationTest(ThreadStart, &xPLDev);
    integrationTest.detach();

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("xPL Fictif"==sch.GetValue("appname"));

    return true;
}

bool TestxPLFictif::StdConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schCfg(xPL::SchemaObject::cmnd, "config", "response");

    schCfg.SetValue("interval", "30");
    schCfg.SetValue("newconf", "test");
    msg = schCfg.ToMessage("fragxpl-test.default", "fragxpl-fictif.default");
    SimpleSockUDP::SetNextRecv(msg);

    msg = SimpleSockUDP::GetLastSend(10);     //Pass Hbeat message
    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("30"==sch.GetValue("interval"));
    assert("fragxpl-fictif.test"==sch.GetSource());

    return true;
}

bool TestxPLFictif::SetAdvConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "request");

    schAdvCfg.SetValue("configname", "fictifinput");
    schAdvCfg.SetValue("type", "input");
    schAdvCfg.SetValue("value", "low");
    schAdvCfg.SetValue("save", "false");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("sensor"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("fictifinput"==sch.GetValue("device"));
    assert("low"==sch.GetValue("current"));

    schAdvCfg.SetValue("configname", "fictiftemp");
    schAdvCfg.SetValue("type", "temp");
    schAdvCfg.SetValue("value", "20");
    schAdvCfg.SetValue("save", "true");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("sensor"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("fictiftemp"==sch.GetValue("device"));
    assert("20"==sch.GetValue("current"));

    return true;
}

bool TestxPLFictif::FictifControl()
{
    xPL::SchemaObject schControl(xPL::ISchema::cmnd, "control", "basic");
    xPL::SchemaObject sch;
    string msg;

    schControl.SetValue("device", "fictiftemp");
    schControl.SetValue("current", "25");
    schControl.SetValue("type", "temp");
    msg = schControl.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert(xPL::SchemaObject::trig==sch.GetMsgType());
    assert("fictiftemp"==sch.GetValue("device"));
    assert("25"==sch.GetValue("current"));

    return true;
}

bool TestxPLFictif::FictifRequest()
{
    xPL::SchemaObject schSensor(xPL::ISchema::cmnd, "sensor", "request");
    xPL::SchemaObject sch;
    string msg;


    schSensor.SetValue("request", "current");
    schSensor.SetValue("device", "fictifinput");
    schSensor.SetValue("type", "input");
    msg = schSensor.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert(xPL::SchemaObject::stat==sch.GetMsgType());
    assert("fictifinput"==sch.GetValue("device"));
    assert("low"==sch.GetValue("current"));

    schSensor.SetValue("request", "current");
    schSensor.SetValue("device", "fictiftemp");
    schSensor.SetValue("type", "temp");
    msg = schSensor.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert(xPL::SchemaObject::stat==sch.GetMsgType());
    assert("fictiftemp"==sch.GetValue("device"));
    assert("25"==sch.GetValue("current"));

    return true;
}

bool TestxPLFictif::GetAdvConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "current");

    schAdvCfg.SetValue("command", "request");
    schAdvCfg.SetValue("configname", "fictiftemp");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("advanceconfig"==sch.GetClass());
    assert("current"==sch.GetType());
    assert("fictiftemp"==sch.GetValue("configname"));
    assert("temp"==sch.GetValue("type"));
    assert("20"==sch.GetValue("value"));
    assert("true"==sch.GetValue("save"));

    return true;
}

bool TestxPLFictif::ModifyAdvConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schAdvCfgReq(xPL::ISchema::cmnd, "advanceconfig", "request");
    xPL::SchemaObject schAdvCfgCur(xPL::ISchema::cmnd, "advanceconfig", "current");

    schAdvCfgReq.SetValue("configname", "fictifinput");
    schAdvCfgReq.SetValue("type", "input");
    schAdvCfgReq.SetValue("value", "low");
    schAdvCfgReq.SetValue("save", "true");
    msg = schAdvCfgReq.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("sensor"==sch.GetClass());
    assert("basic"==sch.GetType());
    assert("fictifinput"==sch.GetValue("device"));
    assert("low"==sch.GetValue("current"));

    schAdvCfgCur.SetValue("command", "request");
    schAdvCfgCur.SetValue("configname", "fictifinput");
    msg = schAdvCfgCur.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("advanceconfig"==sch.GetClass());
    assert("current"==sch.GetType());
    assert("fictifinput"==sch.GetValue("configname"));
    assert("input"==sch.GetValue("type"));
    assert("low"==sch.GetValue("value"));
    assert("true"==sch.GetValue("save"));

    return true;
}

bool TestxPLFictif::Stop()
{
    string msg;
    xPL::SchemaObject sch;

    xPLDev.ServicePause(true);
    Plateforms::delay(800);
    xPLDev.ServicePause(false);
    xPLDev.ServiceStop();

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("hbeat"==sch.GetClass());
    assert("end"==sch.GetType());
    Plateforms::delay(200);
    return true;
}

bool TestxPLFictif::ReStart()
{
    string msg;
    xPL::SchemaObject sch;

    thread integrationTest(ThreadStart, &xPLDev);
    integrationTest.detach();

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("xPL Fictif"==sch.GetValue("appname"));

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert(xPL::SchemaObject::trig==sch.GetMsgType());
    assert("fictifinput"==sch.GetValue("device"));
    assert("low"==sch.GetValue("current"));

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert(xPL::SchemaObject::trig==sch.GetMsgType());
    assert("fictiftemp"==sch.GetValue("device"));
    assert("25"==sch.GetValue("current"));

    return true;
}

bool TestxPLFictif::DelAdvConfig()
{
    string msg;
    xPL::SchemaObject sch;
    xPL::SchemaObject schAdvCfg(xPL::ISchema::cmnd, "advanceconfig", "current");

    schAdvCfg.SetValue("command", "delete");
    schAdvCfg.SetValue("configname", "fictifinput");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);
    Plateforms::delay(500);

    schAdvCfg.ClearValues();
    schAdvCfg.SetValue("command", "delete");
    msg = schAdvCfg.ToMessage("fragxpl-test.default", "fragxpl-fictif.test");
    SimpleSockUDP::SetNextRecv(msg);
    Plateforms::delay(500);

    return true;
}

bool TestxPLFictif::ReStop()
{
    string msg;
    xPL::SchemaObject sch;

    xPLDev.ServiceStop();

    msg = SimpleSockUDP::GetLastSend(10);
    sch.Parse(msg);
    assert("hbeat"==sch.GetClass());
    assert("end"==sch.GetType());
    Plateforms::delay(200);

    return true;
}

