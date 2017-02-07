//
// Created by bugz on 07/02/17.
//

#ifndef DIKIEAUDIO_AUDIO_DEVICE_H
#define DIKIEAUDIO_AUDIO_DEVICE_H

class audio_device {
public:
  virtual audio_device(audio_header header);
  virtual ~audio_device();
  virtual int write(vector<audio_data> data);
};

#endif // DIKIEAUDIO_AUDIO_DEVICE_H
