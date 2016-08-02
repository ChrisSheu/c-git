## SerialPort_C_code

[![PayPayl donate button](https://img.shields.io/badge/paypal-donate-yellow.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JCT98Z2B5WMM8 "Donate once-off to this project using Paypal")

* It's a interesting connect serial port tool which is built on C , and support cross-compile.
* It support transforming hex value into ascii in string, like '0x6ch0x73h' is equal to 'ls', 
* and you can mix hex-value and string, like 'ifconfi0x67h' is equal to 'ifconfig'.
* The hex-value format: 0x[hex]h,  0x: is start, [hex]: your hex value, h: ending notation.

## Overview

* [Getting Started](#getting-started)
* [Compile](#compile)
* [Usage](#usage)
* [License](#license)

## Getting Started
git clone https://github.com/ChrisSheu/c-git.git<br>
cd c-git<br>
git checkout origin/SerialPort_C_code -b [local-branch-name]<br>
cd [local-branch-name]

## Compile
`normal compile : make clean all`<br>
`cross compile :  make CROSS_COMPILE=arm-xxx-linux- clean all`<br><br>

## Usage
`execute ./serialconn to check help`<br><br>

* now you can this tool!<br>

## License
* MIT LINCENSE.
