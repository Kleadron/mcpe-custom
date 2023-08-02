/********************************************************************
	mcpe-custom Wave to PCM file conversion tool
	Copyright (C) 2023 Kleadron
	
	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using WAV_Tools_C_Sharp;

namespace Wave2PCM
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Usage: Wave2PCM.exe infile.wav outfile.pcm");
                return;
            }

            string inputPath = args[0];
            string outputPath = args[1];

            if (!File.Exists(inputPath))
            {
                Console.WriteLine("Input wave file does not exist");
                return;
            }

            string outDirectory = Path.GetDirectoryName(outputPath);

            // make sure the output path exists
            if (outDirectory.Length > 0 && !Directory.Exists(outDirectory))
            {
                Directory.CreateDirectory(outDirectory);
            }

            Console.WriteLine("Reading wave file");

            // interesting it wants the directory and the filename separate
            WAV_file waveFile = new WAV_file(Path.GetDirectoryName(inputPath), Path.GetFileName(inputPath));

            waveFile.loadFile();

            if (!(waveFile.NumOfChannels == WAV_file.NUM_CHANNELS.ONE || waveFile.NumOfChannels == WAV_file.NUM_CHANNELS.TWO))
            {
                Console.WriteLine("Wave file has bad channel count");
                return;
            }

            if (waveFile.BitsPerSample != WAV_file.BITS_PER_SAMPLE.BPS_16_BITS)
            {
                Console.WriteLine("Wave file must be 16-bit");
                return;
            }

            PCMSoundHeader header = new PCMSoundHeader()
            {
                Channels = (int)waveFile.NumOfChannels,
                BytesPerSample = 2,
                SampleRate = (int)waveFile.SampleRate,
                Length = (int)waveFile.NumberOfSamples
            };

            if (header.Channels == 2)
                header.Length *= 2;

            Console.WriteLine("Writing PCM file");

            FileStream outStream = File.Create(outputPath);
            BinaryWriter w = new BinaryWriter(outStream);

            w.Write(header.Channels);
            w.Write(header.BytesPerSample);
            w.Write(header.SampleRate);
            w.Write(header.Length);

            if (waveFile.NumOfChannels == WAV_file.NUM_CHANNELS.ONE)
            {
                short[] samples;
                int numSamples = waveFile.getBuffer_16_bits_mono(out samples);
                
                for(int i = 0; i < numSamples; i++)
                {
                    w.Write(samples[i]);
                }
            }
            else if (waveFile.NumOfChannels == WAV_file.NUM_CHANNELS.TWO)
            {
                short[] samplesLeft;
                short[] samplesRight;
                int numSamples = waveFile.getBuffer_16_bits_stereo(out samplesLeft, out samplesRight);

                for (int i = 0; i < numSamples; i++)
                {
                    w.Write(samplesLeft[i]);
                    w.Write(samplesRight[i]);
                }
            }

            w.Close();
            outStream.Close();

            Console.WriteLine("Finished!");
        }
    }
}
