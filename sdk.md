---
title: Max/MSP Externals
permalink: /
nav_order: 2
---

## The sdk 

[Max/MSP](https://cycling74.com/) has an extensive sdk (Software Development Kit) for developing new objects for the Max/MSP visual programming environment. 
The latest version of the sdk is the 8.0.3. The Max/MSP sdk contains the API (headers, source, libraries) for building external objects in C/C++. 
It additionally includes documentation of the API and example projects using the Xcode and Visual Studio development environments.

## Download and place the sdk 

You can download the sdk here: [download sdk 8.0.3](https://cycling74.com/downloads/sdk). 
The SDK is structured as a Max package. Simply place the SDK in your `Max/Packages` folder to begin working.

## Online API documentation

You can find the latest documentation of the sdk here: [API documentation](https://cycling74.com/sdk/max-sdk-8.0.3/html/index.html).
The SDK documentation is accompanied by a series of projects for compiling some example Max external objects. 
When you build the example projects, the resulting Max external will be located in a folder called "externals" at the top level of the SDK package.

## testing

It is highly recommended that you test your code thoroughly. One option is use the [max-test](https://github.com/Cycling74/max-test) package.

## support
 
For support, please use the developer forums at: [max forum](http://cycling74.com/forums/)






