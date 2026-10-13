#!/bin/bash
g++ -O3 src/package_manager/ngt.cpp -o src/package_manager/ngt
sudo cp src/package_manager/ngt /usr/bin/ngt
sudo chmod +x /usr/bin/ngt
