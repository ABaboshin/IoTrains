import Box from '@mui/material/Box';
import Paper from '@mui/material/Paper';
import { DeviceInfo, DeviceType } from 'common';
import { useEffect, useState } from 'react';
import { TrainControl } from './TrainControl';
import { Divider } from '@mui/material';
import { MP3Control } from './MP3Control';

export default function DevicesList() {
  async function fetchApi() {
    try {
      console.log("fetchApi");
      const response = await fetch('/api/v1/device', {
        headers: { 'Accept': 'application/json', 'Content-Type': 'application/json' },
        method: "GET"
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
    fetchApi().then(data => setRows(data));
  }, []);

  return (
    <Box sx={{ width: '90%' }}>
      <Paper sx={{ width: '90%', mb: 2 }}>
        {rows && rows.map((row, index) => {
          return (
            <Box key={row.device.id}>
              {row.device.id}
              <Divider />
              {
                row.device.type === DeviceType.Train && <TrainControl device={row.device} state={row.state} />
              }
              {
                row.device.type === DeviceType.Player && <MP3Control device={row.device} state={row.state} />
              }
            </Box>
          );
        })}
      </Paper>
    </Box>
  );
}
