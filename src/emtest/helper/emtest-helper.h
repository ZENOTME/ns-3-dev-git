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
#ifndef EmTest_HELPER_H
#define EmTest_HELPER_H

#include "ns3/attribute.h"
#include "ns3/emtest-channel.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/queue.h"
#include "ns3/trace-helper.h"

#include <string>

namespace ns3
{

class Packet;

/**
 * \ingroup EmTest
 * \brief build a set of EmTestNetDevice objects
 *
 * Normally we eschew multiple inheritance, however, the classes
 * PcapUserHelperForDevice and AsciiTraceUserHelperForDevice are
 * treated as "mixins".  A mixin is a self-contained class that
 * encapsulates a general attribute or a set of functionality that
 * may be of interest to many other classes.
 */
class EmTestHelper 
{
  public:
    /**
     * Construct a EmTestHelper.
     */
    EmTestHelper();

    // ~EmTestHelper() override
    // {
    // }

    /**
     * \tparam Ts \deduced Argument types
     * \param type the type of queue
     * \param [in] args Name and AttributeValue pairs to set.
     *
     * Set the type of queue to create and associated to each
     * EmTestNetDevice created through EmTestHelper::Install.
     */
    template <typename... Ts>
    void SetQueue(std::string type, Ts&&... args);

    /**
     * \param n1 the name of the attribute to set
     * \param v1 the value of the attribute to set
     *
     * Set these attributes on each ns3::EmTestNetDevice created
     * by EmTestHelper::Install
     */
    void SetDeviceAttribute(std::string n1, const AttributeValue& v1);

    /**
     * \param n1 the name of the attribute to set
     * \param v1 the value of the attribute to set
     *
     * Set these attributes on each ns3::EmTestChannel created
     * by EmTestHelper::Install
     */
    void SetChannelAttribute(std::string n1, const AttributeValue& v1);

    /**
     * Disable flow control only if you know what you are doing. By disabling
     * flow control, this NetDevice will be sent packets even if there is no
     * room for them (such packets will be likely dropped by this NetDevice).
     * Also, any queue disc installed on this NetDevice will have no effect,
     * as every packet enqueued to the traffic control layer queue disc will
     * be immediately dequeued.
     */
    void DisableFlowControl();

    /**
     * This method creates an ns3::EmTestChannel with the attributes configured by
     * EmTestHelper::SetChannelAttribute, an ns3::EmTestNetDevice with the attributes
     * configured by EmTestHelper::SetDeviceAttribute and then adds the device
     * to the node and attaches the channel to the device.
     *
     * \param node The node to install the device in
     * \returns A container holding the added net device.
     */
    NetDeviceContainer Install(Ptr<Node> node) const;

    /**
     * This method creates an ns3::EmTestChannel with the attributes configured by
     * EmTestHelper::SetChannelAttribute, an ns3::EmTestNetDevice with the attributes
     * configured by EmTestHelper::SetDeviceAttribute and then adds the device
     * to the node and attaches the channel to the device.
     *
     * \param name The name of the node to install the device in
     * \returns A container holding the added net device.
     */
    NetDeviceContainer Install(std::string name) const;

    /**
     * This method creates an ns3::EmTestNetDevice with the attributes configured by
     * EmTestHelper::SetDeviceAttribute and then adds the device to the node and
     * attaches the provided channel to the device.
     *
     * \param node The node to install the device in
     * \param channel The channel to attach to the device.
     * \returns A container holding the added net device.
     */
    NetDeviceContainer Install(Ptr<Node> node, Ptr<EmTestChannel> channel) const;

    /**
     * This method creates an ns3::EmTestNetDevice with the attributes configured by
     * EmTestHelper::SetDeviceAttribute and then adds the device to the node and
     * attaches the provided channel to the device.
     *
     * \param node The node to install the device in
     * \param channelName The name of the channel to attach to the device.
     * \returns A container holding the added net device.
     */
    NetDeviceContainer Install(Ptr<Node> node, std::string channelName) const;

    /**
     * This method creates an ns3::EmTestNetDevice with the attributes configured by
     * EmTestHelper::SetDeviceAttribute and then adds the device to the node and
     * attaches the provided channel to the device.
     *
     * \param nodeName The name of the node to install the device in
     * \param channel The channel to attach to the device.
     * \returns A container holding the added net device.
     */
    NetDeviceContainer Install(std::string nodeName, Ptr<EmTestChannel> channel) const;

    /**
     * This method creates an ns3::EmTestNetDevice with the attributes configured by
     * EmTestHelper::SetDeviceAttribute and then adds the device to the node and
     * attaches the provided channel to the device.
     *
     * \param nodeName The name of the node to install the device in
     * \param channelName The name of the channel to attach to the device.
     * \returns A container holding the added net device.
     */
    NetDeviceContainer Install(std::string nodeName, std::string channelName) const;

    /**
     * This method creates an ns3::EmTestChannel with the attributes configured by
     * EmTestHelper::SetChannelAttribute.  For each Ptr<node> in the provided
     * container: it creates an ns3::EmTestNetDevice (with the attributes
     * configured by EmTestHelper::SetDeviceAttribute); adds the device to the
     * node; and attaches the channel to the device.
     *
     * \param c The NodeContainer holding the nodes to be changed.
     * \returns A container holding the added net devices.
     */
    NetDeviceContainer Install(const NodeContainer& c) const;

    /**
     * For each Ptr<node> in the provided container, this method creates an
     * ns3::EmTestNetDevice (with the attributes configured by
     * EmTestHelper::SetDeviceAttribute); adds the device to the node; and attaches
     * the provided channel to the device.
     *
     * \param c The NodeContainer holding the nodes to be changed.
     * \param channel The channel to attach to the devices.
     * \returns A container holding the added net devices.
     */
    NetDeviceContainer Install(const NodeContainer& c, Ptr<EmTestChannel> channel) const;

    /**
     * For each Ptr<node> in the provided container, this method creates an
     * ns3::EmTestNetDevice (with the attributes configured by
     * EmTestHelper::SetDeviceAttribute); adds the device to the node; and attaches
     * the provided channel to the device.
     *
     * \param c The NodeContainer holding the nodes to be changed.
     * \param channelName The name of the channel to attach to the devices.
     * \returns A container holding the added net devices.
     */
    NetDeviceContainer Install(const NodeContainer& c, std::string channelName) const;

    /**
     * Assign a fixed random variable stream number to the random variables
     * used by this model. Return the number of streams (possibly zero) that
     * have been assigned. The Install() method should have previously been
     * called by the user.
     *
     * \param c NetDeviceContainer of the set of net devices for which the
     *          EmTestNetDevice should be modified to use a fixed stream
     * \param stream first stream index to use
     * \return the number of stream indices assigned by this helper
     */
    int64_t AssignStreams(NetDeviceContainer c, int64_t stream);

  private:
    /**
     * This method creates an ns3::EmTestNetDevice with the attributes configured by
     * EmTestHelper::SetDeviceAttribute and then adds the device to the node and
     * attaches the provided channel to the device.
     *
     * \param node The node to install the device in
     * \param channel The channel to attach to the device.
     * \returns A container holding the added net device.
     */
    Ptr<NetDevice> InstallPriv(Ptr<Node> node, Ptr<EmTestChannel> channel) const;

    ObjectFactory m_queueFactory;   //!< factory for the queues
    ObjectFactory m_deviceFactory;  //!< factory for the NetDevices
    ObjectFactory m_channelFactory; //!< factory for the channel
    bool m_enableFlowControl;       //!< whether to enable flow control
};

} // namespace ns3

#endif /* EmTest_HELPER_H */