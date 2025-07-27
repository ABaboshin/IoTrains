import { Box, Button, Input } from "@mui/material";
import { Function, DeviceInfo, OtaCommand } from "../../common";

export const OTAControl = (ota: DeviceInfo) => {
  let url = "";

  async function SendCommand(id: string, command: OtaCommand) {
    fetch(`/api/v1/device/${id}`, {
      headers: {
        Accept: "application/json",
        "Content-Type": "application/json",
      },
      method: "PUT",
      body: JSON.stringify(command),
    });
  }

  function ChangeUrl(e: string) {
    url = e;
  }

  return (
    <Box>
      {
        <Box>
          <form>
            <Input placeholder="url" onChange={(e) => ChangeUrl(e.target.value)} />
            <Button
              variant="contained"
              onClick={() =>
                SendCommand(ota.device.id, {
                  function: Function.Update,
                  discriminator: "OtaCommand",
                  url,
                })
              }
            >
              Update
            </Button>
          </form>
        </Box>
      }
    </Box>
  );
};
