# scomp
A simple compression/decompression tool with a source code implementation following the Linux kernel coding style.

## Installation
You may either download ```scomp``` from the [Releases](https://github.com/ArijitKD/scomp/releases) section or build it yourself from the source using ```make```. If you are using a POSIX-compliant system, you may already have ```make``` pre-installed. Build the project by opening a terminal in the project directory and running ```make```. ```make clean``` removes the built binaries.

## Usage
* ```scomp``` uses a very simple algorithm to compress/decompress a given string that consists a repeating sequence of characters.
* Run ```./scomp``` or ```./scomp -help``` in the build directory to get an idea about how the program works.

## License
```scomp``` is licensed under the MIT License.

## Why make this?
Hitherto I have not been following any particular coding style for creating my projects. While I always aim for an optimized code as far as possible, many a times things go unnoticed and the code remains unpolished at certain regions. Additionally, I started to feel that my code formatting may not be up to the mark as I struggle to understand my code, if left unattended for a while. Being aware that [The Linux Kernel](https://github.com/torvalds/linux) is one of the best (if not the only best) projects written in C, I decided to derive inspiration from it and improve my C coding style. This project is an implementation of the same.
