import { Box, Button, Input } from '@mui/material';
import { Function, DeviceInfo, Mp3Command, CapabilityType } from 'common';
import { FunctionComponent } from 'react';

export const MP3Control: FunctionComponent<DeviceInfo> = (mp3: DeviceInfo) =>
  {
    let url = "";

    async function SendCommand(id: string, command: Mp3Command )
    {
      fetch(`/api/v1/device/${id}`, {
        headers: { 'Accept': 'application/json', 'Content-Type': 'application/json' },
        method: "PUT",
        body: JSON.stringify(command)
      })
    }

    function ChangeUrl(e: any) {
      url = e.target.value;
    }

  return (<Box>
    {
      mp3.device.capabilities.map((x, i, ar) => x.type).includes(CapabilityType.PlayURL) &&
      <Box>
        <form>
            <Input placeholder='url' onChange={(e) => ChangeUrl(e)} />
            <Button variant="contained" onClick={() => SendCommand(mp3.device.id, {
              function: Function.PlayURL,
              discriminator: "Mp3Command",
              url
            })}>Play url</Button>
        </form>
      </Box>
    }

    {
      mp3.device.capabilities.map((x, i, ar) => x.type).includes(CapabilityType.PlayID) &&
      mp3.device.capabilities.filter(x => x.type === CapabilityType.PlayID).map((x, i, ar) => x.value).map(x =>
        <Button variant="contained" onClick={() => SendCommand(mp3.device.id, {
          function: Function.PlayID,
          discriminator: "Mp3Command",
          url: x
        })}>Play built-in {x}</Button>
        )
    }

    <Box>
      <Button variant="contained" onClick={() => SendCommand(mp3.device.id, {
        function: Function.StopPlay,
        url: "",
        discriminator: "Mp3Command"
      })}>Stop sound</Button>
    </Box>
  </Box>);
  };
