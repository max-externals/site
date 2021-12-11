---
title: Tools
permalink: /mmc/tools
parent: Meet Music Code
nav_order: 1
---

{:toc}

# Tools

Per partecipare al workshop sara' necessario avere preinstallato sul proprio computer alcuni programmi, di seguito le istruzioni per Windows e per macOS

## Windows

### Programmi
- [Build Tools for Visual Studio](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) necessario per avere tutti gli strumenti di sviluppo in ambiente windows, eseguire l'installer e selezionare le opzioni:
    - Desktop development with C++
        - MSVC
        - Windows 10 SDK
        - C++ CMake
        - Clang

![VS Build Tools](https://raw.githubusercontent.com/max-externals/site/main/media/VSBuildTools.PNG)

- [Visual Studio Code](https://code.visualstudio.com/) durante il workshop useremo Visual Studio Code che rende lo sviluppo semplificato


## OS X

### Programmi
- [XCode command line utils](https://raw.githubusercontent.com/max-externals/site/main/tools/install-clang.zip) necessario per avere tutti gli strumenti di sviluppo in ambiente mac, scarica lo zip aprilo e apri il file install-clang.command , in alternativa apri un terminale ed esegui `xcode-select --install`
- [Visual Studio Code](https://code.visualstudio.com/) durante il workshop useremo Visual Studio Code che rende lo sviluppo semplificato
- [CMake](https://cmake.org/download/) cmake permette di semplificare il processo di build
- Da terminale `sudo "/Applications/CMake.app/Contents/bin/cmake-gui" --install`


## Set-up Visual Studio Code

Durante il workshop inoltre useremo le estensioni C/C++ per Visual Studio Code [https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)

Per installarla aprire Visual Studio Code, aprire le estensioni, cercare `c/c++ extension pack` e selezionare il primo elemento

![Install C++ Extension](https://raw.githubusercontent.com/max-externals/site/main/media/vs_code_cpp_extension.png)
