#!/bin/bash

invalidate=false
cache_dir="$HOME/.local/thirdparty"
thirdparty_sums_path="$HOME/.local/thirdparty_sums.md5"
thirdparty_yml_path="thirdparty.yml"
thirdparty_builder_path="$HOME/.local/thirdparty_builder"

cat $thirdparty_sums_path
md5sum -c $thirdparty_sums_path
if [[ ! $? -eq 0 ]]; then
    echo "Either our script has changed or thirdparty.yml has changed"
    invalidate=true
    md5sum $thirdparty_yml_path $0 > $thirdparty_sums_path
fi

if [[ ! -d $thirdparty_builder_path ]]; then
    invalidate=true

    # Clone the repository
    git clone git@github.com:/Lugdunum3D/Thirdparty-Builder.git $thirdparty_builder_path
    cd $thirdparty_builder_path

    # Setup the python environment
    virtualenv venv
else
    cd $thirdparty_builder_path

    # Pull latest changes
    git remote update

    if [[ $(git rev-parse HEAD) != $(git rev-parse @{u}) ]]; then
        invalidate=true
        git pull --rebase --autostash

        # Remove build cache
        rm -rf build
    fi
fi

# Exit if a command fails
set -e

if [[ ! -d "$cache_dir" || "$invalidate" = true ]]; then
    rm -rf "$cache_dir"
    mkdir "$cache_dir"

    cd $thirdparty_builder_path

    # Setup the python environment
    source ./venv/bin/activate
    pip install -r requirements.txt

    mkdir -p build
    cd build

    # Build thirdparty
    python ../build.py -vvv --path $cache_dir -z linux.zip ~/Lugdunum/thirdparty.yml

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
