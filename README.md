# Apollo

> A physically-based rendering system.

[![Generic badge](https://img.shields.io/badge/License-BSD3-blue)](LICENSE)
[![Generic badge](https://img.shields.io/badge/Language-C++17-red.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Build status](https://ci.appveyor.com/api/projects/status/u32mb711m11v749w?svg=true)](https://ci.appveyor.com/project/marovira/apollo-52qan)
[![Build Status](https://travis-ci.org/marovira/apollo.svg?branch=master)](https://travis-ci.org/marovira/apollo)
[![CodeFactor](https://www.codefactor.io/repository/github/marovira/apollo/badge)](https://www.codefactor.io/repository/github/marovira/apollo)


## What is Apollo?

Apollo is a physically-based rendering system inspired by PBRT and Kevin
Suffern's *Ray Tracing from the Ground Up*. The idea behind Apollo is to provide
a testing ground for learning and implementing new techniques in offline
rendering. There are two major end-goals for this project:

- [  ] Match [Mitsuba](https://github.com/mitsuba-renderer/mitsuba) in terms of
  quality and speed.
- [  ] Be able to render the *Moana Island Scene* from Walt Disney Animation
  Studios as found
  [here](https://www.disneyanimation.com/resources/moana-island-scene/).

For the sake of simplicity, Apollo will be purely CPU-based, as the focus right
now is to learn *how* to implement the rendering algorithms. In the future,
there will be variants of Apollo that will be implemented using specific
technologies (such as CUDA/OptiX, Embree, etc).

## Results

Render pending.

## Dependencies

The following are **core** dependencies of Apollo:

* CMake 3.19+
* A C++17 compiler.

Apollo has been tested on the following platforms and compilers:

| Platform (Compiler) | Version |
| ------------------- | ------- |
| Windows (MSVC) | 19.28 |
| Linux (GCC) | 9.3 |
| Linux (LLVM Clang) | 9.0 |

As with most of my projects, macOS is **not** supported.

In addition, Apollo depends on the following libraries:

* [Zeus](https://github.com/marovira/zeus)
* [oneAPI TBB](https://github.com/oneapi-src/oneTBB)

## License

Apollo is published under the BSD-3 license and can be viewed
[here](https://github.com/marovira/apollo/blob/master/LICENSE).
