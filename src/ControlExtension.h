/*** LICENCE ***************************************************************************************/
/*
  xPPLib - Simple class to manage socket communication TCP or UDP

  This file is part of xPPLib.

    xPPLib is free software : you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    xPPLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with xPPLib.  If not, see <http://www.gnu.org/licenses/>.
*/
/***************************************************************************************************/

#ifndef CONTROLEXT_H
#define CONTROLEXT_H

#include "xPLLib/xPLDevice.h"

namespace xPL
{

class ControlExtension : public xPLDevice::IExtension
{
    public:
        class IControlMsg;
        ControlExtension(SimpleLog* log);
        ~ControlExtension();

        void SetCallBackMsg(IControlMsg* callBackMsg);
        bool MsgAnswer(SchemaObject& msg);

    private:
        IControlMsg* m_ControlMsg;
        SimpleLog* m_Log;

};

class ControlExtension::IControlMsg
{
    public:
        virtual void CallBackMsg(const std::string& device, const std::string& value) = 0;
};

}
#endif // CONTROLEXT_H
