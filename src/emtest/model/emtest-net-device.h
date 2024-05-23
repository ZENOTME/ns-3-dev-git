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
 * Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca
 */

#ifndef EmTest_NET_DEVICE_H
#define EmTest_NET_DEVICE_H

#include "ns3/address.h"
#include "ns3/callback.h"
#include "ns3/data-rate.h"
#include "ns3/mac48-address.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include "ns3/ptr.h"
#include "ns3/queue-fwd.h"
#include "ns3/traced-callback.h"

#include <cstring>

namespace ns3
{

class EmTestChannel;
class ErrorModel;

/**
 * \defgroup EmTest EmTest Network Device
 *
 * This section documents the API of the ns-3 EmTest module. For a generic functional description,
 * please refer to the ns-3 manual.
 */

/**
 * \ingroup EmTest
 * \class EmTestNetDevice
 * \brief A Device for a EmTest Network Link.
 *
 * The EmTest net device class is analogous to layer 1 and 2 of the
 * TCP stack. The NetDevice takes a raw packet of bytes and creates a
 * protocol specific packet from them.
 */
class EmTestNetDevice : public NetDevice
{
  public:
    /**
     * \brief Get the type ID.
     * \return the object TypeId
     */
    static TypeId GetTypeId();

    /**
     * Enumeration of the types of packets supported in the class.
     */
    enum EncapsulationMode
    {
        ILLEGAL, /**< Encapsulation mode not set */
        DIX,     /**< DIX II / Ethernet II packet */
        LLC,     /**< 802.2 LLC/SNAP Packet*/
    };

    /**
     * Construct a EmTestNetDevice
     *
     * This is the default constructor for a EmTestNetDevice.
     */
    EmTestNetDevice();

    /**
     * Destroy a EmTestNetDevice
     *
     * This is the destructor for a EmTestNetDevice.
     */
    ~EmTestNetDevice() override;

    /**
     * Attach the device to a channel.
     *
     * The function Attach is used to add a EmTestNetDevice to a EmTestChannel.
     *
     * \see SetDataRate ()
     * \see SetInterframeGap ()
     * \param ch a pointer to the channel to which this object is being attached.
     * \returns true if no error
     */
    bool Attach(Ptr<EmTestChannel> ch);

    /**
     * Attach a queue to the EmTestNetDevice.
     *
     * The EmTestNetDevice "owns" a queue.  This queue may be set by higher
     * level topology objects to implement a particular queueing method such as
     * DropTail.
     *
     * \see Queue
     * \see DropTailQueue
     * \param queue a Ptr to the queue for being assigned to the device.
     */
    void SetQueue(Ptr<Queue<Packet>> queue);

    /**
     * Get a copy of the attached Queue.
     *
     * \return a pointer to the queue.
     */
    Ptr<Queue<Packet>> GetQueue() const;

    /**
     * Attach a receive ErrorModel to the EmTestNetDevice.
     *
     * The EmTestNetDevice may optionally include an ErrorModel in
     * the packet receive chain to simulate data errors in during transmission.
     *
     * \see ErrorModel
     * \param em a pointer to the ErrorModel
     */
    void SetReceiveErrorModel(Ptr<ErrorModel> em);

    /**
     * Receive a packet from a connected EmTestChannel.
     *
     * The EmTestNetDevice receives packets from its connected channel
     * and forwards them up the protocol stack.  This is the public method
     * used by the channel to indicate that the last bit of a packet has
     * arrived at the device.
     *
     * \see EmTestChannel
     * \param p a reference to the received packet
     * \param sender the EmTestNetDevice that transmitted the packet in the first place
     */
    void Receive(Ptr<const Packet> packet,
                 const Address& src,
                 const Address& dest,
                 uint16_t protocolNumber);

    /**
     * Is the send side of the network device enabled?
     *
     * \returns True if the send side is enabled, otherwise false.
     */
    bool IsSendEnabled() const;

    /**
     * Enable or disable the send side of the network device.
     *
     * \param enable Enable the send side if true, otherwise disable.
     */
    void SetSendEnable(bool enable);

    /**
     * Is the receive side of the network device enabled?
     *
     * \returns True if the receiver side is enabled, otherwise false.
     */
    bool IsReceiveEnabled() const;

    /**
     * Enable or disable the receive side of the network device.
     *
     * \param enable Enable the receive side if true, otherwise disable.
     */
    void SetReceiveEnable(bool enable);

    /**
     * Set the encapsulation mode of this device.
     *
     * \param mode The encapsulation mode of this device.
     *
     */
    void SetEncapsulationMode(EmTestNetDevice::EncapsulationMode mode);

    /**
     * Get the encapsulation mode of this device.
     *
     * \returns The encapsulation mode of this device.
     */
    EmTestNetDevice::EncapsulationMode GetEncapsulationMode();

