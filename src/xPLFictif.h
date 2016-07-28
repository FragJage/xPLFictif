#ifndef XPLFICTIF_H
#define XPLFICTIF_H

#include <map>
#include "Service/Service.h"
#include "xPLLib/xPLDevCfg.h"
#include "xPLLib/Extensions/Sensors.h"
#include "xPLLib/Extensions/AdvanceConfig.h"
#include "xPLLib/Schemas/SchemaSensor.h"
#include "ControlExtension.h"

class xPLFictif : public Service::IService, public xPL::ControlExtension::IControlMsg, public xPL::AdvanceConfig::ICallBack
{
    public:
        xPLFictif();
        ~xPLFictif();

        void CallBackMsg(const std::string& device, const std::string& value);
        void AdvanceConfigure();
        void ConfigChange(const std::string& name);
        void ConfigDelete(const std::string& name);
        void ConfigsDelete();

		int ServiceStart(int argc, char* argv[]);
		void ServicePause(bool bPause);
		void ServiceStop();

    private:
        xPL::xPLDevCfg m_xPLDevice;
        xPL::AdvanceConfig m_AdvanceConfig;
        xPL::Sensors m_Sensors;
        xPL::ControlExtension m_ControlExtension;
        bool m_bServicePause;
        bool m_bServiceStop;
        map<string, string> m_DeviceMemo;
        std::string GetDataFileName();
        void SaveData();
        SimpleLog* m_Log;
};

#endif // XPLFICTIF_H
