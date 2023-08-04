#include <dsound.h>

// needs to be isolated because of stupid type 
void ReleaseDirectSoundBuffer(LPDIRECTSOUNDBUFFER buffer)
{
	IDirectSoundBuffer_Release(buffer);
}