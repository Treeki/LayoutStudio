LayoutStudio Readme
===================

LayoutStudio is a work-in-progress application written by Treeki which plans to
offer graphical editing of Wii NW4R layouts/banners (.brlyt/.brlan files) with
functional, real-time previews.

The application is written in C++ (with heavy usage of Nokia's Qt toolkit).

The graphical rendering will eventually use OpenGL, and build on the excellent
GX GPU emulation in the [Dolphin][dol] GameCube/Wii emulator.

[dol]: http://code.google.com/p/dolphin-emu/


### Implemented Features ###
- BRLYT reading, writing (writing is not fully tested yet)
- Simple API for manipulating layout files from other code
- Wii filesystem API (currently only supports U8 archives)


### Planned Features ###
- BRLAN reading/writing
- Graphical interface for editing layouts and animations
- Rendering of layouts using OpenGL and Dolphin's GX emulation
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
