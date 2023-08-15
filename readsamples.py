
from pydub import AudioSegment

# Load the MP3 file
mp3_file = "Sultans of Swing.mp3"
audio = AudioSegment.from_mp3(mp3_file)

# Get the frame rate (frames per second) and number of channels
frame_rate = audio.frame_rate
num_channels = audio.channels

# Calculate the frame size for 8 seconds
desired_duration = 8 * 1000  # 8 seconds in milliseconds
frame_size = int((desired_duration * frame_rate) / 1000)

# Split the desired portion of audio into frames
frames = [audio[i:i + frame_size] for i in range(0, len(audio), frame_size)]

# Write left and right channel values to a file
output_file = "channel_values.txt"
with open(output_file, "w") as f:
    for frame in frames:
        left_values = frame.get_array_of_samples()[::num_channels]
        right_values = frame.get_array_of_samples()[1::num_channels]
        for left, right in zip(left_values, right_values):
            f.write(f"{left},{right}\n")

print(f"Channel values from the first 8 seconds written to {output_file}.")
