# eip_scanner

![GitHub release (latest by date)](https://img.shields.io/github/v/release/wefabricate/eip_scanner)

Free implementation of Ethernet/IP scanner in C++.

Fork of https://github.com/nimbuscontrols/EIPScanner. Changes include:
* Naming EIPScanner -> eip_scanner
* namespace eipScanner -> eip_scanner
* headers installed into `eip_scanner` folder (https://github.com/nimbuscontrols/EIPScanner/pull/115)
* building libraries static/shared toggle now done in standard Cmake way
* cmake discoverable (https://github.com/nimbuscontrols/EIPScanner/pull/88)
* Fix timing particularly around receiving messages. NOTE: It is not recommended to use a long receive timeout as there is a good chance it will use it, particularly in multi connection scenarios.
* Add ltt-ng tracepoints
* Default disable vendor source build
* Other fixes:
  * https://github.com/nimbuscontrols/EIPScanner/pull/112
  * https://github.com/ambi-robotics/EIPScanner/pull/4

## Features

* Explicit messaging
* Implicit messaging (only point-to-point)
* Discovery

CIP Standard objects:

* File Object (upload only)
* Parameter Object (read only)
* Identity Object

Vendor specific objects:

* RA DPI Fault Object 

## Requirements

* CMake 3.5 and higher
* C++20 compiler (tested with GCC and MinGW)
* Linux, MacOS, and Windows

## Installing

```shell
$ mkdir build && cd build
$ cmake ..
$ cmake --build . --target install
```

## Usage

To see how to work with the library, look into `examples` directory 

## Contribution

We welcome outside contributions for any improvements, features, or new devices (see below) to the library. Please create a pull request with unit/mock tests

## Vendor specific objects & devices

Our goal is to maintain an active device catalog of all EtherNet/IP devices and their objects that we encounter. We encourage others to contribute to this communal device catalog in /src/vendor/.

All vendor source files are automatically enabled. If you'd like to disable vendor files, you can disable with any of the below options.
1) set this option in CMakeLists.txt
```cmake
option(ENABLE_VENDOR_SRC "Enable vendor source" OFF)
```
2) set the cmake build flag 
```shell
-DENABLE_VENDOR_SRC=OFF
```
3) manually comment needed files in src/vendor/CMakeLists.txt

## Authors

Aleksey Timin

Adam Roth
