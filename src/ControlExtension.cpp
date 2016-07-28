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

#include "ControlExtension.h"
namespace xPL
{

using namespace std;

/**************************************************************************************************************/
/***                                                                                                        ***/
/*** Class ControlExtension                                                                                           ***/
/***                                                                                                        ***/
/**************************************************************************************************************/
ControlExtension::ControlExtension(SimpleLog* log)
{
    m_ControlMsg = nullptr;
    m_Log = log;
}

ControlExtension::~ControlExtension()
{
}

void ControlExtension::SetCallBackMsg(IControlMsg* callBackMsg)
{
    m_ControlMsg = callBackMsg;
}

bool ControlExtension::MsgAnswer(SchemaObject& msg)
{
    string device;
    string current;


    if(msg.GetMsgType() != xPL::SchemaObject::cmnd) return false;
    if(msg.GetClass() != "control") return false;

    device = msg.GetValue("device");
    current = msg.GetValue("current");

    if(m_ControlMsg!=nullptr) m_ControlMsg->CallBackMsg(device, current);

    return true;
}

}
