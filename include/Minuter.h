#ifndef DIXIEAUDIO_MINUTER_H
#define DIXIEAUDIO_MINUTER_H

#include <boost/chrono.hpp>

template <class F> typename boost::chrono::system_clock::duration minuter(F f) {
  auto avant = boost::chrono::system_clock::now();
  f();
  return boost::chrono::system_clock::now() - avant;
}

#endif // DIXIEAUDIO_MINUTER_H
