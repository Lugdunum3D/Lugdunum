mkdir -p $HOME/.android # For sdkmanager configs
echo 'count=0' > $HOME/.android/repositories.cfg # Avoid warning

if [ ! -d "$ANDROID_SDK_ROOT" ]; then
    wget --quiet --output-document=android-sdk.zip https://dl.google.com/android/repository/sdk-tools-linux-3859397.zip
    mkdir "$ANDROID_SDK_ROOT"
    unzip -qq android-sdk.zip -d "$ANDROID_SDK_ROOT"
fi

echo y | $ANDROID_SDK_ROOT/tools/bin/sdkmanager 'ndk-bundle'
echo y | $ANDROID_SDK_ROOT/tools/bin/sdkmanager 'cmake;3.6.4111459'
echo y | $ANDROID_SDK_ROOT/tools/bin/sdkmanager --update
