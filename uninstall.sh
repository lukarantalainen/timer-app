#!/bin/bash
sudo rm /tmp/loggertest.socket
if [ ! -f /tmp/loggertest.socket ]; then
    echo "File not found!"
fi