    //
    // The following methods are inherited from NetDevice base class.
    //
    void SetIfIndex(const uint32_t index) override;
    uint32_t GetIfIndex() const override;
    Ptr<Channel> GetChannel() const override;
    bool SetMtu(const uint16_t mtu) override;
    uint16_t GetMtu() const override;
    void SetAddress(Address address) override;
    Address GetAddress() const override;
    bool IsLinkUp() const override;
    void AddLinkChangeCallback(Callback<void> callback) override;
    bool IsBroadcast() const override;
    Address GetBroadcast() const override;
    bool IsMulticast() const override;

    /**
     * \brief Make and return a MAC multicast address using the provided
     *        multicast group
     *
     * \RFC{1112} says that an Ipv4 host group address is mapped to an Ethernet
     * multicast address by placing the low-order 23-bits of the IP address into
     * the low-order 23 bits of the Ethernet multicast address
     * 01-00-5E-00-00-00 (hex).
     *
     * This method performs the multicast address creation function appropriate
     * to an EUI-48-based EmTest device.  This MAC address is encapsulated in an
     *  abstract Address to avoid dependencies on the exact address format.
     *
     * \param multicastGroup The IP address for the multicast group destination
     * of the packet.
     * \return The MAC multicast Address used to send packets to the provided
     * multicast group.
     *
     * \see Ipv4Address
     * \see Mac48Address
     * \see Address
     */
    Address GetMulticast(Ipv4Address multicastGroup) const override;

    /**
     * Is this a point to point link?
     * \returns false.
     */
    bool IsPointToPoint() const override;

    /**
     * Is this a bridge?
     * \returns false.
     */
    bool IsBridge() const override;

    /**
     * Start sending a packet down the channel.
     * \param packet packet to send
     * \param dest layer 2 destination address
     * \param protocolNumber protocol number
     * \return true if successful, false otherwise (drop, ...)
     */
    bool Send(Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber) override;

    /**
     * Start sending a packet down the channel, with MAC spoofing
     * \param packet packet to send
     * \param source layer 2 source address
     * \param dest layer 2 destination address
     * \param protocolNumber protocol number
     * \return true if successful, false otherwise (drop, ...)
     */
    bool SendFrom(Ptr<Packet> packet,
                  const Address& source,
                  const Address& dest,
                  uint16_t protocolNumber) override;

    /**
     * Get the node to which this device is attached.
     *
     * \returns Ptr to the Node to which the device is attached.
     */
    Ptr<Node> GetNode() const override;

    /**
     * Set the node to which this device is being attached.
     *
     * \param node Ptr to the Node to which the device is being attached.
     */
    void SetNode(Ptr<Node> node) override;

    /**
     * Does this device need to use the address resolution protocol?
     *
     * \returns True if the encapsulation mode is set to a value that requires
     * ARP (IP_ARP or LLC).
     */
    bool NeedsArp() const override;

    /**
     * Set the callback to be used to notify higher layers when a packet has been
     * received.
     *
     * \param cb The callback.
     */
    void SetReceiveCallback(NetDevice::ReceiveCallback cb) override;

    /**
     * \brief Get the MAC multicast address corresponding
     * to the IPv6 address provided.
     * \param addr IPv6 address
     * \return the MAC multicast address
     * \warning Calling this method is invalid if IsMulticast returns not true.
     */
    Address GetMulticast(Ipv6Address addr) const override;

    void SetPromiscReceiveCallback(PromiscReceiveCallback cb) override;
    bool SupportsSendFrom() const override;

  protected:
    /**
     * Perform any object release functionality required to break reference
     * cycles in reference counted objects held by the device.
     */
    void DoDispose() override;

  private:
    /**
     * Operator = is declared but not implemented.  This disables the assignment
     * operator for EmTestNetDevice objects.
     * \param o object to copy
     * \returns the copied object
     */
    EmTestNetDevice& operator=(const EmTestNetDevice& o);

    /**
     * Copy constructor is declared but not implemented.  This disables the
     * copy constructor for EmTestNetDevice objects.
     * \param o object to copy
     */
    EmTestNetDevice(const EmTestNetDevice& o);

    /**
     * The channel to which this device is attached.
     */
    Ptr<EmTestChannel> m_channel;

    /**
     * Device ID returned by the attached functions. It is used by the
     * mp-channel to identify each net device to make sure that only
     * active net devices are writing to the channel
     */
    uint32_t m_deviceId;

    /**
     * The callback used to notify higher layers that a packet has been received.
     */
    NetDevice::ReceiveCallback m_rxCallback;

    /**
     * The callback used to notify higher layers that a packet has been received in promiscuous
     * mode.
     */
    NetDevice::PromiscReceiveCallback m_promiscRxCallback;

    Ptr<Node> m_node;
};

} // namespace ns3

#endif /* EmTest_NET_DEVICE_H */
