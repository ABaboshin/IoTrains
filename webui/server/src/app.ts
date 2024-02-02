import path from "path";
import express, { NextFunction, Request, RequestHandler, Response } from "express";
import cors from "cors";
import { Command, Convert } from "common";
import { connect } from "mqtt";

const PORT = process.env.PORT || 3001;
const client = connect("mqtt://localhost", { username: "manager", password: "!manager"});

client.on("connect", (x) => {
  console.log("connected");
  client.subscribe("report", (err) => {
    if (err) {
      console.log(err);
    } else {
      console.log("subscribed");
    }
  });
});

// client.on("error", (x) => {
//   console.log(`${x}`);
// });

// client.on("close", () => {
//   console.log("close");
// });

client.on("message", (topic, message) => {
  // message is Buffer
  console.log(message.toString());
});

  // client.on("connect", (x) => {
  //   console.log(`connect`);
  // });

  // client.on("message", (x) => {
  //   console.log(`message`);
  // });

  // client.on("packetsend", (x) => {
  //   console.log(`packetsend ${JSON.stringify(x)}`);
  // });

  // client.on("packetreceive", (x) => {
  //   console.log(`packetreceive`);
  // });

  // client.on("disconnect", (x) => {
  //   console.log(`disconnect`);
  // });

  // client.on("error", (x) => {
  //   console.log(`${x}`);
  // });

  // client.on("close", () => {
  //   console.log(`close`);
  // });

  // client.on("end", () => {
  //   console.log(`end`);
  // });

  // client.on("reconnect", () => {
  //   console.log(`reconnect`);
  // });

  // client.on("offline", () => {
  //   console.log(`offline`);
  // });

  // client.on("outgoingEmpty", () => {
  //   console.log(`outgoingEmpty`);
  // });

const app = express();

app.use(cors());
app.use(express.json() as RequestHandler);

app.get("/api", (req, res) => {
  const command: Command = Convert.toCommand(`{"function": "stop"}`);
  res.json(command);
});

app.use((req: Request, res: Response, next: NextFunction) => {
  if (/(.ico|.js|.css|.jpg|.png|.map|.svg|.json)$/i.test(req.path)) {
    next();
  } else {
    res.header('Cache-Control', 'private, no-cache, no-store, must-revalidate');
    res.header('Expires', '-1');
    res.header('Pragma', 'no-cache');
    res.sendFile(path.join(__dirname, 'build', 'index.html'));
  }
});

app.use(express.static(path.join(__dirname, 'build')));

app.listen(PORT, () => {
  console.log(`Server listening on ${PORT}`);
});
