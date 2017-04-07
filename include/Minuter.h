#ifndef DIXIEAUDIO_MINUTER_H
#define DIXIEAUDIO_MINUTER_H

#include <chrono>

template <class F> typename std::chrono::system_clock::duration minuter(F f) {
  auto avant = std::chrono::system_clock::now();
  f();
  return std::chrono::system_clock::now() - avant;
}

#endif // DIXIEAUDIO_MINUTER_H
