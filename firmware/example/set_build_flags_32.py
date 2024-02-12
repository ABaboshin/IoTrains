Import("env")
for lb in env.GetLibBuilders():
    if (lb.name == "iotrains"):
        print("found!")
        lb.env.Append(CPPDEFINES=[
            ("ESP32", 1)
        ])
