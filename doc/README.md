# Welcome to QtAp

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
