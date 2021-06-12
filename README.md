# Ponscripter-fork (07th-mod)

[![Build status](https://github.com/07th-mod/ponscripter-fork/workflows/Build/badge.svg)](../../actions)

## Introduction

Ponscripter was created as an NScripter-style visual novel game interpreter with an emphasis
on supporting games in Western languages.

This fork is intended to provide additional features to Ponscripter that make it more suitable for releasing
games on Steam. It, however, does not have qualms with breaking compatibility in minor ways. See the CHANGES file.

Other engines focussed on Western-language games include:

* [Ponscripter](http://web.archive.org/web/20180812034417/http://unclemion.com/onscripter/) &mdash; The project this is based on
* [Ren'Py](http://www.renpy.org/) &mdash; A modern visual novel engine with python scripting support.

### Documentation

* [Ponscripter Tutorial](https://07th-mod.github.io/ponscripter-fork) - Most likely you should start here
* [Ponscripter API Reference](https://07th-mod.github.io/ponscripter-fork/api/) - Mirror of Uncle Mion's Ponscripter API reference

## Compatibility

This incarnation of the engine has primarily been tested with the [Umineko](https://store.steampowered.com/app/406550/) script.

We strive to keep the engine able to compile and run correctly without the Steam runtime/api, but
the Steam platforms are our primary targets.

The following platforms are supported:

* Any modern Linux
* Windows 7/8/10
* OSX 10.9+

Any issues on these platforms should be reported.

## Bugs

Please report all bugs with this project on the [issues page](../../issues). Include your platform and, if possible, clear reproduction instructions.


## Acknowledgements

The original Ponscripter engine was maintained and, in great part, created by Mion.

Ponscripter is built primarily on Ogapee's work, without which there
would be nothing here.

The version of ONScripter taken as a base also includes notable
contributions from Chendo (original English support), insani
(further enhancements) and The Witch Hunt.

The following people (in alphabetical order) have contributed 
noteworthy patches and bug reports that have enhanced Ponscripter:

* Agilis
* chronotrig
* Daniel Oaks
* Euank
* Klashikari
* Mion of Sonozaki Futago-tachi
* Roine Gustafsson
* Roto
* Starchanchan

Apologies to anyone not mentioned &mdash; please let me know so I can set the
record straight!

## 07th-Mod fork

The 07th-Mod fork of this repo is brought to you with huge thanks to everyone mentioned above, but the latest changes are being maintained by the [07th-Mod](https://07th-mod.com) team. If you have any issues with this fork, please feel free to report them on the issues page or bother us on Discord about it.

This includes 2x mode, which can be used to support older Nscripter games at 2x resolution. This feature has been added as an extra argument to `;mode` (e.g. `;mode960@2x`). Widescreen support is also included, and can be enabled with `;modew720` or `;modew1080`.


## License

ONScripter is copyright © 2001-2007 Ogapee.  The Ponscripter fork is
copyright © 2006-2009 Haeleth.

Ponscripter is licensed under the GNU General Public License, version
2 or (at your option) any later version.  See COPYING for details.
