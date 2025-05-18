# nimped

## Introduction

nimped, or (n)ew and (imp)roved text (ed)itor, is the project for which I
decided to terminate work on my other program, medioed. It is a
from-the-ground-up implementation of a text editor, drawing inspiration and some
limited amount of code from medioed, but actually works very differently. This
should be a more minimal and sensical editor than the previous one.

## Dependencies

* A Linux system
* A compiler and libc supporting `-D_GNU_SOURCE`

## Management

* To build the program, run `./build.sh`
* To install the built binary to your system, run `./install-bin.sh` as root
* To choose a theme to install, set `THEME` in `install-conf.sh` to a filename
  in the `themes/` directory
* To install the configuration files, run `./install-conf.sh`
* To uninstall the binary from your system, run `./uninstall-bin.sh` as root
* To uninstall the configuration files, run `./uninstall-conf.sh`

## Usage

After installing all necessary files, run:

```
$ nimped file.txt file2.txt file3.txt # ...
```

Or, for help information:

```
$ nimped -h
```

You can use a non-default conf directory with the `-o` CLI option.

## Contributing

You can contribute to the project by creating a pull request. Fixes and features
that I consider useful might be accepted.
