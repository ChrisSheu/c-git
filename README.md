## C_Shared_Memory

[![PayPayl donate button](https://img.shields.io/badge/paypal-donate-yellow.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JCT98Z2B5WMM8 "Donate once-off to this project using Paypal")
![share--memory version](https://img.shields.io/badge/share--memory-1.0.0-red.svg)

* It's a simple IPC - shared memory example.

## Overview

* [Getting Started](#getting-started)
* [Usage](#usage)
* [Result](#result)
* [License](#license)

## Getting Started
git clone https://github.com/ChrisSheu/c-git.git<br>
cd c-git<br>
git checkout origin/share_memory -b [yourbranch-name]

## Usage
`~# make all`<br>
//above command will output two file(shm_read, shm_write) in install folder.

`~# cd output`


`~# ./shm_read -t 2`<br>
//show shared memory information every 2 seconds.( ./shm_read -h)

`~# ./shm_write`<br>
//you can change specified content of array.

## Result

* shm_read result:

<div>
<img src="https://github.com/ChrisSheu/c-git/blob/share_memory/docs/read.png?raw=true" align="left|bottom" height="300" width="400">
</div>

* shm_write result:

<div>
<img src="https://github.com/ChrisSheu/c-git/blob/share_memory/docs/write.png?raw=true" align="left|bottom" height="300" width="400">
</div>

## License
* [MIT](http://opensource.org/licenses/MIT)

