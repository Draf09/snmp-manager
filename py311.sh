#!/bin/bash

# Update package list
sudo apt update

# Install necessary dependencies
sudo apt install -y build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev libsqlite3-dev wget libbz2-dev

# Download Python 3.11 source code
wget https://www.python.org/ftp/python/3.11.0/Python-3.11.0.tgz

# Extract the downloaded archive
tar -xf Python-3.11.0.tgz

# Change to the Python 3.11 directory
cd Python-3.11.0

# Configure and install Python 3.11
./configure --enable-optimizations
make -j $(nproc)
sudo make altinstall

# Clean up the downloaded archive and source directory
cd ..
rm -rf Python-3.11.0.tgz Python-3.11.0

# Create a virtual environment using Python 3.11
python3.11 -m venv myenv

# Activate the virtual environment
source myenv/bin/activate

echo "Python 3.11 and virtual environment (myenv) have been set up successfully!"