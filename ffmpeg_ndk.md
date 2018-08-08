
```

#!/bin/bash

######################################################
# Usage:
#   put this script in top of FFmpeg source tree
#   ./build_android
#
# It generates binary for following architectures:
#     ARMv6 
#     ARMv6+VFP 
#     ARMv7+VFPv3-d16 (Tegra2) 
#     ARMv7+Neon (Cortex-A8)
#
# Customizing:
# 1. Feel free to change ./configure parameters for more features
# 2. To adapt other ARM variants
#       set $CPU and $OPTIMIZE_CFLAGS 
#       call build_one
######################################################

NDK=/d/android-ndk-r10e
PLATFORM=$NDK/platforms/android-19/arch-arm/
VERSION=4.8
PREBUILT=$NDK/toolchains/arm-linux-androideabi-$VERSION/prebuilt/windows


function build_one
{
./configure --target-os=android \
    --prefix=$PREFIX \
    --enable-cross-compile \
    --extra-libs="-lgcc" \
    --arch=arm \
    --cc=$PREBUILT/bin/arm-linux-androideabi-gcc \
    --cross-prefix=$PREBUILT/bin/arm-linux-androideabi- \
    --nm=$PREBUILT/bin/arm-linux-androideabi-nm \
    --sysroot=$PLATFORM \
    --extra-cflags=" -O3 -fpic -DANDROID -DHAVE_SYS_UIO_H=1 -Dipv6mr_interface=ipv6mr_ifindex -fasm -Wno-psabi -fno-short-enums -fno-strict-aliasing -finline-limit=300 $OPTIMIZE_CFLAGS " \
    --extra-ldflags="-Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib -lc -lm -ldl -llog" \
    --disable-shared \
    --enable-static \
    --disable-debug \
    --disable-ffplay \
    --disable-ffserver \
    --disable-doc \
    --enable-protocol=file \
    --enable-avformat \
    --enable-avcodec \
    --disable-network \
    --enable-zlib \
    --disable-everything \
    --enable-demuxer=mov \
    --enable-demuxer=h264 \
    --enable-decoder=rawvideo \
    --enable-decoder=mjpeg \
    --enable-decoder=h263 \
    --enable-decoder=mpeg4 \
    --enable-decoder=h264 \
    --enable-parser=h264 \
    --disable-avfilter \
    --disable-avdevice \
    $ADDITIONAL_CONFIGURE_FLAG

make clean
make
make install
$PREBUILT/bin/arm-linux-androideabi-ar d libavcodec/libavcodec.a inverse.o
$PREBUILT/bin/arm-linux-androideabi-ld -rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib  -soname libffmpeg.so -shared -nostdlib  -z noexecstack -Bsymbolic --whole-archive --no-undefined -o $PREFIX/libffmpeg.so libavcodec/libavcodec.a libavformat/libavformat.a libavutil/libavutil.a libswscale/libswscale.a -lc -lm -lz -ldl -llog  --warn-common  --dynamic-linker=/system/bin/linker $PREBUILT/lib/gcc/arm-linux-androideabi/$VERSION/libgcc.a
}

#arm v7n
CPU=armv7-a
OPTIMIZE_CFLAGS="-mfloat-abi=softfp -mfpu=neon -marm -march=$CPU -mtune=cortex-a8"
PREFIX=./android/$CPU-v7n 
ADDITIONAL_CONFIGURE_FLAG=--enable-neon
build_one

```
