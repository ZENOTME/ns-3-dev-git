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
 * Author: Emmanuelle Laprise<emmanuelle.laprise@bluekazoo.ca>
 */

#ifndef EmTest_CHANNEL_H
#define EmTest_CHANNEL_H

#include "ns3/channel.h"
#include "ns3/data-rate.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/address.h"

namespace ns3
{

class Packet;

class EmTestNetDevice;

/**
 * \ingroup EmTest
 * \brief EmTestNetDevice Record
 *
 * Stores the information related to each net device that is
 * connected to the channel.
 */
class EmTestDeviceRec
{
  public:
    Ptr<EmTestNetDevice> devicePtr; //!< Pointer to the net device
    bool active;                    //!< Is net device enabled to TX/RX

    EmTestDeviceRec();

    /**
     * \brief Constructor
     * Builds a record of the given NetDevice, its status is initialized to enabled.
     *
     * \param device the device to record
     */
    EmTestDeviceRec(Ptr<EmTestNetDevice> device);

    /**
     * Copy constructor
     * \param o the object to copy
     */
    EmTestDeviceRec(const EmTestDeviceRec& o);

    /**
     * \return If the net device pointed to by the devicePtr is active
     * and ready to RX/TX.
     */
    bool IsActive() const;
};

/**
 * \ingroup EmTest
 * \brief EmTest Channel.
 *
 * This class represents a simple EmTest channel that can be used
 * when many nodes are connected to one wire. It uses a single busy
 * flag to indicate if the channel is currently in use. It does not
 * take into account the distances between stations or the speed of
 * light to determine collisions.
 */
class EmTestChannel : public Channel
{
  public:
    /**
     * \brief Get the type ID.
     * \return the object TypeId
     */
    static TypeId GetTypeId();

    /**
     * \brief Create a EmTestChannel
     */
    EmTestChannel();
    /**
     * \brief Destroy a EmTestChannel
     */
    ~EmTestChannel() override;

    // Delete copy constructor and assignment operator to avoid misuse
    EmTestChannel(const EmTestChannel&) = delete;
    EmTestChannel& operator=(const EmTestChannel&) = delete;

    /**
     * \brief Attach a given netdevice to this channel
     *
     * \param device Device pointer to the netdevice to attach to the channel
     * \return The assigned device number
     */
    int32_t Attach(Ptr<EmTestNetDevice> device);

    /**
     * \brief Detach a given netdevice from this channel
     *
     * The net device is marked as inactive and it is not allowed to
     * receive or transmit packets
     *
     * \param device Device pointer to the netdevice to detach from the channel
     * \return True if the device is found and attached to the channel,
     * false if the device is not currently connected to the channel or
     * can't be found.
     */
    bool Detach(Ptr<EmTestNetDevice> device);

    /**
     * \brief Start transmitting a packet over the channel
     *
     * If the srcId belongs to a net device that is connected to the
     * channel, packet transmission begins, and the channel becomes busy
     * until the packet has completely reached all destinations.
     *
     * \param p A reference to the packet that will be transmitted over
     * the channel
     * \param srcId The device Id of the net device that wants to
     * transmit on the channel.
     * \return True if the channel is not busy and the transmitting net
     * device is currently active.
     */
    bool TransmitStart(Ptr<const Packet> p,
                       const Address& src,
                       const Address& dest,
                       uint16_t protocolNumber,
                       uint32_t srcId);

    /**
     * \return Returns the total number of devices including devices
     * that have been detached from the channel.
     */
    std::size_t GetNDevices() const override;

    /**
     * \return Get a NetDevice pointer to a connected network device.
     *
     * \param i The index of the net device.
     * \return Returns the pointer to the net device that is associated
     * with deviceId i.
     */
    Ptr<NetDevice> GetDevice(std::size_t i) const override;

  private:
    /**
     * List of the net devices that have been or are currently connected
     * to the channel.
     *
     * Devices are nor removed from this list, they are marked as
     * inactive. Otherwise the assigned device IDs will not refer to the
     * correct NetDevice. The DeviceIds are used so that it is possible
     * to have a number to refer to an entry in the list so that the
     * whole list does not have to be searched when making sure that a
     * source is attached to a channel when it is transmitting data.
     */
    std::vector<EmTestDeviceRec> m_deviceList;

    mutable std::mutex m_mutex;
};

} // namespace ns3

#endif /* EmTest_CHANNEL_H */
