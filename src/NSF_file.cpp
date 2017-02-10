//
// Created by bugz on 07/02/17.
//

#include "../include/NSF_file.h"

using namespace std;

vector<audio_data> NSF_file::read(size_t nb_bytes) {
  vector<audio_data> v;
  return v;
}

const NSF_header &NSF_file::getHeader() const { return header; }
