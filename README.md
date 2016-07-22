## ext filesystem sorce code

[![PayPayl donate button](https://img.shields.io/badge/paypal-donate-yellow.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=JCT98Z2B5WMM8 "Donate once-off to this project using Paypal")

* It's a ext filesystem source and you can cross-compile it via install_ext_fs.sh

## Overview

* [Getting Started](#getting-started)
* [Usage](#usage)
* [License](#license)

## Getting Started
git clone https://github.com/ChrisSheu/c-git.git<br>
cd c-git<br>
git checkout origin/ext_filesystem_sorce_code -b [local-branch-name]<br>
cd [local-branch-name]

## Usage
`tar xvf e2fsprogs-1.41.11.tar.gz`<br>
`Usage ./install_ext_fs.sh [default or crosscompile] <folder>`<br>
`ex(none): ./install_ext_fs.sh none e2fsprogs-1.41.11`<br>
`ex(arm): ./install_ext_fs.sh arm-xxx-linux e2fsprogs-1.41.11`<br><br>

* now you can enjoy this tool!<br>

## License
* See Lincese in the folder.
