# *Terminal Video Player* -- Side quest

you have to compile it using `gnu` compiler. Currently, it is Windows supported.
```bash
gcc player.c -o main.exe # must output main.exe
```

To use this, you must have a `video file` and `ffmpeg` installed on your system. If all conditions are met, then here you go :
```bash
python script.py <video-name-without-extension> <frame-width> <frame-heigth> <fps>
```

```bash
python script.py example 160 90 20
```

Frame numbers must not exist 6 digits. The output video resolution depends on you terminal viewpoint size. You can adjust it by zoomming in or out the terminal. 

`Disclaimer: It can not be stopped with Ctrl-C. You have to close the terminal in order to stop the playback.`



