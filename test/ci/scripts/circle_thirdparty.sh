#!/bin/bash

# Clone the repository
git clone https://github.com/Lugdunum3D/Lugdunum-ThirdParty.git ~/Lugdunum-ThirdParty
cd ~/Lugdunum-ThirdParty
git checkout automatization

# Setup the python environement
virtualenv venv
source ./venv/bin/activate
pip install -r requirements.txt

# Build thirdparty
python ./build.py -vvv --path ~/Lugdunum/thirdparty --build-types Release
