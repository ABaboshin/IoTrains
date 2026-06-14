FROM node:24.4.1 as builder
WORKDIR /
COPY . webui
WORKDIR webui
RUN yarn build
ENTRYPOINT [ "yarn", "start" ]
FROM node:24.4.1 as publisher

WORKDIR /
COPY --from=builder /webui/server/dist /dist
WORKDIR /dist

ENTRYPOINT [ "node", "index.js" ]
