import Box from "@mui/material/Box";
import Paper from "@mui/material/Paper";
import { CapabilityType, DeviceInfo } from "../../common";
import { useEffect, useState } from "react";
import { TrainControl } from "./TrainControl";
import { Divider } from "@mui/material";
import { MP3Control } from "./MP3Control";
import { TurnoutControl } from "./TurnoutControl";
import { LightControl } from "./LightControl";
import { OTAControl } from "./OTAControl";

export default function DevicesList() {
  async function fetchApi() {
    try {
      console.log("fetchApi");
      const response = await fetch("/api/v1/device", {
        headers: {
          Accept: "application/json",
          "Content-Type": "application/json",
        },
        method: "GET",
      });

      if (response.status === 200) {
        return response.json();
      }
    } catch (e) {
      console.log(e);
    }

    return [];
  }

  const def: DeviceInfo[] = [];
  const [rows, setRows] = useState(def);

  useEffect(() => {
    fetchApi().then((data) => setRows(data));
  }, []);

  return (
    <Box className="w-[90%] h-full">
      <Paper className="w-[90%] h-full mb-2">
        test
        {rows &&
          rows.map((row, _) => {
            return (
              <Box key={row.device.id}>
                {row.device.id}
                <Divider />
                {row.device.capabilities
                  .map((x, _, __) => x.type)
                  .includes(CapabilityType.Train) && (
                  <TrainControl device={row.device} state={row.state} />
                )}
                {row.device.capabilities
                  .map((x, _, __) => x.type)
                  .includes(CapabilityType.Player) && (
                  <MP3Control device={row.device} state={row.state} />
                )}
                {row.device.capabilities
                  .map((x, _, __) => x.type)
                  .includes(CapabilityType.Turnout) && (
                  <TurnoutControl device={row.device} state={row.state} />
                )}
                {row.device.capabilities
                  .map((x, _, __) => x.type)
                  .includes(CapabilityType.Light) && (
                  <LightControl device={row.device} state={row.state} />
                )}
                {row.device.capabilities
                  .map((x, _, __) => x.type)
                  .includes(CapabilityType.Ota) && (
                  <OTAControl device={row.device} state={row.state} />
                )}
              </Box>
            );
          })}
      </Paper>
    </Box>
  );
}
