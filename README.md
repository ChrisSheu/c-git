## Busy Box (include cross-compile)

[![PayPayl donate button](https://img.shields.io/badge/paypal-donate-yellow.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JCT98Z2B5WMM8 "Donate once-off to this project using Paypal")

* It's a busybox source code and include cross-compile platform or default(your local compile).
* If you get a error like [loginutils/passwd.c:188:12: error: ‘RLIMIT_FSIZE’ undeclared], 
* you could resolve it by stackoverflow: http://stackoverflow.com/questions/13809545/error-in-busybox-cross-compilation-to-arm

## Overview

* [Getting Started](#getting-started)
* [Usage](#usage)
* [File](#file)
* [License](#license)

## Getting Started
git clone https://github.com/ChrisSheu/c-git.git<br>
cd c-git<br>
git checkout origin/20160725_busy_box -b [local-branch-name]<br>
cd [local-branch-name]<br>

## Usage
`Usage ./compile.sh <folder> [crosscompile]`<br>
`cross-platform example: ./compile.sh folder arm-xxx-linux-`<br>
`default example: ./compile.sh folder`<br>

* now you can this tool!<br><br>

## File
`busybox-1.20.1.tar.bz2 : original file from busybox official`<br>
`busybox-1.20.1.tgz     : fixed error file about http://stackoverflow.com/questions/13809545/error-in-busybox-cross-compilation-to-arm`<br>
`compile.sh             : compile script. refer to Usage step.`<br>

* now you can this tool!<br><br>

## License
* See the LICENSE in the folder.
