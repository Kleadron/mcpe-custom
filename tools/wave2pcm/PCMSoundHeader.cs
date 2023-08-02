/********************************************************************
	mcpe-custom Wave to PCM file conversion tool
	Copyright (C) 2023 Kleadron
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

 // This file represents the C# version of the structure at source/Sound/SoundData.hpp

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Wave2PCM
{
    public struct PCMSoundHeader
    {
        public int Channels;
        public int BytesPerSample;
        public int SampleRate;
        public int Length;
        // Raw sample data should follow directly after this part of the header
    };
}
