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
    trimmed_audio.export(audio_path, format='wav')


mojimap = {
    'あ': 'a', 'い': 'i', 'う': 'u', 'え': 'e', 'お': 'o',
    'か': 'ka', 'き': 'ki', 'く': 'ku', 'け': 'ke', 'こ': 'ko',
    'が': 'ga', 'ぎ': 'gi', 'ぐ': 'gu', 'げ': 'ge', 'ご': 'go',
    'さ': 'sa', 'し': 'si', 'す': 'su', 'せ': 'se', 'そ': 'so',
    'ざ': 'za', 'じ': 'ji', 'ず': 'zu', 'ぜ': 'ze', 'ぞ': 'zo',
    'た': 'ta', 'ち': 'ti', 'つ': 'tu', 'て': 'te', 'と': 'to',
    'だ': 'da', 'ぢ': 'ji', 'づ': 'zu', 'で': 'de', 'ど': 'do',
    'な': 'na', 'に': 'ni', 'ぬ': 'nu', 'ね': 'ne', 'の': 'no',
    'は': 'ha', 'ひ': 'hi', 'ふ': 'fu', 'へ': 'he', 'ほ': 'ho',
    'ば': 'ba', 'び': 'bi', 'ぶ': 'bu', 'べ': 'be', 'ぼ': 'bo',
    'ぱ': 'pa', 'ぴ': 'pi', 'ぷ': 'pu', 'ぺ': 'pe', 'ぽ': 'po',
    'ま': 'ma', 'み': 'mi', 'む': 'mu', 'め': 'me', 'も': 'mo',
    'や': 'ya', 'ゆ': 'yu', 'よ': 'yo',
    'ら': 'ra', 'り': 'ri', 'る': 'ru', 'れ': 're', 'ろ': 'ro',
    'わ': 'wa', 'を': 'wo', 'ん': 'n'
}

output_dir = 'data'
os.makedirs(output_dir, exist_ok=True)

for c, f in mojimap.items():
  filename = f"{output_dir}/{f}.wav"
  if not os.path.exists(filename):
    os.system(f'say -o "{filename}" --data-format=LEI16@44100 "{c}"')
    trim_silence(filename)
