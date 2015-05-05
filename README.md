## C Dup and Dup2

[![PayPayl donate button](https://img.shields.io/badge/paypal-donate-yellow.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JCT98Z2B5WMM8 "Donate once-off to this project using Paypal")
![dup--dup2 version](https://img.shields.io/badge/dup--dup2-1.0.0-red.svg)

* It's a dup and dup2 example, and provide flowchart to easily know how to work.

## Overview

* [Getting Started](#getting-started)
* [Usage](#usage)
* [Flowchart](#flowchart)
* [Result](#result)
* [License](#license)

## Getting Started
git clone https://github.com/ChrisSheu/c-git.git<br>
cd c-git<br>
git checkout origin/dup_dup2 -b [yourbranch-name]<br>
cd [yourbranch-name]

## Usage
//following command will output file(dup_dup2) in output folder.<br>
`~# make all`<br><br>
`~# cd output`<br><br>
`~# ./dup_dup2 `<br><br>

* now you can see result!<br>

## Flowchart

* according source code to record the file descriptors changing status and draw it.

<div>
<img src="https://raw.githubusercontent.com/ChrisSheu/c-git/dup_dup2/docs/dup_dup2%20graph.PNG" align="left|bottom" height="300" width="400">
</div>

* to check [PID] in your processes.<br><br>
`~# ps -u `<br><br>
<div>
<img src="https://raw.githubusercontent.com/ChrisSheu/c-git/dup_dup2/docs/ps.PNG" align="left|bottom" height="150" width="600">
</div>

* to check file descriptors is in used in /proc/[PID]/fd.<br><br>
`~# ls -l /proc/[7772]/fd(in this example) `<br><br>

<div>
<img src="https://raw.githubusercontent.com/ChrisSheu/c-git/dup_dup2/docs/ls.PNG" align="left|bottom" height="100" width="600">
</div>

* there are five file descriptors in this process(7772) is in used.<br><br>

## Result

* Both stdout and file named somefile are be written by file descriptors(stdout, fd, save_fd) respectively. 

<div>
<img src="https://raw.githubusercontent.com/ChrisSheu/c-git/dup_dup2/docs/result.PNG" align="left|bottom" height="100" width="500">
</div>

## License
* [MIT](http://opensource.org/licenses/MIT)
