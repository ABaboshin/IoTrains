FROM ubuntu as build
RUN apt update && apt install -y curl python3
# RUN apt update && apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib git python3 build-essential wget ffmpeg
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
RUN arduino-cli core update-index --additional-urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
RUN arduino-cli core install rp2040:rp2040 --additional-urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
# WORKDIR /
# RUN git clone https://github.com/pschatzmann/arduino-libhelix.git
# WORKDIR /arduino-libhelix
WORKDIR /sketch
COPY . .
WORKDIR /
# RUN arduino-cli core update-index --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
# RUN arduino-cli lib install ESP8266HTTPClient --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
RUN arduino-cli compile --fqbn rp2040:rp2040:rpipico --output-dir /build sketch
FROM scratch
COPY --from=build /build/*.uf2 /
