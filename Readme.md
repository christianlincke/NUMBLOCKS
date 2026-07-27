
A template project with a Makefile that supports sub-directories.
Building three targets:
    - GB ROM ("make rom" / "make rom GBDK_DEBUG=ON")
    - CLI app ("make cli")
    - tests ("make test")
or run one of the vs code tasks

Project directories
  - src/core: Main program source files (.c, .h, .s) can go here
  - src/gb: anything gb specific goes here
  - src/cli: anything cli specific goes here
  - tests: tests 
  - res: Program graphics and audio source files (.c, .h, .s) can go here

The targets are build in build/$(targetname).



== Sprite and Background tiles from: ==
https://sondanielson.itch.io/gameboy-simple-rpg-tileset
"Licence is under CCA so you can use these assets both privately and commercially"
"You are free to use them as is or modify them to your liking. All i ask is you credit me if you do use them please :)"

