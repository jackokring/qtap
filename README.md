# Welcome to QtAp a Generic Qt App

A simple app which has simplicity of design as the build goal. Central to the app is an editor which although simple
has easy access to git repositories. As the app is extended with more views to provide greater functionality, the
library included will expand to cover some processing tasks. The library itself is written in a Qt agnostic way.
Although some functions may accept and return `(void *)` which have to be cast using the header macros into the Qt names.

This hopefully avoids having to have too much linker hastle. And makes for a more useable library. As of version 0.1.13
the app outline is sufficienctly advanced to build up a GUI, and then some business logic. All "official" versions and
not just commit edits will be tagged in the source repository. There are a few funnies in the source, such as naming
the library *libkqfn* so that `liblibkqfn.so` becomes the resolved name. You could say it's a bit libbier, ah, ah,
ha, ha ... Not labier as that would be far too experimental. ha, ha, ha.

Enough of the bad jokes for now, read the source and perhaps add a new class extension of `AViewWidget` and then
install it by adding it into the `MainWindow` constructor just under the other ones. This will be good for keeping
the minimal edits for building up the app. Everything else is proxied out to the view so that handling things like
control updates having a paste function are simply making a future call return of `canPaste` be true, and calling
`selectView` which will show any new availability of things, and in turn itself call `create` so that things are
put in order before exit to the event loop.

# A Framework for the Building of Qt Apps

The app for a framework to make simple plugin widgets. Not really that pluginable as you have to add a few lines and
compile again to alter the app template after you have removed the unplugged plugins you don't want. It has the basic
flow of input text, edit, repository cloud, and then use of any further view widgets which have been included. Anthing
which is not that related to the GUI has been hived off to `liblibkqfn.so` which has an unmangled C external interface,
but requires C++ to load the classes which do everything if you wish to develop it or use it in code developments.

The app can be released easily as a directory, as the library doesn't need to be installed in the system to be used.
The licence is BSD as anything really tasty will be hived into a separate library loaded indirectly. There maybe system
dependancies, but you can assume they will all be present on an equipped modern system. The application should be
renamed `qtap-??` where `??` represents something about what you've added (and maybe removed too).

# Some Useful Data Types and Routines

The application, apart from the `MainWindow` is divided into directories based on the goals of a particular file. These are:

* `AlgorithmsKG` used to store all the generic algorithms in the codebase.
* `DatatypesTG` used for the datatypes which may use the algorithms.
* `GadgetsKS` used for gadgets which can be considered to be implementation units of useful code using the datatypes
and algorithms.
* `JoineryTS` used for classes which implement the joins of the QtAp framework to the underlaying OS.

So it can be considered the the `G` represents generic or general coding, and `S` represents specific coding. The `K`
represents active or kinetic code hence algorithms, and `T` represents a static or much slower structure. It's just one way
of organization that I've been trying as time goes on basedon elemental thinking.

## [Source on GitHub](https://github.com/jackokring/qtap)

[Under Construction]
