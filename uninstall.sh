#!/bin/bash

if sudo rm /tmp/loggertest.socket; then
    echo "Socket removed successfully."
else 
    echo "Failed to remove socket."
fi
