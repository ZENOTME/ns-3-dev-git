/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "emtest-helper.h"

#include "ns3/abort.h"
#include "ns3/config.h"
#include "ns3/emtest-channel.h"
#include "ns3/emtest-net-device.h"
#include "ns3/log.h"
#include "ns3/names.h"
#include "ns3/net-device-queue-interface.h"
#include "ns3/object-factory.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/trace-helper.h"

#include <string>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("EmTestHelper");

EmTestHelper::EmTestHelper()
{
    m_queueFactory.SetTypeId("ns3::DropTailQueue<Packet>");
    m_deviceFactory.SetTypeId("ns3::EmTestNetDevice");
    m_channelFactory.SetTypeId("ns3::EmTestChannel");
}

NetDeviceContainer
EmTestHelper::Install(const NodeContainer& c) const
{
    Ptr<EmTestChannel> channel = m_channelFactory.Create()->GetObject<EmTestChannel>();

    return Install(c, channel);
}

NetDeviceContainer
EmTestHelper::Install(const NodeContainer& c, Ptr<EmTestChannel> channel) const
{
    NetDeviceContainer devs;

    for (auto i = c.Begin(); i != c.End(); i++)
    {
        devs.Add(InstallPriv(*i, channel));
    }

    return devs;
}

Ptr<NetDevice>
EmTestHelper::InstallPriv(Ptr<Node> node, Ptr<EmTestChannel> channel) const
{
    Ptr<EmTestNetDevice> device = m_deviceFactory.Create<EmTestNetDevice>();
    device->SetAddress(Mac48Address::Allocate());
    node->AddDevice(device);
    Ptr<Queue<Packet>> queue = m_queueFactory.Create<Queue<Packet>>();
    device->SetQueue(queue);
    device->Attach(channel);
    if (m_enableFlowControl)
    {
        // Aggregate a NetDeviceQueueInterface object
        Ptr<NetDeviceQueueInterface> ndqi = CreateObject<NetDeviceQueueInterface>();
        ndqi->GetTxQueue(0)->ConnectQueueTraces(queue);
        device->AggregateObject(ndqi);
    }
    return device;
}

} // namespace ns3
