# Neon Drivers Quick-start guide

Neon is a collection of software components for real-time applications.

# Using Drivers

## Configuration

Configuration is done in `neon_app_config.h` header file. The file is included
by `base/include/shared/config.h` file, which is in included in all other Neon
components.

## Building

### Include paths

- `drivers/include` - standard Neon include path
- `port/_port-name_` - where the _port-name_ is the directory of port that is 
used.

### Source files

- `source/rtc/*.c` - RTC source files.
- `port/_port-name_/*.c` - port source files. Ports may have additional
    source files located in `port/_port-name_` directory which need to be 
    compiled.

### Project dependencies

Neon Eds does depend on the following components:
- base
- kernel
- eds

