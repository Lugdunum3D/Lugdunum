#!/bin/bash

# Exit if a command fails
set -e

invalidate=false
cache_dir="$HOME/.local/thirdparty"
thirdparty_builder_sha1_path="$HOME/.local/thirdparty_builder_sha1"
dependency="thirdparty.yml"
date_cache=$(date -r "$cache_dir" +%s)

if [[ -d "$cache_dir" ]]; then
    echo "$cache_dir is present"

    echo -ne "$cache_dir modification time: "
    date -r "$cache_dir"
    echo -ne "$dependency modification time: "
    date --date="$(git log -1 --format="%ai" -- $dependency)"
else
    echo "$cache_dir is missing, creating cache"
fi

# Invalidate from thirdparty.yml changed
date_thirdparty_yml=$(date --date="$(git log -1 --format="%ai" -- $dependency)" +%s)
echo "date_thirdparty_yml: $date_thirdparty_yml, date_cache: $date_cache"
if [[ $date_thirdparty_yml -gt $date_cache ]]; then
    echo "$dependency is newer than $cache_dir, invalidating cache"
    invalidate=true
else
    echo "$dependency is older than $cache_dir, cache is valid"
fi

# Invalidate from our own script that was changed
date_script=$(date --date="$(git log -1 --format="%ai" -- $0)" +%s)
echo "date_script: $date_script, date_cache: $date_cache"
if [[ $date_script -gt $date_cache ]]; then
    echo "$0 is newer than $cache_dir, invalidating cache"
    invalidate=true
else
    echo "$0 is older than $cache_dir, cache is valid"
fi

# Invalidate from ThirdParty-Builder
thirdparty_builder_remote_sha1=$(
    curl -sH 'User-Agent: Lugdunum3D/ThirdParty-Builder build script' \
    'https://api.github.com/repos/Lugdunum3D/ThirdParty-Builder/commits?page=1&per_page=1' \
    | python -c 'import sys, json; print(json.load(sys.stdin)[0]["sha"][:7])'
)

# Line below might seem a little hacky. If we don't have the file we want an empty
# string. But we also want a 0 return code, so we pipe to cat again to discard the error
cached_sha1=$(cat $thirdparty_builder_sha1_path 2>/dev/null | cat)

echo "cached_sha1: '$cached_sha1', thirdparty_builder_remote_sha1: '$thirdparty_builder_remote_sha1'"
if [[ $cached_sha1 != $thirdparty_builder_remote_sha1 ]]; then
    echo "ThirdParty-Builder differs, invalidating cache"
    echo "$thirdparty_builder_remote_sha1" > "$thirdparty_builder_sha1_path"
    invalidate=true
else
    echo "ThirdParty-Builder sha1 checks cache is valid"
fi


if [[ ! -d "$cache_dir" || "$invalidate" = true ]]; then
    rm -rf "$cache_dir"
    mkdir "$cache_dir"

    # Clone the repository
    git clone git@github.com:/Lugdunum3D/Thirdparty-Builder.git ~/Thirdparty-Builder
    cd ~/Thirdparty-Builder

    # Setup the python environment
    virtualenv venv
    source ./venv/bin/activate
    pip install -r requirements.txt

    # Build thirdparty
    python ./build.py -vvv --path $cache_dir -z linux.zip ~/Lugdunum/thirdparty.yml

    echo "Done building!"

    if [[ $CIRCLE_NODE_INDEX -eq 0 ]]; then
        thirdparty_short_sha1=$(git -C ~/Lugdunum log -n 1 --pretty=format:%h -- thirdparty.yml)

        echo "Uploading with sha1 $thirdparty_short_sha1..."

        echo -n $(md5sum linux.zip | cut -c -32) > linux.md5
        curl --user "upload:${UPLOAD_PASSWORD}" -T linux.md5 "https://thirdparty-dl.lugbench.eu/${thirdparty_short_sha1}/linux.md5"
        curl --user "upload:${UPLOAD_PASSWORD}" -T linux.zip "https://thirdparty-dl.lugbench.eu/${thirdparty_short_sha1}/linux.zip"

        echo "Done uploading! Zip available at https://thirdparty-dl.lugbench.eu/${thirdparty_short_sha1}/linux.zip"
    else
        echo "Skipping uploading (CIRCLE_NODE_INDEX is $CIRCLE_NODE_INDEX)"
    fi
fi
