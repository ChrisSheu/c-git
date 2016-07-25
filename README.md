## watch_File Descriptor

[![PayPayl donate button](https://img.shields.io/badge/paypal-donate-yellow.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JCT98Z2B5WMM8 "Donate once-off to this project using Paypal")

* It's C tool that checking file descriptor of specified process.

## Overview

* [Getting Started](#getting-started)
* [Usage](#usage)
* [How to Compile](#how-to-compile)
* [License](#license)

## Getting Started
git clone https://github.com/ChrisSheu/c-git.git<br>
cd c-git<br>
git checkout origin/watch_FileDescriptor -b [local-branch-name]<br>
cd [local-branch-name]

## How to Compile
make clean all<br>
　　　or<br>
make CROSS_COMPILE=arm-xxx-linux- clean all<br>

## Usage
`./watch_file_descriptor.c <pid> <timer>`<br>
`<pid>  : process id.`<br>
`<timer>: specifies delay time for updating`<br><br>

* now you can this tool!<br>

## License
* MIT LINCENSE.
