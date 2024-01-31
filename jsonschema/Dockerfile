FROM node

RUN npm install -g quicktype

WORKDIR /decoder/jsonschema

ENTRYPOINT [ "npm", "run", "build" ]
