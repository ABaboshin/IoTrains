Import("env")
for lb in env.GetLibBuilders():
    if (lb.name == "iotrains"):
        lb.env.Append(CPPDEFINES=[
            ("ESP32", 1),
            ("ESPWROOM", 1)
        ])
