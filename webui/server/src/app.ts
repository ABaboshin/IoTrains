import path from "path";
import express, { NextFunction, Request, RequestHandler, Response } from "express";
import cors from "cors";
import { Command, Convert } from "common";

const PORT = process.env.PORT || 3001;

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
