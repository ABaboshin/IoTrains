Import("env")
for lb in env.GetLibBuilders():
    if (lb.name == "iotrains"):
        lb.env.Append(CPPDEFINES=[
            ("ESP8266", 1),
            ("AUDIO_I2S", 1)
        ])
