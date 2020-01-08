# Welcome to QtAp a Generic Qt App

A simple app which has simplicity of design as the build goal. Central to the app is an editor which although simple has easy access to git repositories. As the app is extended with more views to provide greater functionality, the library included will expand to cover some processing tasks. The library itself is written in a Qt agnostic way. Although some functions may accept and return `(void *)` which have to be cast using the header macros into the Qt names.

This hopefully avoids having to have too much linker hastle. And makes for a more useable library. As of version 0.1.13 the app outline is sufficienctly advanced to build up a GUI, and then some business logic. All "official" versions and not just commit edits will be tagged in the source repository. There are a few funnies in the source, such as naming the library *libkqfn* so that *liblibkqfn.so* becomes the resolved name. You could say it's a bit libbier, ah, ah , ha, ha ... Not labier as that would be far too experimental. ha, ha, ha.

Enough of the bad jokes for now, read the source and perhaps add a new class extension of *StatsView* and then install it by adding it into the *MainWindow* constructor just under the other ones. This will be good for keeping the minimal edits for building up the app. Everything else is proxied out to the view so that handling things like control updates having a paste function are simply making a future call return of *canPaste* be true, and calling *selectView* which will show any new availability of things, and in turn call *create* so that things are put in order before exit to the event loop.

# A Framework for the Building of Qt Apps

The app for a framework to make simple plugin widgets. Not really that plugin as you have to add a few lines and compile again to alter the app template after you have removed the unplugged plugins you don't want. It has the basic flow of input text, edit, repository cloud, and then use of any further view widgets which have been included. Anthing which is not that related to the GUI has been hived off to *liblibkqfn.so* which has an unmangled C external interface, but requires C++ to load the classes which do everything if you wish to develop it or use it in code developments.

The app can be released easily as a directory, as the library doesn't need to be installed in the system to be used. The licence is BSD as anything really tasty will be hived into a separate library loaded indirectly. There maybe system dependancies, but you can assume they will all be present on an equipped modern system. The application should be renamed `qtap-??` where `??` represents something about what you've added (and maybe removed too).

[Source on GitHub](https://github.com/jackokring/qtap)

[Under Construction]

## Welcome to GitHub Pages

You can use the [editor on GitHub](https://github.com/jackokring/qtap/edit/master/README.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/jackokring/qtap/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
