# PNP

The code included within this branch of the current repository is intended 
to provide a basic overview of my ideas concerning "channel" based VMs and 
the Chasm (channel/syntagm) VM in particular.  Other branches here will 
include more substantial implementations of a compiler and runtime stack.

For a summary of channels and channel packages, please look at the paper 
on compiler theory linked at the top of the main branch readme.  I can also 
share PDFs of chapters in books addressing these topics on request.

The basic theme of the code presented here is building channel packages 
and then evaluating them.  The first of these steps can proceed 
in several ways.  First, the channels can be built step-by-step via 
C++ code (this is illustrated with the `chasm-lib-console` project).  Second, 
C++ code can call VM instructions in the form of C++ methods (illustrated 
with the `crb-console` project).  Finally, the VM instructions can be 
built up by parsing an input script (illustrated by the `chasm-tr-console` 
and `chasm-vm-console` projects).

The "projects" referred to in this case are Qt creator projects, and each 
corresponds to a Qt project file (`.pro` extension).  More recent 
Qt versions have switched to a `cmake` build system in lieu of the 
internal `qmake`.  However, for purposes of demonstration I believe 
the `qmake` framework is easier to use, so I am basing this demo 
code on an older version of Qt (specifically, 5.15).  Users seeking 
to compile the code against Qt 6.x should have few problems, though I 
cannot vouch that every single line will compile properly.  Although 
Qt6 is not fully backward-compatible with Qt5, the few differences 
appear to be in the context of GUI classes, which is mostly not 
relevant for this demo code (not relevant at all, currently).  I have 
also included the instruction `QT_DISABLE_DEPRECATED_BEFORE=0x060000` 
to detect whether I use any construction that is not present in Qt6.

The simplest way to examine this code is probably to use the Qt Creator 
IDE.  One then has the option of building each object in groups 
or individually.  The repo's file structure has many layers, with 
the anticipation that similar code could form the basis of 
Executable Research Objects, which may progress through several 
versions and multiplie contributors.  I have chosen a file layout 
that could keep multiple work-in-progress organized without 
(needing to) split the code into multiple git branches.
With that said, assuming this brach is cloned into ROOT, 
then project files fit a pattern like `ROOT/code/cpp/projects/qt/qtm/both/prelim/contrib/all/all/sk` 
followed by a two-part project name.  Calling the above folder "`SK`" (or "scripting kernel"), 
the projects can be loaded and compiled in this order:

 - chasm-lib-33
 - chasm-lib-43
 - chasm-lib-X1
 - chasm-lib-X2
 - chasm-lib
 - chasm-lib-console
 - chasm-tr
 - chasm-tr-parser
 - chasm-tr-console
 - chasm-procedure-table
 - chasm-runtime-bridge
 - chasm-vm
 - crb-console
 - chasm-vm-console

Here, the projects whose name ends with "-console" are compiled 
as command-line programs that can be run from Qt Creator.  The 
others are libaries.  See the "`main.cpp`" files for each console 
program for a description of its purpose.

An additional step is needed to compile and run some of these projects.  This 
is because, first, some of the VM compiler code is auto-generated.  Rather than 
just including this specific code as part of the repository, I have 
included it as a zipped file (specifically ROOT/dev.zip).  This gives 
me an option of swapping some different batch of auto-generated code 
if I want to test some variation.  The code presented here expects this 
generated code to be in a folder called `dev` that is sibling to ROOT, so 
just extract the zip file to its parent directory.  The `dev` folder 
will also have a sample script to run for the VM demo.



 

