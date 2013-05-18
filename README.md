# QH3Client
Yet another H3C Authentication CLient

## Dependences

* qt5
* UNIX socket
* C++0x support

## Installation

First you need to install all dependences above. If you want to install it from source, you also need to install Qt5 dev tools (`qmake`, libs and other stuffs).

* Install from source

```bash
git clone git@github.com:zonyitoo/qh3clinet.git
qmake
make -j4
sudo ./install.sh
```

* Install from deb

If you are using Ubuntu 13.04 amd64 or later

```bash
dpkg -i qh3client_[version]_amd64.deb
```

## Thanks

* [YaH3C](git@github.com:humiaozuzu/YaH3C.git)
