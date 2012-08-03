LayoutStudio Readme
===================

LayoutStudio is a work-in-progress application written by Treeki which plans to
offer graphical editing of Wii NW4R layouts/banners (.brlyt/.brlan files) with
functional, real-time previews.

The application is written in C++ (with heavy usage of Nokia's Qt toolkit).

I originally started this project in 2010, and wrote all the code required to
read and write layout files, but then lost interest because I didn't know where
to start with the rendering code.

In July 2012, I've taken it back up and changed my focus. Instead of trying to
render layouts exactly the same way as they appear on the Wii, I'm going to
forget about all the fancy TEV stuff and just create something that's usable for
designing and editing layouts. I've been informed that this is actually how
Nintendo's official tool works.

Of course, I'd still love to add accurate rendering to this some day, but it's
not going to be my #1 goal any more. Having a working editor is more important.


### Implemented Features ###
- BRLYT reading, writing (writing is not fully tested yet)
- Simple API for manipulating layout files from other code
- Wii filesystem API (currently only supports U8 archives)


### Planned Features ###
- BRLAN reading/writing
- Graphical interface for editing layouts and animations
- Rendering of layouts using OpenGL
- BRFNT support


### Current (Short-Term) TODO ###
- Disassemble code that sets up indirect textures for materials
- Add usd1 handling
- Add TPL file support


Other Stuff
-----------

Special thanks to megazig, trap15 and everyone else who worked on benzin.
Greets to #HACKERCHANNEL and the Newer team :p

Icons used in the GUI are from the Fugue set by [Yusuke Kamiyamane][yk].

The project is licensed under the GNU General Public License v2.

[yk]: http://p.yusukekamiyamane.com/
