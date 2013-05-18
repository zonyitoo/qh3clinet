#!/bin/sh

BIN_PATH=usr/bin
DBUS_SYSTEMD=etc/dbus-1/system.d
DBUS_SERVICE=usr/share/dbus-1/system-services
SYSTEM_PIXMAP=usr/share/pixmaps
APP_PATH=usr/share/applications
INIT_PATH=etc/init.d

rm -rf pkg
mkdir pkg
mkdir -p pkg/$DBUS_SYSTEMD
mkdir -p pkg/$DBUS_SERVICE
mkdir -p pkg/$SYSTEM_PIXMAP
mkdir -p pkg/$APP_PATH
mkdir -p pkg/$INIT_PATH
mkdir -p pkg/$BIN_PATH

cp auth/qh3client-daemon pkg/$BIN_PATH
cp auth/qh3client-daemon-script pkg/$INIT_PATH/qh3client-daemon
cp auth/com.qh3client.EAPDaemon.conf pkg/$DBUS_SYSTEMD
cp auth/com.qh3client.EAPDaemon.service pkg/$DBUS_SERVICE
cp gui/qh3client pkg/$BIN_PATH
cp gui/qH3Client.desktop pkg/$SYSTEM_PIXMAP
cp gui/qH3Client.desktop pkg/$APP_PATH

cp -r debian pkg/DEBIAN

dpkg -b ./pkg $1
