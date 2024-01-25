arduino-cli core update-index --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json

arduino-cli core install esp8266:esp8266 --additional-urls https://arduino.esp8266.com/stable/package_esp8266com_index.json
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

protoc ../protobuf/controlunit.proto  --cpp_out=protobuf -I../protobuf --experimental_allow_proto3_optional
pip3 install protobuf grpcio-tools
python3 ~/playground/nanopb/generator/nanopb_generator.py ../protobuf/controlunit.proto -I../protobuf