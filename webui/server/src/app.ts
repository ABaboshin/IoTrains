import path from "path";
import express, { NextFunction, Request, RequestHandler, Response } from "express";
import cors from "cors";
import { Command, Convert, ControlUnit, Device } from "common";
import { connect } from "mqtt";

const PORT = process.env.PORT || 3001;
const client = connect("mqtt://localhost", { username: "manager", password: "!manager" });

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

let units = new Map<string, ControlUnit>();

client.on("message", (topic, message) => {
  const cu = Convert.toControlUnit(message.toString());

  units.set(cu.id, cu);

  console.log(JSON.stringify(cu));
});

const app = express();

app.use(cors());
app.use(express.json() as RequestHandler);

app.get("/api/v1/device", (req, res) => {
  const devices = Array.from(units.entries()).map((x, i, ar) => x[1].devices).flat(1);
  res.json(devices);
});

app.put("/api/v1/device/:id", (req, res) => {
  const command: Command = req.body as Command;

  client.publish(req.params.id, JSON.stringify(command));

  res.json({});
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
