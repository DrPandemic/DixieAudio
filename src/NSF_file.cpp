#include "../include/NSF_file.h"
#include "../include/audio_file.h"

using namespace std;

NSF_file::~NSF_file() {}

vector<audio_data> NSF_file::read(size_t nb_bytes) {
  vector<audio_data> v;
  return v;
}

const NSF_header &NSF_file::getHeader() const { return header; }
