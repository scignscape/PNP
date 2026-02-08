## ChasmVM Demo Code

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
then project files fit a pattern like `ROOT/code/cpp/projects/qt/qtm/isobuild/prelim/contrib/all/all/sk` 
followed by a two-part project name.  Calling the above folder "`SK`" (for "scripting kernel"), 
the projects can be loaded and compiled in this order:

 - chasm/chasm-lib/chasm-lib-33
 - chasm/chasm-lib/chasm-lib-43
 - chasm/chasm-lib/chasm-lib-X1
 - chasm/chasm-lib/chasm-lib-X2
 - chasm/chasm-lib/chasm-lib
 - chasm/chasm-lib/chasm-lib-console
 - chasm/chasm-tr/chasm-tr
 - chasm/chasm-tr/chasm-tr-parser
 - chasm/chasm-tr/chasm-tr-console
 - chasm/chasm-vm/chasm-procedure-table
 - chasm/chasm-vm/chasm-runtime-bridge
 - chasm/chasm-vm/chasm-vm
 - chasm/chasm-vm/crb-console
 - chasm/chasm-vm/chasm-vm-console
 - chasm/otqr/otqr-sdi-parser
 - chasm/otqr/otqr-console


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

As a shortcut instead of building each project individually, there are 
three project that use Qt's "`subdirs`" system to group the projects 
together.  With `SK` being `ROOT/code/cpp/projects/qt/qtm/unibuild/prelim/contrib/all/all/sk` 
these are at `SK/chasm/chasm-lib/chasm-lib-uni/chasm-lib-uni.pro`, 
`SK/chasm/chasm-tr/chasm-tr-uni/chasm-tr-uni.pro`, and 
`SK/chasm/chasm-vm/chasm-vm-uni/chasm-vm-uni.pro`


## OTQR

In the future I hope to complete a genuine full-text query language, which I naming 
as "`OTQR`", or "Object-Text Query for Research Objects".  I envision this language 
having some similarities to XQFT (XQUERY Full Text), DSL (Dimensions Search Language), 
and OpenSearch Query DSL (Domain-Specific Language).  However, the use-cases 
would be somewhat different.  While potentially useful for web services for 
reverse-index (vector database) queries, `OTQR` is focused on detailed 
searches against individual documents -- or at least, relatively small 
collections wherein it is feasible to examine text directly, without the 
simplifying steps of stemming and reverse-indexing (which tends to 
lose potentially important contextual detail).  Here `OTQR` could be 
embedded in `PDF` viewers, Executable Research Objects, or available as a 
standalone tool for authors and editors.

This current repository has just a basic demo of `OTQR` features and 
implementation.  The `Qt` project `otqr-console` shows a program that 
evaluates one query, returning a list of matching sentences 
(the request specifies file paths to save the list, in lieu of 
printing to the console or constructing a collection of 
objects).  The demo shows how sentence-level representation can 
be useful.  The general query format has search parameters 
(e.g., `:save` and `:in` in this case) indicated by lisp-style 
keywords, and the overall query structure is encoded via 
associating each such parameter to a vector of `QString` lists 
(supplemented by `QVariant` lists for arguments that are 
are non-string-like).  The class `SDI\_Sentence\_Reader` implements 
the actual search over its collection of sentence objects.  For a 
real query system, of course, this search functionality could 
be significantly expanded.

In addition to demonstrating `OTQR` as a kind of query-language prototype, 
this code also suggests some features of `ChasmVM` that, I believes, 
supports its extensibility and embeddability.  Specifically, `OTQR` 
is designed as a layer on top of `ChasmVM`; `OTQR` functions as a 
scripting language that compiles to `ChasmVM` code.  With sufficient 
development, this could incorporate general scripting facilities 
into search code, such as lexically-scoped variables and user-defined procedures 
(both available for inside actual queries).  Query statements are then just 
one form of script statement with a special syntax (here, starting 
with a '`>`' character).  The actual implementation shows 
extensibility features of a `ChasmVM` system.  For one thing, the parser 
is context-sensitive, and statements initially matching the query 
syntax cause the parser to enter a distinct state.  The `VM` generator 
also recognizes a separate channel for tokens parsed within these statements, 
ultimately passed in to a query-handler as a generic input channel, but with a 
distinct set of preparatory operations.  Specific instructions are added both 
to the initial post-parse "`bytecode`" and the later generated `VM` code. 

Similar combinations of special parser states, new instructions, and 
special channels could be used to implement different kinds of add-on layers 
extending `OTQR` either from a scripting or query perspective.
 








