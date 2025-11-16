#!/usr/bin/env python3
import sys
import subprocess
from pathlib import Path

if len(sys.argv) != 5:
    print(f"Usage: {sys.argv[0]} video_name width height fps")
    sys.exit(1)

video_name = sys.argv[1]
width = sys.argv[2]
height = sys.argv[3]
fps = sys.argv[4]

# Create folder for frames
frame_dir = Path(video_name)
frame_dir.mkdir(exist_ok=True)

# Run ffmpeg to extract frames
ffmpeg_cmd = [
    "ffmpeg",
    "-i", f"{video_name}.mp4",
    "-vf", f"fps={fps},scale={width}:{height}",
    str(frame_dir / "frame_%06d.png")
]

subprocess.run(ffmpeg_cmd, check=True)

# Count number of frames
num_frames = len(list(frame_dir.glob("frame_*.png")))

# Run your C program
subprocess.run(["./main.exe", video_name, str(num_frames), fps])
