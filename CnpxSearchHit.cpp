#include "CnpxSearchHit.h"

using namespace std;

CnpxSearchHit::CnpxSearchHit(){
  hit_rank=0;
  num_tot_proteins = 0;
  num_matched_ions = 0;
  tot_num_ions = 0;
  calc_neutral_pep_mass = 0;
  massdiff = 0;
  num_tol_term = -1;
  num_missed_cleavages = -1;
  num_matched_peptides = 0;
  is_rejected = 0;
  calc_pI = 0;
  protein_mw = 0;
  peptide_start_pos=0;
}

CnpxAlternativeProtein* CnpxSearchHit::addAlternativeProtein(std::string protein){
  CnpxAlternativeProtein s;
  s.protein=protein;
  alternative_protein.push_back(s);
  return &alternative_protein.back();
}

CnpxSearchScore* CnpxSearchHit::addSearchScore(std::string name, std::string value){
  CnpxSearchScore s;
  s.name=name;
  s.value=value;
  search_score.push_back(s);
  return &search_score.back();
}

string CnpxSearchHit::getModifiedPeptide(){
  if(modification_info.modified_peptide.size()==0) return peptide;
  size_t i,j;
  char str[32];
  string pep;
  for(i=0;i<peptide.size();i++){
    pep+=peptide[i];
    for(j=0;j<modification_info.mod_aminoacid_mass.size();j++){
      if(modification_info.mod_aminoacid_mass[j].position==(i+1)){
        sprintf(str,"[%.2lf]",modification_info.mod_aminoacid_mass[j].mass);
        pep+=str;
        break;
      }
    }
  }
  return pep;
}

void CnpxSearchHit::write(FILE* f, int tabs){
  size_t i;

  string el = "search_hit";
  if (peptide.empty()) NPXerrMsg(el, "peptide");
  if (protein.empty()) NPXerrMsg(el, "protein");
  if (hit_rank == 0) NPXerrMsg(el, "hit_Rank");

  int t = tabs;
  if (t>-1) t++;
  NPXprintTabs(f, tabs);

  fprintf(f, "<search_hit hit_rank=\"%d\"",hit_rank);
  fprintf(f, " peptide=\"%s\"", peptide.c_str());
  if (!peptide_prev_aa.empty()) fprintf(f, " peptide_prev_aa=\"%s\"", peptide_prev_aa.c_str());
  if (!peptide_next_aa.empty()) fprintf(f, " peptide_next_aa=\"%s\"", peptide_next_aa.c_str());
  if (peptide_start_pos>0) fprintf(f, " peptide_start_pos=\"%d\"", peptide_start_pos);
  fprintf(f, " protein=\"%s\"", protein.c_str());
  fprintf(f, " num_tot_proteins=\"%d\"", num_tot_proteins);
  if(num_matched_ions>0) fprintf(f, " num_matched_ions=\"%d\"", num_matched_ions);
  if (tot_num_ions>0) fprintf(f, " tot_num_ions=\"%d\"", tot_num_ions);
  fprintf(f, " calc_neutral_pep_mass=\"%.6lf\"", calc_neutral_pep_mass);
  if(massdiff!=0) fprintf(f, " massdiff=\"%.6lf\"", massdiff);
  else fprintf(f, " massdiff=\"0\"", massdiff);
  if (num_tol_term>-1) fprintf(f, " num_tol_term=\"%d\"", num_tol_term);
  if (num_missed_cleavages>-1) fprintf(f, " num_missed_cleavages=\"%d\"", num_missed_cleavages);
  if (num_matched_peptides>0) fprintf(f, " num_matched_peptides=\"%d\"", num_matched_peptides);
  if (!xlink_type.empty()) fprintf(f, " xlink_type=\"%s\"", xlink_type.c_str());
  if (is_rejected>0) fprintf(f, " is_rejected=\"%d\"", is_rejected);
  if (!protein_descr.empty()) fprintf(f, " protein_descr=\"%s\"", protein_descr.c_str());
  if (calc_pI>0) fprintf(f, " calc_pI=\"%.6lf\"", calc_pI);
  if (protein_mw>0) fprintf(f, " protein_mw=\"%.2lf\"", protein_mw);
  fprintf(f, ">\n");

  for (i = 0; i < alternative_protein.size(); i++) alternative_protein[i].write(f,t);
  if(!modification_info.modified_peptide.empty()) modification_info.write(f,t);
  if(!xlink.identifier.empty()) xlink.write(f,t);
  for (i = 0; i < search_score.size(); i++) search_score[i].write(f,t);
  for (i = 0; i < analysis_result.size(); i++) analysis_result[i].write(f,t);

  NPXprintTabs(f, tabs);
  fprintf(f, "</search_hit>\n");

}
