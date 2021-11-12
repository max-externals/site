---
title: Max/MSP Introduction
permalink: /max-msp-intro
nav_order: 3
---

# An introduction to Max/MSP

## The Max/MSP ecosystem
Max/MSP is a **visual programming language** and environment for music and multimedia, owned by the company [Cycling '74](https://cycling74.com/). Max/MSP is a commercial product, and not an open source project. 

Max/MSP has a thirty-year history, began in 1988 at IRCAM in Paris thanks to Miller Puckette. Some notes on its history can be found on the dedicated [wikipedia page](https://en.wikipedia.org/wiki/Max_(software)).

The Max/MSP ecosystem includes a **visual programming environment** (the Patcher Window) and a large number of **routines** (called objects). An object can be seen as a function that processes input data to generate output data. These objects are created (as visual objects) in the **Patcher Window** and connected together with **Patch cords** for creating algorithms. A Max/MSP program is called a **patch**.

Many objects are available natively in the software itself, but the Max/MSP ecosystem can be expanded with thirdy-party libraries (collections of objects). Cycling '74 made available an **SDK (Software Development Kit)** for the community of developers with an extensive API (Application Programming Interface) for building **external objects in C/C++**. 

There is a large user base of programmers, unaffiliated with Cycling '74, who enhance the Max/MSP environment with commercial and non-commercial extensions and libraries. 

The main sections that compose the **native Max/MSP ecosystem** can be listed as follow :   

 **Max** section: this covers all the concepts connected to the processing of **control data streams** (not audio signals). This section of the Max/MSP environment allows you to manage all data that are not audio signals or image matrices. The signals handled by this section are called **control signals**. The objects (functions) that act on these signals are connected to each other with **gray patch chords**. 

> The control signals in Max/MSP can be of the following **data types**:

> - **Integer Numbers**: positive and negative integer numbers. Integer type is indicated by the letter i.
> - **Float Number**: real numbers, with 32bit precision. Float is indicated by the letter f.
> - **Symbols**: alphanumeric string. Symbol is indicated with the letter s (or sym in some cases). 
> - **Lists**: a list of symbols or numbers. List is indicated by the letter l. 
  
> **MSP** section: this covers all the concepts connected to the processing of **audio data streams** (audio signals).  
 
> **Jitter** section: this covers all the concepts connected to the **image/video/3D processing**.  

> **Gen**: this is an extension of the Max patching environment that converts what you build visually into **efficient compiled code**.

> **Node** section: this is an extension that allows the integration of the **nodeJS environment** into the Max/MSP environment.



## The Max/MSP visual programming environment
- concept of patch
- patcher window
- connecting objects


## The anatomy of a Max/MSP Object
- concept of inlets and outlets
- hot and cold inlets

### Max Object

### MSP Object "~"
- riceive two types of 



## Connections (patch cords)

### Audio Signal

### Control signal 

### Data Type of control signal



## Using libraries
- citare alcune librerie famose




