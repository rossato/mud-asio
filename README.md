C++11 Mud Project using the Boost ASIO library
==============================================

Copyright 2018 Kenneth Case Rossato,
 offered under the GNU GPL 2 or any later version, see COPYING file.

This mud is not (yet) practical for public or any other meaningful use.  See limitations below.

About
-----
This is a work-in-progress mud for learning/demonstrating C++ techniques,
including using the C++11/14/17 (and beyond) standards.  Current code uses C++17
and requires the Boost ASIO library.  CMake is used for building, and Google Test
and Doxygen are optionally required for unit testing and class documentation,
respectively (you can comment them out of the CMakeList.txt if you don't have them).

Features
--------
 * Cross-platform.  Developed on Linux but should work anywhere CMake and Boost exist.
 * Text parsing capabilities are slightly advanced for a mud (but not as good as a text adventure).
 * Self-documenting "help" system for commands.
 * Multiple simultaneous logins (views) for a user.
 * HTTP REST endpoint for health-check, basic server status information (number of connections).
 * Code attempts to be performance-conscious without sacrificing good object-oriented design.
 * Type safety is heavily leveraged throughout, especially during text parsing.
   Static casts never occur for classes, and other casts never occur ever.
 * Pointer safety is strongly observed.  Unique_ptrs are used for ownership semantics, and
   references are prefered over bare-pointers for observers.

Limitations/roadmap
-------------------
 * Currently loads Item and Location data from a file but not Users.  Todo soon.
 * Does not yet save anything to a file.  Obviously a prerequesite for an actual useful mud.  Todo.
 * Commands are minimal, basically amount to movement and inventory management.
 * My plans don't include combat, but making some obvious place to extend code for combat.
 * In-world area development is a planned feature.
 * NPCs/bots are a desired feature.
