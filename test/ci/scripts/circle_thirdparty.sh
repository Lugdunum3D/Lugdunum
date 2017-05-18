#!/bin/bash

invalidate=false
cache_dir="$HOME/.local/thirdparty"
dependency="thirdparty.yml"

if [[ -d "$cache_dir" ]]; then
    echo "$cache_dir is present"

    echo -ne "$cache_dir modification time: "
    date -r "$cache_dir"
    echo -ne "$dependency modification time: "
    date --date="$(git log -1 --format="%ai" -- $dependency)"
else
    echo "$cache_dir is missing, creating cache"
fi

date_script=$(date --date="$(git log -1 --format="%ai" -- $dependency)" +%s)
date_cache=$(date -r "$cache_dir" +%s)

echo "date_script: $date_script, date_cache: $date_cache"
if [[ $date_script -gt $date_cache ]]; then
    echo "$dependency is newer than $cache_dir, invalidating cache"
    invalidate=true
else
    echo "$dependency is older than $cache_dir, cache is valid"
fi

if [[ ! -d "$cache_dir" || "$invalidate" = true ]]; then
    rm -r "$cache_dir"
    mkdir "$cache_dir"

    # Clone the repository
    git clone https://github.com/Lugdunum3D/Lugdunum-ThirdParty.git ~/Lugdunum-ThirdParty
    cd ~/Lugdunum-ThirdParty

    # Setup the python environement
    virtualenv venv
    source ./venv/bin/activate
    pip install -r requirements.txt

    # Build thirdparty
    python ./build.py -vvv --build-types Release --path $cache_dir ~/Lugdunum/thirdparty.yml
fi
