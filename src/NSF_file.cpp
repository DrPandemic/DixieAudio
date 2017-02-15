#include "../include/NSF_file.h"
#include "../include/audio_file.h"
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

NSF_header::NSF_header(std::istream &file_stream) {
  std::copy_n(std::istream_iterator<char>(file_stream), 5,
              back_inserter(format_file));

  char v_tmp;
  std::copy_n(std::istream_iterator<char>(file_stream), 1, &v_tmp);
}

NSF_file::NSF_file(std::istream &file_stream)
    : audio_file{file_stream}, header{file_stream} {}

NSF_file::~NSF_file() {}

vector<audio_data> NSF_file::read(size_t nb_bytes) {
  vector<audio_data> v;
  return v;
}

const NSF_header &NSF_file::getHeader() const { return header; }
