#!/bin/bash

# 检查参数数量
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <number_of_bridges> <setup|teardown>"
    exit 1
fi

n=$1
action=$2

if [ "$action" == "setup" ]; then
    for i in $(seq 1 $n); do
        # 创建网桥
        ip link add br-$i type bridge
        
        # 创建tap设备
        ip tuntap add mode tap tap-$i
        
        # 设置tap设备
        ip link set tap-$i promisc on up
        ip link set dev tap-$i master br-$i
        ip link set dev br-$i up
        
        # 创建网络命名空间
        ip netns add ns-$i
        
        # 创建veth对
        ip link add veth-$i type veth peer name veth-$i-br
        
        # 将veth的一端放入命名空间
        ip link set veth-$i netns ns-$i
        
        # 连接veth的另一端到网桥
        ip link set dev veth-$i-br master br-$i
        ip link set dev veth-$i-br up
        
        # 在命名空间中设置veth
        ip netns exec ns-$i ip link set dev veth-$i up
        ip netns exec ns-$i ip address add 10.0.0.$i/24 dev veth-$i
        # ip netns exec ns-$i iperf3 -s -D 
    done
elif [ "$action" == "teardown" ]; then
    for i in $(seq 1 $n); do
        # 删除命名空间
        ip netns exec ns-$i pkill iperf3
        ip netns del ns-$i
        ip link del veth-$i-br
        # 删除网桥和tap设备
        ip link del br-$i
        ip link del tap-$i
    done
else
    echo "Invalid action. Use 'setup' or 'teardown'."
    exit 1
fi