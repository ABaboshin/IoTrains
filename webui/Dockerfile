FROM node as builder
WORKDIR /
COPY . webui
WORKDIR webui
RUN npm run build
ENTRYPOINT [ "npm", "run", "start" ]
# FROM node as publisher

# WORKDIR /
# COPY --from=builder /webui/server/dist /dist
# WORKDIR /dist

# ENTRYPOINT [ "node", "index.js" ]
