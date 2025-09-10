Getting Started
===============

Installing
----------

**eip_scanner** provides only installing from the sources. To compile the sources, your system must meet the following
requirements:

* Linux or MacOS operation system
* CMake 3.5 or higher 
* C++ compiler supporting C++20 standard
* Gtest 1.8.0 or higher (optional)


In order to compile and install the library, type from the project's root directory:

::

   mkdir build
   cd build
   cmake ..
   cmake --build . --target install

Optionally, you can build the usage examples and the unit tests by adding the following CMake options:

::

   cmake -DTEST_ENABLED=ON -DEXAMPLE_ENABLED=ON ..


For successful usage of the library, it will be very helpful if you remember where **eip_scanner** have 
been installed.

Usage
-----

Here we will show how you can use the library in your CMake project. For that, let's make a simple project.

First of all, we should create *CMakeLists.txt* with the following content: 

.. code-block:: cmake
  :emphasize-lines: 8,9

   cmake_minimum_required(VERSION 3.5)
   project(hi_eip)

   set(CMAKE_CXX_STANDARD 14)

   add_executable(hi_eip main.cpp)

   include_directories(/usr/local/include/eip_scanner)
   target_link_libraries(hi_eip eip_scanner)


Pay attention to the last two lines. Currently, **eip_scanner** doesn't provide a cmake module to help to find
the library on your machine and we have to do all manually. First, we point on the include directory whose path
should be `path/were/eipscanner/is/installed/` + `eip_scanner`. Second, we link our executable file with the library 
`eip_scanner`. If you'd like to use the static library instead, use `eip_scannerS` name.

Okay, we have *CMakeLists.txt*. Now we should create *main.cpp* and place there this code:

.. code-block:: cpp
  
   #include <eip_scanner/MessageRouter.h>
   #include <eip_scanner/utils/Logger.h>
   #include <eip_scanner/utils/Buffer.h>

   using eipScanner::SessionInfo;
   using eipScanner::MessageRouter;
   using namespace eipScanner::cip;
   using namespace eipScanner::utils;

   int main() {
           Logger::setLogLevel(LogLevel::DEBUG);
           auto si = std::make_shared<SessionInfo>("172.28.1.3", 0xAF12);
           auto messageRouter = std::make_shared<MessageRouter>();

           // Read attribute
           auto response = messageRouter->sendRequest(si, ServiceCodes::GET_ATTRIBUTE_SINGLE,
                                                          EPath(0x01, 1, 1));

           if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
                   Buffer buffer(response.getData());
                   CipUint vendorId;
                   buffer >> vendorId;

                   Logger(LogLevel::INFO) << "Vendor ID is " << vendorId;
           }

           return 0;
   }


If you are familiar with **EtherNet/IP** protocol you should understand that the code is doing. If not, it doesn't matter,
we will discuss this later.

Let's build the example and run it:

::

  mkdir build && cd build
  cmake ..
  ./hi_eip


It might happen you become the error:

::

  libeip_scanner.so.1: cannot open shared object file: No such file or directory


It means, your host system didn't manage to find **eip_scanner**'s shared library. We can help it:

::

  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/where/eipscanner/is/installed


Try again and it must work. If you failed again, then `a bug report`_ will be welcomed. 


.. _a bug report: https://github.com/nimbuscontrols/eip_scanner/issues
