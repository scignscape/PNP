
# ScignPNP: Science Grid Nodes — Publishing for Nonprofits

If you are visiting this repository to examine code I presented at the NISO/JATS conference of 2026, please consult the "prelim" branch `README-chasm.txt`.  Other "work in process" within this branch is explained in similar readme files.

What follows is an informal, thematic overview.  A summary of the included source code is further down this 
page (or [skip ahead](#technical-overview-raised)).  

For a description of several projects that employed some of the code published here scroll down or click 
here.  A list of open-access articles and book chapters created via this code is also further down on this 
page ([skip ahead](#sample-documents-raised)).

**_Personal Note_**
I have developed and used the code described here over a multi-year period.  Some 
of this code was originally designed to be part of a commercial product (at least on one version) 
by a startup I was working for (see [here](https://raw.githubusercontent.com/scignscape/PNP/main/documents/PacTk/Publishing-Accelerator-Toolkit.pdf) for a summary of this work in a more commercial 
context).  Prior to the Covid pandemic, we were also engaged with implementations for 
Electronic Medical Records, clinical informatics, and diagnostic imaging, but transitioned 
to focus more on software for publishing during the "stay-at-home" phase 
(some EMR and imaging components can be folded into Research Object class 
libaries)  During this phase we took on several pro-bono projects to help 
develop this technology, so self-consciously adopting the technology 
to a nonprofit context is an organic next step.

—-

### Overview

The focus of this project is providing free publishing services to nonprofit and/or charitable organizations.  

ScignPNP places a special emphasis on technology associated with academic and scientific research.  Although manuscripts with only basic-level requirements are certainly supported, the code shared here might be especially useful to those who seek to develop publications alongside data sets, multimedia presentations, and other supporting materials.

In contemporary academic publishing, the basic unit for sharing experience and ideas often is not a single manuscript, but rather a collection of files packaging variegated content-types, including PDF files for viewing documents, but also machine-readable text encoding, computer code, raw data, and statistical/graphics summaries.  These packages typically adhere to collaborative standards such as Research Object Bundles, Executable Research Objects, `FAIR`sharing (Findable, Accessible, Interoperable, Reusable), and research-grid specifications for packages deposited on decentralized but searchable community networks.

The computer code published with this repository can help authors and digital editors create publications that adhere to standards such as `FAIR`sharing and Executable Research Objects.  Code may be specialized for each project.  

If desired, 
ScignPNP can set up a GitHub repository to hold and/or share any files, data, and code associated with a 
project.  This is also a no-cost way to host documents online.  Via GitHub.io pages, repositories can also 
serve as a free-tier web hosting platform (an example of a site I created for demonstration purposes, 
that could serve as a template for folder layout and supporting code, is [here](https://sledproc.github.io/baltimore-files/)).  

### The "Diamond" Publishing Model

In the genre of open-access publishing, the "diamond" model indicates documents that 
are free for both readers and authors.  This paradigm is usually contrasted with a 
"gold" model where documents are free for readers but require authors to pay 
"processing charges" (APCs).  The diamond model is also sometimes contrasted with a "green" 
model where authors create private copies of articles whose "official" versions are 
behind a paywall.

Another important feature of most diamond systems is that authors retain all copyrights 
to their work.  They are free to share and disseminate their writing however they choose.

In traditional scientific publishing, research data sets and other supplemental 
materials are open-source while the actual books or articles are paywalled as 
commercial products.  One problem with this paradigm is that publications 
cannot be distributed alongside their complementary assets.  This means that 
cross-references between publications and other materials, such as data sets and 
computer code, cannot be rigorously defined.  Also, publications cannot benefit 
from technological integration with associated resources, via techniques 
such as customized PDF viewers specifically designed for a shared data package.

In brief, the diamond model is not only more ethical; it also permits technological 
enhancements that are impossible otherwise.  In ScignPNP all projects are 
assumed to follow the diamond model unless authors specifically choose 
an alternative.

Furthermore, ScignPNP maintains a list of diamond-open-access journals that authors 
can consult if they may wish to submit their writing to a technical journal 
without paywalls, Author Processing Charges, or relinquishing control over their manuscripts.


###  Science Grid Technology for Nonprofits

A science grid increases the visibility of its published projects and also helps them benefit from shared technology.
I believe nonprofits can similarly benefit.  A "nonprofit grid" would help individuals discover nonprofits addressing issues of interest to them, and potentially share computer code and other digital resources.  Creating research-worthy publications informed by nonprofits' mission, experience, and observations is different from a full-fledged research grid, but it's a good start and valuable in its own right.

Aligning with recognized scientific and academic standards helps nonprofits' work get recognized and incorporated into research ecosystems.  Ideally, pure research institutions and organizations outside of academia — especially nonprofits — share a symbiotic relationship.  In particulr, the concepts of "translational medicine" and "translational science" are increasingly emphasized in contemporary research and focus on how theoretical investigations can be "translated" to social benefits.  All research is motivated in part by potential real-world applications, but the discipline of translational science develops rigorous models and guidelines to help organizations implement socially-conscious protocols, with special attention to community outreach, data management, ongoing assessment, and projects' sustainability.

In the "translational" environment, academic and non-academic institutions are intrinsically linked.  Nonprofits can contribute to these partnerships by systematically warehousing information that could be useful to scientists and other researchers; by examining their day-to-day activities to consider whether there are data or observations that could be incorporated into research projects; by encouraging staff or volunteers to develop studies that could be submitted to technical journals and/or shared with the general public; and by networking with educational institutions in consideration of collaborations, internships, data sharing, and so forth.

###  Use Cases

Nonprofits focused on public health, education, environmental preservation, community development, and similar areas may find it especially beneficial to develop research collaborations.  But similar ideas apply to many forms of nonprofits and charities.  Concomitant research may be associated with natural/biomedical sciences, humanities, or both.  For instance, groups that work with underprivileged and marginalized communities may have a need for individuals with sociological/social-work training or language and communication experts, who may in turn benefit from experience gained "in the field".  

At the same time, the need to maintain health records, intake forms, databases, statistical modules, and user-interface tools can serve to integrate nonprofits into the broader ecosystem of EMR/EHR (Electronic Medical and Health Records) and bioinformatics.  

With that said, traditional publishing — focused on books and articles — is of course important to nonprofits as well.  ScignPNP will certainly work with organizations whose main emphasis is creating materials to learn about their mission, history, and the everyday stories that make their work worthwhile.  Hopefully code in this repository can facilitate both traditional and scientific/data publishing.

<a id="technical-overview-raised"></a>


## Technical Overview

There are three main software components to ScignPNP.

###  GTagML ("Grounded" TagML)  
This is an input language for composing 
manuscripts at the start of the ScignPNP pipeline.  On the surface, `GTagML` 
is similar to Markdown, albeit with many additional features 
and constructions.  Behind the scenes, `GTagML` text encoding is 
conceptually based on the Text As Graph Markup Language (`TagML`) although 
the implementation has no direct connection to `TagML` 
toolsets.  The term "grounded" is intend to signify that sections of markup 
may be identified as serializing objects of a specified type.

`GTagML` is readily extensible (compared to typical markup languages).  C++ programmers can add new features by 
inserting new parsing rules and IR (intermediate representation) or 
output handlers.  `GTagML` uses a pure-C++ parser, with no `LEX/YACC` style 
separate compilation step.  The `GTagML` grammar has two layers of 
contextual flags that activate or deactivate specific rule patterns.  All 
rules query the immediate next character and thereafter in a 
parse-unfold, using a specialized Regular Expression language.  Each 
rule is connected to a callback that executes when the match 
hits, preempting any additional rules declared later in the grammar.  The 
callback may produce code for a desired output format (`LaTeX`, `XML`, 
etc.) immediately, or contribute to a graph structure to be 
traversed during post-parsing, or both.

Commonplace styling and layout patterns are expressed in `GTagML` 
similar to Markdown, so authors familiar with Markdown 
(from social media, github, etc) should find it easy to 
write basic documents in `GTagML`.  However, for ScignPNP 
publishing services, conversion from other document 
formats (such as MS Word) to `GTagML` is normally 
part of the overall workflow implemented on authors' behalf.

###  TSOM (Topomorphic Sentence Object Model)  
This refers to 
the encoding of textual elements (such as sentences, paragraphs, 
keyphrases, and semantic annotations) as objects of programming 
languages such as `C++`.  `TSOM` classes also cover "discursive" 
elements such as block quotes, footnotes, citations, and 
list environments (bulleted, itemized, or enumerated 
quasiparagraphs).  By design, `TSOM` encodes textual content 
via navigable object-structures that can be traversed 
by analogy to `XML` or other markup formats (I discuss 
"topomorphic" visitor-pattern algorithms in greater 
detail in the NISO/JATS presentation linked above). 

Aside from ubiquitous elements of written texts such 
sentences and block quotes, most individual publications 
will have their own collection of meaningful segments 
depending on their subject matter: chemical formulae, 
linguistics annotations, special symbols, etc.  By 
design, this material is encoded via objects of specialized 
types for queries and indexing.  In general, then, the 
specific `TSOM` classes employed for a particular 
document or manuscript-collection are implemented 
separately for each project. 

###  ChasmVM (Channel/Syntagm Virtual Machine)  
The goal of `ChasmVM` is to provide a unified basis 
for special-purpose code related both to text 
documents (e.g., full-text queries) and to 
data sets (e.g., deserializing raw data 
files).  In contexts such as Executable Research 
Objects, internal code (included in the package as 
open-access source files) will typically 
manage a unified information space covering 
both textual content and the raw data 
observations, measurements, or samples serialized 
in a data set.  Scripting or query languages 
can support `ChasmVN` bytecode as a compilation 
target to facilitate integration among 
multiple content-/media-types in a Research Object package. 

In the past, I have also used `ChasmVN` (in preliminary versions) 
for development tasks such as performing unit tests, in a 
dataset environment.  `ChasmVN` is especially focused 
on interop with `Qt` applications, so that it hopefully 
can be used for `GUI` and plugin scripting to help 
fine-tune software applications to load Research Object 
data sets and present their contents to users in 
accessible, interactive ways (consistent with `FAIR`sharing). 
Executable Research Objects need an execution environment; 
this could be implemented directly in the dataset code 
or supported by a host application where the data set is 
loaded.  For Research Objects reflecting a specific 
scientific discipline or academic field, software 
commonly used for investigations and laboratories 
focused on those subject-areas may provide this 
host context.  Typically, however, individual 
data sets have requirements related to application 
state and user interactions that must 
be implemented within the host runtime, and 
load scripts may be used to satisfy such requirements.

Compared to typical `VM`, compiler, and runtime 
solutions, `ChasmVM` has certain unique features embodying 
its specific focus on data publishing and Research 
Objects.  For one thing, the entire `ChasmVM` compiler and bytecode 
stack is designed to be self-contained, with minimal 
external dependencies.  In particular, it may be 
included as source code alongside other dataset 
code.  Everything needed to run bytecode scripts or 
files that compile to them is thereby part of the 
Research Object itself (apart from standard requirements 
typical of any code in the relevant programming 
languages, such as a `C++` compiler and `Qt` libraries 
for data sets with cross-platform front 
ends).  Second, `ChasmVM` supports type-interface designs 
optimized for data-publishing, in terms of 
the use and documentation of class libraries; notation 
and enforcement of procedural preconditions; semantic 
indexing for computer code and `GUI` elements 
(context menu options, drop-down combos, etc.); and 
cross-referencing between data, code, and 
text publications.  In particular, Design by Contract 
(`DbC`) coding conventions, as well as the utilization 
of unit-decorated and related dimensional/coordinate 
types (expressing units of measurement, `SI` pairings, 
coordinate axiation, etc.), helps clarify Research Objects' 
data profile and analytic tactics, and more generally 
the theoretical and experimental frameworks guiding 
published research.

With this background, `ChasmVM` supports front-end 
languages implementing a variety of constructions 
related to contract enforcement, pointer/reference 
arguments and return values, memory/lifetime management, 
template-metaprogramming dependent types, 
metadata attached to lexical scope, and other 
language engineering details with some 
noteworthy differences from conventional programming 
languages.  Much of this work is still in an experimental 
stage, but hopefully can serve as a prototyping 
compiler and runtime for use by scripting 
and/or query languages explicitly targeting 
Executable Research Object contexts.  

Academic publications where I have discussed various 
technical features of `ChasmVM` (including the 
general idea of calling-convention channels and Syntagmatic 
Graphs) are listed among the documents in the next section.


###  Organization of this Repository

Demonstration code for `GTagML` and `ChasmVM` is included 
in the `prelim` branch.  This code is still under development, 
but it represents components I have adopted for practical 
use cases and could serve as a components to look over 
for anyone who would like to learn more about 
ScignPNP implementations. 

The `gui-dev` branch includes some project-specific `C++` 
classes.  This material is mostly implementation for `GUI` windows intended 
to be provided as part of data sets with specific front-end requirements, 
such as videos, image series, and digital maps for `GIS` 
displays.  The code here was focused on preparations for specific 
publications and therefore not particularly well documunted or 
refined.  However, over time hopefully I can elevate these classes 
to a more general library of `GUI` classes for reuse in multiple 
Executable Research Objects.

The `mid` branch includes experimental code related to `IR` transforms 
for mid-end compiler pipelines subsequent to parsing but prior 
to bytecode generation.  


—- 


# Projects and Documents Using ScignPNP

Due to privacy or copyright issues, for some of these examples 
I can only describe projects and their unique coding 
requirements.  In other cases this list will include 
links to `PDF` documents illustration some ScignPNP features.

###  Projects Involving Data and/or Code Packages

1.  CSV and SVG Integration:  This project coincided with the five-year cycle 
for the Environmental Protection Agency's "Hazard Mitigation" plan.  As part 
of this `EPA` program, states and counties are required to submit documentation 
for how they identify and address environmental dangers in their 
communities.  Here, I used ScignPNP tools to address both environmental 
data and Zoning/Land Use ordinances so as to explore the connections 
between environmental health and urban development.  Part of this 
project involved extending a New York City tool called 
`ZoLa` (for Zoning and Land Use), which was created by 
NYC Planning Labs but openly shared in the hopes that 
other jurisdictions adopt the same technology.  Other 
data sourecs came from the `EPA` "Toxic Release Inventory" (`TRI`) 
data set, which publishes data files identifying reported 
contamination incidents by geographic coordinates, and 
has been studied for publications such as the NYC "Environmental Justice" 
(`EJNYC`) report and mapping tool.  My goal was to examine how 
data sets such as `TRI`, along with environmental-impact 
data presented by states or private organizations, could be cross-referenced 
with Zoning and Land Use maps.  Demonstration code included `CSV` deserializers 
to handle to complex `TRI` data structures, that were hierarchical 
more than tabular in nature but used `CSV` as a rough serialization tool, 
as well as native-compiled `GUI` components based on `OpenStreetMap` 
for `GIS` displays.

    During this work I identified a series of errors with the `TRI` encoding and 
documentation, so another layor of components implemented for this project 
involved mapping `TRI`-related `PDF` files to `SVG`, so that I could 
superimpose annotations identifying the `TRI` problems and 
how they could be corrected in code. 


2.  Annotated Legal Documents:  This was another project leveraging 
`PDF`-to-`SVG` conversion.  The basic material for this project 
was a collection of legal documents that had been merged into a 
single file and then annotated to show patterns of 
behavior.  Applying techniques from ethnomethodology, the annotator 
designed a controlled vocabuly to insert terms functioning 
as "handles" into `PDF` comment boxes.  I then implemented 
code which scanned these `PDF` annotations and identified 
sub-documents which, according to the annotator, 
exhibited various patterns of communicative behavior.  I then 
split the larger `PDF` into multiple subdocuments and mapped 
that content to hybrid `HTML`/`SVG` pages, together with an 
online index that showed annotated classifications for 
each subdocument, plus page-by-page as well as document-by-document 
navigation for the linked pages.  The overall package was presented 
to a judge as a user-friendly alternative to submitting the 
original manuscript as a monolithic item of evidence. 

3.  Building a Book's Second Edition:  This project involved merging 
additional material added by the authors into a book they had 
initially published in 2005.  We built an in-house full `PDF` 
of the new edition that mimicked the style and layout of the 
preparatory manuscripts shared by the publishers.  By cross-referencing 
the two versions, we were able to identify errors made on the 
publisher's end by copy editors and also fine-tune the new material.
Significant effort went into creating and updated index.  Special 
ScignPNP tools laid a foundation for custom `GUI`s whose goal 
was to compare the first- and second-edition manuscripts to 
ensure that all previous index entries were migrated to the 
updated book, as well as creating new index material in the 
pbulisher's desired metadata format.

4.  Examining Biomedical Software:  During the Covid pandemic, 
I wrote the text for a book called _Innovative Data Integration and Conceptual 
Space Modeling for COVID, Cancer, and Cardiac Care_ (Elsevier, 
2022).  This book included a detailed examination of certain 
software components — such as the Cancer Phenomics Toolkit (`CapTk`), 
`IQmol` molecular visualization software, `cytolib` for flow cytometry, 
`medInria` and `Semantic PACS` (Picture Archiving and Communication System), 
and the Cancer Bioinformatics Grid (`caBig`) Image Annotation Toolkit — 
bioinformatics file formats (and their deserialization/analytic libaries), 
and collaborative projects (such as `caBig`'s "Digital Model Repository" 
and the `CORD-19` archive for Covid research).  `CORD-19` (the 
COVID-19 Open Research Dataset) grew to encompass over 400,000 open-access 
articles focusing on SARS-COV-2 and related topics (including the original 
SARS epidemic and Coronavirus biology in general) with freely-available 
full-text encoding.  The goal of `CORD-19` was to help scientists in 
different areas addressing the pandemic (viral morphology, infectious 
mechanisms, diagnostics, clinical treatment assesment, vaccine development, 
genomics, variants/mutations, epidemiology, long-term effects) discover 
resources or research that could augment their own work, helping 
piece together the Covid puzzle.  Unfortunately, much of `CORD-19`'s
implementation depended on `PDF` text extraction, which 
yielded errors and limitations I analyzed in the Elsevier book.
The `CORD-19` curators (from Allen Institute for AI) were open about these problems and 
even issued a "call to action" requesting publishers to 
develop and adopt more rigorous text-encoding 
methods:

    >   Though the full text of many scientific papers are available to researchers through `CORD-19`, a number of challenges prevent easy application of `NLP` and text mining techniques to these papers. First, the primary distribution format of scientific papers — `PDF` — is not amenable to text processing. The PDF file format is designed to share electronic documents rendered faithfully for reading and printing, and mixes visual with semantic information. Significant effort is needed to coerce `PDF` into a format more amenable to text mining, such as `JATS` `XML`, `BioC`, ... or `S2ORC` `JSON` ... [W]e can still benefit from better `PDF` parsing tools for scientific documents. As a complement, scientific papers should also be made available in a structured format like `JSON`, `XML`, or `HTML`.

    Hopefully ScignPNP can contribute to this initiative. 

    Apart from text representations responding to the Allen Institute 
"call to action", the special code for this book involved 
compiling and, in some cases, extending bioinformatics 
or scientific software applicable to biomedical 
research.  One of the book's themes was how to implement data-integration 
protocols that could unify disparate applications into 
executable research collaborations.  Much of the book's discussion 
was shaped by prototypes for plugins or extensions 
to software commonly used for research or lab services 
in fields such as organic chemistry, tumor simulations, 
systems biology, bioimaging, and Computer Vision.  


<a id="sample-documents-raised"></a>


## Sample Documents

The following are examples of my own publications 
developed via ScignPNP (not in chronological order).

- [Merging Full-Text Query with Research Data Sets: A perspective from compiler theory](https://raw.githubusercontent.com/scignscape/PNP/main/documents/A-perspective-from-compiler-theory.pdf)  Presented at NISO/JATS 2026.

- [Ordering Conceptual Synthesis: The Implications of Cognitive Phenomenology
for Syntactic Theory](https://raw.githubusercontent.com/scignscape/PNP/main/documents/Ordering-Conceptual-Synthesis.pdf)  A paper about linguistics and the philosophy of science, which uses 
programming languages as case-studies for some 
linguistic topics.  The ScignPNP code 
for this manuscript included document-specific 
algorithms to isolate linguistic samples and present 
them alongside the index as a kind of thematic summary.

- "From 'Naturalizing Phenomenology' to Formalizing 
Cognitive Linguistics (I-III)"  These three papers were 
developed in connection with supplemental materials 
for an article in the International Journal of 
Speech Technology, where I joined as a co-author 
with researchers from Bar-Ilan University (Israel) 
and Johns Hopkins (US).  The supplemental content 
was a data set including audio and written 
samples for linguistic annotation, including some 
taken as "hypothetical" sentences or fragments 
created to illustrate linguistic themes or 
theories.  I wrote three supplemental essays to analyze 
material present in the data set.  Within those 
papers, I notated new or previously-discussed 
linguistic samples, and employed ScignPNP to 
pull those examples as structured data available 
to readers as part of the dataset package; in short, 
the code demonstrated techniques for compiling 
linguistic data sets from example 
sentences/fragments listed for analysis in linguistics papers.

1. [From 'Naturalizing Phenomenology' to Formalizing Cognitive Linguistics (I):
Cognitive Transform Grammar](https://raw.githubusercontent.com/scignscape/PNP/main/documents/ijst/ctg.pdf)

2. [From 'Naturalizing Phenomenology' to Formalizing Cognitive Linguistics (II):
Grounding and Center/Peripheral Relations](https://raw.githubusercontent.com/scignscape/PNP/main/documents/ijst/icg.pdf)

3. [From 'Naturalizing Phenomenology' to Formalizing Cognitive Linguistics (III):
Externalism and the Interface Theory of Meaning](https://raw.githubusercontent.com/scignscape/PNP/main/documents/ijst/itm.pdf)


- Five chapters from the book _AI, IoT, Big Data and Cloud Computing for Industry 4.0_ 
(Amy Neustein, Parikshit N. Mahalle, Prachi Joshi, Gitanjali Rahul Shinde, eds., 
Springer 2024: [https://link.springer.com/book/10.1007/978-3-031-29713-7](https://link.springer.com/book/10.1007/978-3-031-29713-7)).

1. [Chapter 20 — Multi-Component Interoperability and Virtual Machines: Examples from
Architecture, Engineering, Cyber-Physical Networks, and Geographic Information Systems](https://raw.githubusercontent.com/scignscape/PNP/main/documents/iot/ch20.pdf)

2. [Chapter 21 — Virtual Machines and Hypergraph Data/Code Models: Graph-Theoretic
Representations of Lambda-Style Calculi](https://raw.githubusercontent.com/scignscape/PNP/main/documents/iot/ch21.pdf)

3. [Chapter 22 — GUI Integration and Virtual Machine Constructions for Image Processing:
Phenomenological and Database-Engineering insights into Computer Vision](https://raw.githubusercontent.com/scignscape/PNP/main/documents/iot/ch22.pdf)

4. [Chapter 23 — The Missing Links Between Computer and Human Languages: Animal Cognition
and Robotics](https://raw.githubusercontent.com/scignscape/PNP/main/documents/iot/ch23.pdf)

5. [Chapter 24 — GUIs, Robots, and Language: Toward a Neo-Davidsonian Procedural Semantics](https://raw.githubusercontent.com/scignscape/PNP/main/documents/iot/ch24.pdf)







 






      



