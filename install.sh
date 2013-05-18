#!/bin/sh
BINPATH=/usr/bin
DBUS_SYSTEMD=/etc/dbus-1/system.d
DBUS_SERVICE=/usr/share/dbus-1/system-services
SYSTEM_PIXMAP=/usr/share/pixmaps
APP_PATH=/usr/share/applications
INIT_PATH=/etc/init.d

cp gui/icon.png $SYSTEM_PIXMAP/qH3Client.png
cp gui/qH3Client.desktop $APP_PATH

cp auth/com.qh3client.EAPDaemon.conf $DBUS_SYSTEMD
cp auth/com.qh3client.EAPDaemon.service $DBUS_SERVICE
cp auth/qh3client-daemon-script $INIT_PATH/qh3client-daemon
chmod +x $INIT_PATH/qh3client-daemon

cp auth/qh3client-daemon $BINPATH
cp gui/qh3client $BINPATH
chmod +x $BINPATH/qh3client $BINPATH/qh3client-daemon
