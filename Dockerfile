FROM osrf/ros:humble-desktop-full-jammy AS deps

SHELL ["/bin/bash", "-c"]

# Creating ROS2 workspace (ws)
WORKDIR /root/ros2_ws

# Copying the node and messages over
COPY ./ROS2/Bridge/Server_Python/bridge_py_s /root/ros2_ws/src
COPY ./Misc/ROS2Msg /root/ros2_ws/src
COPY ./Misc/ProtoBuffers /root/ros2_ws/src

# Install general dependencies
RUN apt-get update
RUN apt-get install python3 -y 
RUN apt-get install python3-pip -y

# Preparing for WebSocket.
RUN apt-get install libwebsockets-dev -y
RUN pip install websockets

# Preparing for Protocol Buffers.
RUN apt-get install libwebsockets-dev -y
RUN apt-get install protobuf-c-compiler -y
RUN apt-get install libprotobuf-c-dev -y
RUN pip install protobuf

# Colcon the ws
FROM deps AS builder
ARG CMAKE_BUILD_TYPE=Release
RUN source /opt/ros/humble/setup.bash && colcon build --symlink-install

# Source the ROS2 setup file
RUN echo "source /root/ros2_ws/install/setup.bash" >> ~/.bashrc

# Run a default command, e.g., starting a bash shell
CMD ["bash"]
