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

#include "emtest-net-device.h"

#include "emtest-channel.h"

#include "ns3/boolean.h"
#include "ns3/enum.h"
#include "ns3/error-model.h"
#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/llc-snap-header.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/uinteger.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("EmTestNetDevice");

NS_OBJECT_ENSURE_REGISTERED(EmTestNetDevice);

TypeId
EmTestNetDevice::GetTypeId()
{
    static TypeId tid = TypeId("ns3::EmTestNetDevice")
                            .SetParent<NetDevice>()
                            .SetGroupName("EmTest")
                            .AddConstructor<EmTestNetDevice>();
    return tid;
}

EmTestNetDevice::EmTestNetDevice()
{
    NS_LOG_FUNCTION(this);
    m_channel = nullptr;
}

EmTestNetDevice::~EmTestNetDevice()
{
    NS_LOG_FUNCTION_NOARGS();
}

void
EmTestNetDevice::DoDispose()
{
    NS_LOG_FUNCTION_NOARGS();
    m_channel = nullptr;
    NetDevice::DoDispose();
}

bool
EmTestNetDevice::Attach(Ptr<EmTestChannel> ch)
{
    NS_LOG_FUNCTION(this << &ch);
    m_channel = ch;
    m_deviceId = m_channel->Attach(this);
    return true;
}

void
EmTestNetDevice::Receive(Ptr<const Packet> packet,
                         const Address& src,
                         const Address& dest,
                         uint16_t protocolNumber)
{
    //
    // For all kinds of packetType we receive, we hit the promiscuous sniffer
    // hook and pass a copy up to the promiscuous callback.  Pass a copy to
    // make sure that nobody messes with our packet.
    //
    if (!m_promiscRxCallback.IsNull())
    {
        m_promiscRxCallback(this, packet, protocolNumber, src, dest, PacketType::PACKET_HOST);
    }
}

Ptr<Queue<Packet>>
EmTestNetDevice::GetQueue() const
{
    NS_LOG_FUNCTION_NOARGS();
    return nullptr;
}

void
EmTestNetDevice::SetIfIndex(const uint32_t index)
{
    NS_LOG_FUNCTION(index);
}

uint32_t
EmTestNetDevice::GetIfIndex() const
{
    NS_LOG_FUNCTION_NOARGS();
    return 0;
}

Ptr<Channel>
EmTestNetDevice::GetChannel() const
{
    NS_LOG_FUNCTION_NOARGS();
    return m_channel;
}

void
EmTestNetDevice::SetAddress(Address address)
{
    NS_LOG_FUNCTION_NOARGS();
}

Address
EmTestNetDevice::GetAddress() const
{
    NS_LOG_FUNCTION_NOARGS();
    return Mac48Address::Allocate();
}

bool
EmTestNetDevice::IsLinkUp() const
{
    NS_LOG_FUNCTION_NOARGS();
    return false;
}

void
EmTestNetDevice::AddLinkChangeCallback(Callback<void> callback)
{
    NS_LOG_FUNCTION(&callback);
}

bool
EmTestNetDevice::IsBroadcast() const
{
    NS_LOG_FUNCTION_NOARGS();
    return true;
}

Address
EmTestNetDevice::GetBroadcast() const
{
    NS_LOG_FUNCTION_NOARGS();
    return Mac48Address("ff:ff:ff:ff:ff:ff");
}

bool
EmTestNetDevice::IsMulticast() const
{
    NS_LOG_FUNCTION_NOARGS();
    return true;
}

Address
EmTestNetDevice::GetMulticast(Ipv4Address multicastGroup) const
{
    NS_LOG_FUNCTION(multicastGroup);

    Mac48Address ad = Mac48Address::GetMulticast(multicastGroup);

    //
    // Implicit conversion (operator Address ()) is defined for Mac48Address, so
    // use it by just returning the EUI-48 address which is automagically converted
    // to an Address.
    //
    NS_LOG_LOGIC("multicast address is " << ad);

    return ad;
}

bool
EmTestNetDevice::IsPointToPoint() const
{
    NS_LOG_FUNCTION_NOARGS();
    return false;
}

bool
EmTestNetDevice::IsBridge() const
{
    NS_LOG_FUNCTION_NOARGS();
    return false;
}

bool
EmTestNetDevice::Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
    NS_LOG_FUNCTION(packet << dest << protocolNumber);
    return SendFrom(packet, Address(), dest, protocolNumber);
}

bool
EmTestNetDevice::SendFrom(Ptr<Packet> packet,
                          const Address& src,
                          const Address& dest,
                          uint16_t protocolNumber)
{
    m_channel->TransmitStart(packet, src, dest, protocolNumber, m_deviceId);
    return true;
}

Ptr<Node>
EmTestNetDevice::GetNode() const
{
    NS_LOG_FUNCTION_NOARGS();
    return m_node;
}

void
EmTestNetDevice::SetNode(Ptr<Node> node)
{
    NS_LOG_FUNCTION(node);

    m_node = node;
}

void
EmTestNetDevice::SetQueue(Ptr<Queue<Packet>> queue)
{
}

bool
EmTestNetDevice::SetMtu(const uint16_t mtu)
{
    return true;
}

uint16_t
EmTestNetDevice::GetMtu() const
{
    return 1500;
}

bool
EmTestNetDevice::NeedsArp() const
{
    NS_LOG_FUNCTION_NOARGS();
    return true;
}

void
EmTestNetDevice::SetReceiveCallback(NetDevice::ReceiveCallback cb)
{
    NS_LOG_FUNCTION(&cb);
    m_rxCallback = cb;
}

Address
EmTestNetDevice::GetMulticast(Ipv6Address addr) const
{
    Mac48Address ad = Mac48Address::GetMulticast(addr);

    NS_LOG_LOGIC("MAC IPv6 multicast address is " << ad);
    return ad;
}

void
EmTestNetDevice::SetPromiscReceiveCallback(NetDevice::PromiscReceiveCallback cb)
{
    NS_LOG_FUNCTION(&cb);
    m_promiscRxCallback = cb;
    printf("EmTestNetDevice::SetPromiscReceiveCallback\n");
}

bool
EmTestNetDevice::SupportsSendFrom() const
{
    NS_LOG_FUNCTION_NOARGS();
    return true;
}
} // namespace ns3
