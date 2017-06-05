# DixieAudio
Realtime Nintendo Sound Format(NSF) player.

## Reality
Initially, we were planning to do a NSF player, but we didn't realized the size of the challenge.
Since our time was limited, we went for a 16bits WAV platlyer.

The code works with pulseaudio simple api. The code tries to be modular and easily updatable.
Also, it tries to dynamically manage its buffers and timing to use small amount of resources.
It tries to keep some soft realtime promises. Since it's workig  on linux, it would be
really difficult(impossible?) to be hars realtime.

The code to read the NSF headers is still present.
