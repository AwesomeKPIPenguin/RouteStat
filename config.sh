#!/bin/bash

sudo apt-get update
sudo apt-get upgrade
sudo apt-get install -y libtool pkg-config build-essential autoconf automake uuid-dev
sudo apt-get install libzmq3-dev postgresql postgresql-contrib libpqxx-4.0v5 libpq-dev libpqxx-dev