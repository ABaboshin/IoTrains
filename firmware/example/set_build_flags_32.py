Import("env")
for lb in env.GetLibBuilders():
    if (lb.name == "iotrains"):
        lb.env.Append(CPPDEFINES=[
            ("ESP32", 1),
            # ("AUDIO_ANALOG", 1)
            ("AUDIO_I2S", 1)
            # ("AUDIO_PWM", 1)
        ])
