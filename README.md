# Welcome to QtAp a Generic Qt App

A simple app which has simplicity of design as the build goal. Central to the app is an editor which although simple has easy access to git repositories. As the app is extended with more views to provide greater functionality, the library included will expand to cover some processing tasks. The library itself is written in a Qt agnostic way. Although some functions may accept and return `(void *)` which have to be cast using the header macros into the Qt names.

This hopefully avoids having to have too much linker hastle. And makes for a more useable library. As of version 0.1.13 the app outline is sufficienctly advanced to build up a GUI, and then some business logic. All "official" versions and not just commit edits will be tagged in the source repository. There are a few funnies in the source, such as naming the library *libkqfn* so that *liblibkqfn.so* becomes the resolved name. You could say it's a bit libbier, ah, ah , ha, ha ...
