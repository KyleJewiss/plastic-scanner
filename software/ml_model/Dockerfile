# Use the official Ubuntu 22.04 base image
FROM ubuntu:22.04

# Set environment variables to non-interactive
ENV DEBIAN_FRONTEND=noninteractive

COPY requirements.txt /home/ml_workspace/requirements.txt

# Install basic utilities
RUN apt-get update && \
    apt-get install -y \
    curl \
    wget \
    vim \
    python3 \
    python3-pip && \
    apt-get clean

# Set the working directory
WORKDIR /home/ml_workspace

# Install Python dependencies
RUN pip3 install -r requirements.txt

# Default command to keep the container running
CMD ["bash"]