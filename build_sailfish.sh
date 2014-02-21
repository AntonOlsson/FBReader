#! /bin/bash

CURDIR=`pwd`
VERSION=`cat fbreader/VERSION`
CPUARCH=$DEB_BUILD_ARCH_CPU

DESTDIR="${CURDIR}/fbreader-${VERSION}-sailfish-${CPUARCH}"

make DESTDIR=$DESTDIR TARGET_ARCH=sailfish UI_TYPE=qml TARGET_STATUS=release install