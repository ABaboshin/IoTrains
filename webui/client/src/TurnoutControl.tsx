import { Box, Button } from "@mui/material";
import { Function, DeviceInfo, Command } from "../../common";

export const TurnoutControl = (mp3: DeviceInfo) => {
  async function SendCommand(id: string, command: Command) {
    fetch(`/api/v1/device/${id}`, {
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
      <Button
        variant="contained"
        onClick={() =>
          SendCommand(mp3.device.id, {
            function: Function.TurnoutPos1,
            discriminator: "Command",
          })
        }
      >
        Turn left
      </Button>
      <Button
        variant="contained"
        onClick={() =>
          SendCommand(mp3.device.id, {
            function: Function.TurnoutPos2,
            discriminator: "Command",
          })
        }
      >
        Turn right
      </Button>
    </Box>
  );
};
