
//           Copyright Nathaniel Christen 2026.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "gtagml-pregraph.h"



USING_KANS(GTagML)

GTagML_Pregraph::GTagML_Pregraph()
  :  acc(&acc_)
{

}

void GTagML_Pregraph::write_raw_text(QString text)
{
 QString pre = "| ";
 text.replace("\n", pre);
 text.prepend(pre);
 acc << "\n\n" << pre << "\n\n";
}

void GTagML_Pregraph::new_text_node(u2 ply_id, QString text)
{
 write_raw_text(text);

 acc << ".text-node $ " << ply_id << " ;.\n";

}


