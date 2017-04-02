#ifndef DIXIEAUDIO_MINUTER_H
#define DIXIEAUDIO_MINUTER_H

#include <chrono>

template <class F>
typename system_clock::duration minuter(F f)
{
  auto avant = system_clock::now();
  f();
  return system_clock::now() - avant;
}

#endif //DIXIEAUDIO_MINUTER_H
