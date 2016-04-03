# Marathon24 template

Code template for Marathon24-type competitions. Not very pretty or robust, use at your own risk.
Uses older version of Tcp commucation by @simsa-st, check out a more complex library
[contest24](https://github.com/simsa-st/contest24) by him.

The code template provides Tcp communication and a visualizer window.

## Installation

Install dependencies:
- Gflags:
 - Mac OS X: `brew install gflags`
- Glog:
 - Mac OS X: `brew install glog`
- SFML2.x:
 - Mac OS X: `brew install sfml`

Compile contest24:

```
cd c24/build
./build.sh
```

In the root directory, examine the build script (adjust to your platform) and compile:
```
cat build_solution.sh
./build_solution.sh
```

## Usage
Simply run `./solution` in the root directory.

When run with parameters `./solution --log_dir=logs --stderrthreshold=0`, you should see
error messages about failed Tcp connection in the console and a window with very simple graphics.
