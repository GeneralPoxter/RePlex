# RePlex

A live C++ library reloader, following the [tutorial by Jeremy Ong](https://howistart.org/posts/cpp/1/index.html).

## Usage

The library is entirely implemented in [RePlex.h](lib/pub/RePlex.h).
A sample usage is given in [Test.h](test/pub/Test.h).

To test the library, install [premake5](https://premake.github.io/) and run in the project root:
```
premake5 gmake
make
bin/Debug/RePlexRuntime
```