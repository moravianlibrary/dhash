# Dependencies
The library is dependent on ImageMagick library. In Debian based systems you can install it by following command.
```
# apt-get install libmagickwand-dev
```
# Build
You need to have installed *automake* tools. If you have it, you just run following commands.
```bash
$ autoreconf -i
$ ./configure
$ make
```
# Install
```
# make install
# ldconfig /usr/local/lib
```
