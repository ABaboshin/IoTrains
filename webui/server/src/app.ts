import path from "path";
import express, {
  NextFunction,
  Request,
  RequestHandler,
  Response,
} from "express";
import cors from "cors";
import { Command, Convert, ControlUnit, State, DeviceInfo } from "common";
import { connect } from "mqtt";
import moment from "moment";

const PORT = process.env.PORT || 3001;
const MQTT_URL = process.env.MQTT_URL || "mqtt://192.168.31.208";
const MQTT_USERNAME = process.env.MQTT_USERNAME || "manager";
const MQTT_PASSWORD = process.env.MQTT_PASSWORD || "!manager";
const client = connect(MQTT_URL, {
  username: MQTT_USERNAME,
  password: MQTT_PASSWORD,
});

const reportQueue = "report";
const stateQueue = "state";
const eventQueue = "event";

client.on("connect", (x) => {
  console.log("connected");
  client.subscribe(reportQueue, (err) => {
    if (err) {
      console.log(err);
    } else {
      console.log("subscribed report");
    }
  });
  client.subscribe(stateQueue, (err) => {
    if (err) {
      console.log(err);
    } else {
      console.log("subscribed state");
    }
  });
  client.subscribe(eventQueue, (err) => {
    if (err) {
      console.log(err);
    } else {
      console.log("subscribed event");
    }
  });
});

client.on("error", (err) => {
  console.warn(err);
  client.end();
});

let units = new Map<string, ControlUnit>();
let states = new Map<string, State>();
let events = new Array<Event>();

client.on("message", (topic, message) => {
  console.log(
    `${moment().format("yyyy-MM-DD HH:mm:ss")} ${topic} ${message.toString()}`
  );
  if (topic === reportQueue) {
    const cu = Convert.toControlUnit(message.toString());

    units.set(cu.id, cu);
  }

  if (topic === stateQueue) {
    const tmp = JSON.parse(message.toString());

    states.set(tmp.id, tmp);
  }

  if (topic === eventQueue) {
    const tmp = JSON.parse(message.toString());

    events.push(tmp);
  }
});

const app = express();

app.use(cors());
app.use(express.json() as RequestHandler);

app.get("/api/v1/event", (req, res) => {
  res.json(events);
});

app.get("/api/v1/device", (req, res) => {
  const devices = Array.from(units.entries())
    .map((x, i, ar) => x[1].devices)
    .flat(1)
    .map((d) => {
      let r: DeviceInfo = { device: d, state: states.get(d.id) };
      return r;
    });
  res.json(devices);
});

app.put("/api/v1/device/:id", (req, res) => {
  const command: Command = req.body as Command;

  client.publish(req.params.id, JSON.stringify(command));
  console.log(`publish ${JSON.stringify(command)}`);

  res.json({});
});

app.use((req: Request, res: Response, next: NextFunction) => {
  if (/(.ico|.js|.css|.jpg|.png|.map|.svg|.json|.mp3)$/i.test(req.path)) {
    console.log(`request ${req.path}`);
    next();
  } else {
    res.header("Cache-Control", "private, no-cache, no-store, must-revalidate");
    res.header("Expires", "-1");
    res.header("Pragma", "no-cache");
    res.sendFile(path.join(__dirname, "build", "index.html"));
  }
});

app.use(express.static(path.join(__dirname, "build")));

app.listen(PORT, () => {
  console.log(`Server listening on ${PORT}`);
});
