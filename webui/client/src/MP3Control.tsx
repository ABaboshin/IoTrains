import { Box, Button } from '@mui/material';
import { Function, DeviceInfo, Mp3Command } from 'common';
import { FunctionComponent } from 'react';

export const MP3Control: FunctionComponent<DeviceInfo> = (mp3: DeviceInfo) =>
  {
    async function SendCommand(id: string, command: Mp3Command )
    {
      fetch(`/api/v1/device/${id}`, {
        headers: { 'Accept': 'application/json', 'Content-Type': 'application/json' },
        method: "PUT",
        body: JSON.stringify(command)
      })
    }

  return (<Box>
    <Button variant="contained" onClick={() => SendCommand(mp3.device.id, {
      function: Function.PlayURL,
      discriminator: "Mp3Command",
      url: "http://192.168.31.90:3001/test.mp3"
    })}>Play url</Button>
    <Button variant="contained" onClick={() => SendCommand(mp3.device.id, {
      function: Function.PlayID,
      discriminator: "Mp3Command",
      url: "test"
    })}>Play built-in</Button>
    <Button variant="contained" onClick={() => SendCommand(mp3.device.id, {
      function: Function.StopPlay,
      url: "",
      discriminator: "Mp3Command"
    })}>Stop</Button>
  </Box>);
  };
