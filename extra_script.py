import os
import subprocess
import sys

try:
    from pydub import AudioSegment
    from pydub.silence import detect_nonsilent
except ImportError:
    subprocess.check_call([sys.executable, "-m", "pip", "install", "pydub"])
    from pydub import AudioSegment
    from pydub.silence import detect_nonsilent


def trim_silence(audio_path, silence_threshold=-30.0, chunk_size=10):
    audio = AudioSegment.from_file(audio_path)
    non_silent_ranges = detect_nonsilent(audio, min_silence_len=chunk_size, silence_thresh=silence_threshold)
    trimmed_audio = sum([audio[start:end] for start, end in non_silent_ranges])
    trimmed_audio.export(audio_path, format='wav', parameters=["-ac", "1", "-ar", "22050"])

mojis = 'あいうえおかきくけこがぎぐげごさしすせそざじずぜぞたちつてとだぢづでど'\
    'なにぬねのはひふへほばびぶべぼぱぴぷぺぽまみむめもやゆよらりるれろわをん'

output_dir = 'src'
header = f'{output_dir}/kerowav.hpp'

if not os.path.exists(header):
    print('generate wavedata...')
    with open(header, 'w', encoding='utf-8') as f:
        f.write('#pragma once\n');
        f.write('#include <array>\n')
    
    for i, c in enumerate(mojis):
        print(c, end='')
        filename = f"{output_dir}/{i}.wav"
        os.system(f'say -o "{filename}" --data-format=LEI16@44100 "{c}"')
        trim_silence(filename)
        os.system(f'xxd -i "{filename}" | sed "s/^unsigned /const unsigned /" >> {header}')
        os.remove(filename)

    print(len(mojis))
    with open(header, 'a', encoding='utf-8') as f:
        f.write(f'constexpr std::array<std::pair<const char*, const unsigned char*>, {len(mojis)}> mojiarray = {{{{\n')
        for i, c in enumerate(mojis):
            f.write(f'{{"{c}", src_{i}_wav}},\n');
        f.write('}};\n')
