
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GTAGML_DOCUMENT__H
#define GTAGML_DOCUMENT__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/gtagml-dominion.h"

#include "kernel/query/gtagml-query.h"

#include "kernel/grammar/gtagml-parsing-mode.h"


#include "gtagml-document-info.h"

#include "accessors.h"

#include "kans.h"

#include "global-types.h"

#include <QTextStream>

KANS_(GTagML)

class GTagML_Parser;
class GTagML_Grammar;
class GTagML_Parse_State;

class GTagML_Pregraph;
class GTagML_Streams;


class GTagML_Word_Entry_List;

class GTagML_Project_Info;

class GTagML_Annotation_Tile;

class GTagML_Document
{
 GTagML_Graph* graph_;
 GTagML_Parser* parser_;
 GTagML_Parse_State* parse_state_;
 GTagML_Streams* streams_;

 GTagML_Pregraph* pregraph_;
 GTagML_Grammar* grammar_;

 QString local_path_;
 QString raw_text_;

 QString khi_info_;
 QString local_file_name_;
 QString file_job_name_;


 QMap<QString, GTagML_Annotation_Tile* >* annotations_;

 typedef QMap<QString, GTagML_Annotation_Tile* >* annotations_type;

 QMap<QString, GTagML_Annotation_Tile* > local_annotations_;

 GTagML_Parsing_Modes parsing_mode_;

 GTagML_Document_Info document_info_;
 GTagML_Project_Info* project_info_;

 QString sdi_tag_command_info_path_;

 QTextStream* divert_;

 QString man_folder_path_;

 QString top_level_path_;

 QString info_path_;


public:


 ACCESSORS(QString ,local_path)
 ACCESSORS(QString ,raw_text)
 ACCESSORS(GTagML_Graph* ,graph)
 ACCESSORS__GET(GTagML_Grammar* ,grammar)

 ACCESSORS__RGET(GTagML_Document_Info ,document_info)
 ACCESSORS(GTagML_Project_Info* ,project_info)


 ACCESSORS(annotations_type ,annotations)

 ACCESSORS(QString ,khi_info)
 ACCESSORS(GTagML_Parsing_Modes ,parsing_mode)
 ACCESSORS(QString ,local_file_name)
 ACCESSORS(QString ,file_job_name)

 ACCESSORS(QString ,man_folder_path)

 ACCESSORS(QString ,top_level_path)
 ACCESSORS(QString ,info_path)

 ACCESSORS(QTextStream*, divert)


 GTagML_Document();

 GTagML_Document(GTagML_Project_Info* gpi);

 ~GTagML_Document();

 QString get_path_root();

 void insert_latex_template(QString path);
 void insert_xml_template(QString path);

 void save_sentences(QString path);

 void save_jats(QString path, QString bib_path);
 void save_latex(QString path);
 void save_pregraph(QString path);

 QString use_info_path();


 GTagML_Project_Info* init_project_info(QString folder = {});

 void check_sdi_tag_command_info();

 void load_and_parse(QString path, GTagML_Grammar* grammar = nullptr );

 void load_file(QString path);

 void set_grammar(GTagML_Grammar* grammar = nullptr );
 void parse();

 void use_light_xml();

 QString file_name();

 QString get_full_top_level_path();


 QString save_word_count();
 QString save_word_stream();
 QString save_hrefs();
 QString save_light_xml();

 QString save_quotes();

 QString local_or_info_path();

 static void save_file(QString path, QString contents);

 void sdi_check(QString sdi_path, QString out_path);

 static void clean_html(QString& str);
 static QString escape_unicode(QString contents);

// void tag_command_annotation(GTagML_Tile> nt, GTagML_Annotation_Tile> tile);
 void tag_command_annotation(GTagML_Annotation_Tile* tile);

 void write_local_annotations(QString path);
 static void write_annotations(QString path, QMap<QString, GTagML_Annotation_Tile*>& annotations);

};

_KANS(GTagML)


#endif
