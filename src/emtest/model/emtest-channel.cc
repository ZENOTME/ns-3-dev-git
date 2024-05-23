/*
 * Copyright (c) 2007 Emmanuelle Laprise
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
 */

#include "emtest-channel.h"

#include "emtest-net-device.h"

#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include <cassert>
#include <mutex>
#include <thread>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("EmTestChannel");

NS_OBJECT_ENSURE_REGISTERED(EmTestChannel);

TypeId
EmTestChannel::GetTypeId()
{
    static TypeId tid = TypeId("ns3::EmTestChannel")
                            .SetParent<Channel>()
                            .SetGroupName("EmTest")
                            .AddConstructor<EmTestChannel>();
    return tid;
}

EmTestChannel::EmTestChannel()
    : Channel()
{
    NS_LOG_FUNCTION_NOARGS();
    m_deviceList.clear();
}

EmTestChannel::~EmTestChannel()
{
    NS_LOG_FUNCTION(this);
    m_deviceList.clear();
}

Ptr<NetDevice>
EmTestChannel::GetDevice(std::size_t i) const
{
    return m_deviceList[i].devicePtr;
}

int32_t
EmTestChannel::Attach(Ptr<EmTestNetDevice> device)
{
    NS_LOG_FUNCTION(this << device);
    NS_ASSERT(device);

    // Only support attch two device for test
    NS_ASSERT(m_deviceList.size() < 2);

    EmTestDeviceRec rec(device);

    m_deviceList.push_back(rec);
    return (m_deviceList.size() - 1);
}

bool
EmTestChannel::Detach(Ptr<EmTestNetDevice> device)
{
    NS_LOG_FUNCTION(this << device);
    NS_ASSERT(device);

    for (auto it = m_deviceList.begin(); it < m_deviceList.end(); it++)
    {
        if ((it->devicePtr == device) && (it->active))
        {
            it->active = false;
            return true;
        }
    }
    return false;
}

bool
EmTestChannel::TransmitStart(Ptr<const Packet> p,
                             const Address& src,
                             const Address& dest,
                             uint16_t protocolNumber,
                             uint32_t srcId)
{
    static std::thread::id last_thread_id = std::this_thread::get_id();
    static Time last_time = Time(0);
    {
        std::unique_lock lock{m_mutex};
        auto now_time = Simulator::Now();
        if(now_time<last_time) {
            // std::cout<<"conflict: "<<last_thread_id<<" "<<last_time<<","<<std::this_thread::get_id()<<now_time<<std::endl;
        }
        last_time = now_time;
        last_thread_id = std::this_thread::get_id();

        auto tar_device = m_deviceList[srcId ^ 1];
        Simulator::ScheduleWithContext(tar_device.devicePtr->GetNode()->GetId(),
                                       Time(0),
                                       &EmTestNetDevice::Receive,
                                       tar_device.devicePtr,
                                       p,
                                       src,
                                       dest,
                                       protocolNumber);
        return true;
    }
}

std::size_t
EmTestChannel::GetNDevices() const
{
    return m_deviceList.size();
}

EmTestDeviceRec::EmTestDeviceRec()
{
    active = false;
}

EmTestDeviceRec::EmTestDeviceRec(Ptr<EmTestNetDevice> device)
{
    devicePtr = device;
    active = true;
}

EmTestDeviceRec::EmTestDeviceRec(const EmTestDeviceRec& deviceRec)
{
    devicePtr = deviceRec.devicePtr;
    active = deviceRec.active;
}

bool
EmTestDeviceRec::IsActive() const
{
    return active;
}

} // namespace ns3
