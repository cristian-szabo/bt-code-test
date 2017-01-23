# BTCodeTest

Develop a CSV parser to read router information (domain, ip address, patched, os version and notes) and filter data acording to some conditions and output the final list.

## Build Status

| Ubuntu 12.04 |
| --- |
| [![Build Status](https://travis-ci.org/cristian-szabo/bt-code-test.svg?branch=master)](https://travis-ci.org/cristian-szabo/bt-code-test) |

## Requirements

- Ubuntu 12.04
- GCC 4.6
- CMake 2.8.12
- Git > 1.7

## How To Install

Because **Ubuntu 12.04** comes with CMake 2.8.7 which is a lower version than the one required by the testing library. An updated version of CMake can be downloaded from the repository below.

```
sudo add-apt-repository ppa:smspillaz/cmake-2.8.12
sudo apt-get update
sudo apt-get install -y cmake-data cmake
```

Install remaining software dependencies.

```
sudo apt-get install -y git g++
```

Clone the project repository in a preffered location and navigate to the directory.

```
cd /home/[USER]/Desktop
git clone https://github.com/cristian-szabo/bt-code-test.git
cd bt-code-test
```

Configure the project files using the **Unix Makefiles** generator.

```
cmake -G "Unix Makefiles" -B./build -H./
```

Build the project using the make command.

```
cd build
make all
```

Run the tests and the application.

```
./app/CodeTestApp ../res/sample.csv
./test/CodeTestCheck --reporter=spec
```
