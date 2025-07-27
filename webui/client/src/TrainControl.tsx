import { Box, Button } from "@mui/material";
import Slider from "@mui/material/Slider";
import { Function, DeviceInfo, TrainCommand } from "../../common";

export const TrainControl = (train: DeviceInfo) => {
  async function SendCommand(speed: number) {
    if (train.state !== undefined) {
      train.state.speed = speed;
    }

    const command: TrainCommand = {
      function:
        speed === 0
          ? Function.Break
          : speed > 0
          ? Function.MoveForward
          : Function.MoveBackward,
      speed: speed,
      discriminator: "TrainCommand",
    };

    fetch(`/api/v1/device/${train.device.id}`, {
      headers: {
        Accept: "application/json",
        "Content-Type": "application/json",
      },
      method: "PUT",
      body: JSON.stringify(command),
    });
  }

  return (
    <Box>
      <Slider
        defaultValue={train?.state?.speed ?? 0}
        aria-label="Default"
        valueLabelDisplay="on"
        min={-100}
        max={100}
        onChange={(_, v, __) => SendCommand(v)}
      />
      <Button variant="contained" onClick={() => SendCommand(0)}>
        Stop
      </Button>
    </Box>
  );
};
