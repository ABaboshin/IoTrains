import { Box, Button } from '@mui/material';
import { Function, DeviceInfo, Command, CapabilityType } from 'common';
import { FunctionComponent } from 'react';

export const LightControl: FunctionComponent<DeviceInfo> = (light: DeviceInfo) => {
  async function SendCommand(id: string, command: Command) {
    fetch(`/api/v1/device/${id}`, {
      headers: { 'Accept': 'application/json', 'Content-Type': 'application/json' },
      method: "PUT",
      body: JSON.stringify(command)
    })
  }

  return (<Box>{
    light.device.capabilities.filter(x => x.type === CapabilityType.Light).map((x, i, ar) => x.value).map(x =>
      <Box>
        {x}
        <Button variant="contained" onClick={() => SendCommand(light.device.id, {
          function: Function.On,
          discriminator: "LightCommand",
          name: x
        })}>On</Button>
        <Button variant="contained" onClick={() => SendCommand(light.device.id, {
          function: Function.Off,
          discriminator: "LightCommand",
          name: x
        })}>Off</Button>
      </Box>
    )
  }
  </Box>);
};
