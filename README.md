# Dependencies

Library uses opencv library for image processing. You can install it in debian-based systems by command:

```
# apt-get install libopencv-dev
```

# Building

You can build the library by following command:

```
$ make all
```

# Installing

You can install the library by copying builded files on appropriate locations in filesystem.

```
# cp dhash.h /usr/local/include
# cp dist/Release/GNU-Linux-x86/libdhash.so /usr/local/lib
# ldconfig /usr/local/lib
```
