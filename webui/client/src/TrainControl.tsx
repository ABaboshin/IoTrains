import { Box, Button } from '@mui/material';
import Slider from '@mui/material/Slider';
import { Command, Device, Function } from 'common';
import { FunctionComponent } from 'react';

type TrainControlProps = {
  train: Device
}

export const TrainControl: FunctionComponent<TrainControlProps> = ({ train }) =>
  {
    async function SendCommand(v: any)
    {
      if (typeof v === "number") {
        const speed = v as number;

        train.state.speed = v;

        const command: Command = {
          function: speed === 0 ? Function.Stop : speed > 0 ? Function.MoveForward : Function.MoveBackward,
          value: speed.toString()
        };

        fetch(`/api/v1/device/${train.id}`, {
          headers: { 'Accept': 'application/json', 'Content-Type': 'application/json' },
          method: "PUT",
          body: JSON.stringify(command)
        })
      }
    }

  return (<Box>
    <Slider defaultValue={train?.state?.speed ?? 0} aria-label="Default" valueLabelDisplay="on" min={-100} max={100} onChange={(e, v, a) => SendCommand(v)} />
    <Button variant="contained" onClick={() => SendCommand(0)}>Stop</Button>
  </Box>);
  };
