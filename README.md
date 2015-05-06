## C fcntl

[![PayPayl donate button](https://img.shields.io/badge/paypal-donate-yellow.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JCT98Z2B5WMM8 "Donate once-off to this project using Paypal")
![fcntl version](https://img.shields.io/badge/fcntl-1.0.0-red.svg)

* It's a fcntl example which display how to set file descriptor non-blocking and blocking.

## Overview

* [Getting Started](#getting-started)
* [Usage](#usage)
* [Flowchart](#flowchart)
* [Result](#result)
* [License](#license)

## Getting Started
git clone https://github.com/ChrisSheu/c-git.git<br>
cd c-git<br>
git checkout origin/fcntl -b [yourbranch-name]<br>
cd [yourbranch-name]

## Usage
//following command will output file(myfcntl) in output folder.<br>
`~# make all`<br><br>
`~# cd output`<br><br>
`~# ./myfcntl `<br><br>

* now you can see result!<br>

## Flowchart

* according source code to record the file descriptors how to work and draw it.

<div>
<img src="https://raw.githubusercontent.com/ChrisSheu/c-git/fcntl/docs/FlowChart.PNG" align="left|bottom" height="250" width="500">
</div>

## Result

* if set non-blocking, the read function is still running , but the return value is equal to -1.

<div>
<img src="https://raw.githubusercontent.com/ChrisSheu/c-git/fcntl/docs/setnonblocking.PNG" align="left|bottom" height="200" width="500">
</div>

* if set blocking, the read function is locked until pipe be input.

<div>
<img src="https://raw.githubusercontent.com/ChrisSheu/c-git/fcntl/docs/setblocking.PNG" align="left|bottom" height="200" width="500">
</div>

## License
* [MIT](http://opensource.org/licenses/MIT)
