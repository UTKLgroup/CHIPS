#include "Riostream.h"
#include "TString.h"
#include "iostream"
#include "sstream"
#include <stdio.h>
#include <string.h>

void test_file() {

  string fname="tmp.root";

  TFile *input = new TFile(fname.c_str(),"READ");
  if( input->IsZombie() ){
    cout << " zombie " << endl;
    exit(0);
  }

  cout << " good " << endl;
  exit(0);


}

