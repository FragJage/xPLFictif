#include "SimpleFolders/SimpleFolders.h"
#include "SimpleIni/SimpleIni.h"
#include "Plateforms/Plateforms.h"
#include "xPLFictif.h"

using namespace std;

xPLFictif::xPLFictif() :    m_AdvanceConfig(&m_xPLDevice),
                            m_Sensors(&m_xPLDevice),
                            m_ControlExtension(m_xPLDevice.GetLogHandle())

{
    m_Log = m_xPLDevice.GetLogHandle();

    m_AdvanceConfig.AddFormat("configname", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.AddFormat("type", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::SENSORTYPE);
    m_AdvanceConfig.AddFormat("value", xPL::AdvanceConfig::ParamType::STRING, xPL::AdvanceConfig::ParamList::NONE);
    m_AdvanceConfig.AddFormat("save", xPL::AdvanceConfig::ParamType::BOOLEAN, xPL::AdvanceConfig::ParamList::NONE);

    m_AdvanceConfig.SetCallBack(this);
    m_ControlExtension.SetCallBackMsg(this);

    m_xPLDevice.AddExtension(&m_Sensors);
    m_xPLDevice.AddExtension(&m_ControlExtension);
    m_xPLDevice.AddExtension(&m_AdvanceConfig);

    m_xPLDevice.Initialisation("fragxpl", "fictif", "default");
    m_xPLDevice.SetAppName("xPL Fictif", "1.1.0.0");
    m_bServicePause = false;
    m_bServiceStop = false;
}

xPLFictif::~xPLFictif()
{
}

void xPLFictif::CallBackMsg(const string& device, const string& current)
{
    if(m_DeviceMemo.find(device)!=m_DeviceMemo.end()) m_DeviceMemo[device] = current;
    m_Sensors.ModifyMessage(device, current);
}

void xPLFictif::ConfigChange(const string& device)
{
    std::map<std::string, std::string>* config;
    string name;
    string type;
    string value;
    string memo;
    xPL::SchemaSensorBasic* pSensorBasic;


    config = m_AdvanceConfig.GetConfig(device);

    name = (*config)["configname"];
    type = (*config)["type"];
    value= (*config)["value"];

    if((*config)["save"]=="true")
        m_DeviceMemo[name] = value;

    if(!m_Sensors.ModifyMessage(name, value, type))
    {
        pSensorBasic = new xPL::SchemaSensorBasic(name, xPL::SchemaSensorTypeUtility::toSensorType(type), value);
        m_Sensors.AddMessage(pSensorBasic);
    }
}

void xPLFictif::ConfigDelete(const string& name)
{
    m_Sensors.RemoveMessage(name);
}

void xPLFictif::ConfigsDelete()
{
    m_Sensors.RemoveAllMessages();
}

void xPLFictif::AdvanceConfigure()
{
    int i;
    int nb;
    string dataFileName;
    SimpleIni dataFile;


    LOG_ENTER;

    nb = m_AdvanceConfig.GetNbConfig();
    if(nb>0) SaveData();
    m_Sensors.RemoveAllMessages();

    dataFileName = GetDataFileName();
	LOG_VERBOSE(m_Log) << "DataFileName : " << dataFileName;

	dataFile.SetOptions(dataFile.Comment, "#");
    dataFile.Load(dataFileName);

    for(i=0; i<nb; i++)
    {
        std::map<std::string, std::string>* config;
        string name;
        string type;
        string value;
        string memo;
        xPL::SchemaSensorBasic* pSensorBasic;


        config = m_AdvanceConfig.GetConfig(i);

        name = (*config)["configname"];
        type = (*config)["type"];
        value= (*config)["value"];

        if((*config)["save"]=="true")
        {
            memo = dataFile.GetValue("Values", name, "#NOTHING");
            if(memo!="#NOTHING") value = memo;
            m_DeviceMemo[name] = value;
        }

        pSensorBasic = new xPL::SchemaSensorBasic(name, xPL::SchemaSensorTypeUtility::toSensorType(type), value);
        m_Sensors.AddMessage(pSensorBasic);
    }

	LOG_EXIT_OK;
}

string xPLFictif::GetDataFileName()
{
    SimpleFolders   folder;
    string          fileName;

    try
    {
        fileName = folder.AddFile(m_xPLDevice.GetConfigFolder(), "xPLFictif.data");
    }
    catch(char const* msg)
    {
        LOG_ERROR(m_Log) << "Unable to get user folder : "<< msg;
        fileName = "xPLFictif.data";
    }

    return fileName;
}

void xPLFictif::SaveData()
{
    SimpleIni dataFile;
    map<string, string>::iterator it;
    map<string, string>::iterator itEnd;

    if(m_DeviceMemo.size()==0) return;
	LOG_VERBOSE(m_Log) << "Save data";

    itEnd = m_DeviceMemo.end();
    for(it=m_DeviceMemo.begin(); it!=itEnd; ++it)
        dataFile.SetValue("Values", it->first, it->second);

    dataFile.SaveAs(GetDataFileName());
}

int xPLFictif::ServiceStart(int argc, char* argv[])
{
    m_bServiceStop = false;
    if(argc > 2) m_xPLDevice.SetConfigFileName(argv[2]);
    m_xPLDevice.Open();

    while(!m_bServiceStop)
    {
        if(m_bServicePause)
            Plateforms::delay(500);
        else
            m_xPLDevice.WaitRecv(500);
    }

    m_xPLDevice.Close();
    SaveData();
    return 0;
}

void xPLFictif::ServicePause(bool bPause)
{
    m_bServicePause = bPause;
}

void xPLFictif::ServiceStop()
{
    m_bServiceStop = true;
}
