
.. contents:: Table of contents
   :backlinks: top
   :local:

Neon Drivers
============

Neon is a collection of software components for real-time applications.

Using Drivers
=============

Configuration
-------------

Configuration is done in ``neon_drivers_app_config.h`` header file. The file is
included by ``mcu/config.h`` file, which is in included in other Neon Drivers
components.

Building
--------

**Include paths**

- ``drivers/include`` - standard Neon include path
- ``port/_port-name_`` - where the _port-name_ is the directory of port that is
  used.

**Source files**

- ``source/rtc/*.c`` - RTC source files.
- ``port/_port-name_/*.c`` - port source files. Ports may have additional
  source files located in `port/_port-name_` directory which need to be
  compiled.

**Project dependencies**

Neon Eds does depend on the following components:

- eds

Support
=======

If you've found an error, please `file an issue
<https://github.com/nradulovic/neon-drivers/issues/new/>`_.

Patches are encouraged, and may be submitted by `forking this project 
<https://github.com/nradulovic/neon-drivers/fork/>`_ and submitting
a pull request through GitHub. Please see `CONTRIBUTING.rst
<https://github.com/nradulovic/neon-drivers/blob/master/CONTRIBUTING.rst/>`_
for more details.
