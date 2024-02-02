import Box from '@mui/material/Box';
import Table from '@mui/material/Table';
import TableBody from '@mui/material/TableBody';
import TableCell from '@mui/material/TableCell';
import TableContainer from '@mui/material/TableContainer';
import TableHead from '@mui/material/TableHead';
import TableRow from '@mui/material/TableRow';
import Paper from '@mui/material/Paper';
import { Device, DeviceType } from 'common';
import { useEffect, useState } from 'react';
import { TrainControl } from './TrainControl';

interface HeadCell {
  id: keyof Device;
  label: string;
}

const headCells: readonly HeadCell[] = [
  {
    id: 'id',
    label: 'Name',
  },
];

function EnhancedTableHead() {
  return (
    <TableHead>
      <TableRow>
        {headCells.map((headCell) => (
          <TableCell
            key={headCell.id}
            scope="row"
          >
            {headCell.label}
          </TableCell>
        ))}
        <TableCell>
          Action
        </TableCell>
      </TableRow>
    </TableHead>
  );
}

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

  const def: Device[] = [];
  const [rows, setRows] = useState(def);

  useEffect(() => {
    fetchApi().then(data => setRows(data));
  }, []);

  return (
    <Box sx={{ width: '90%' }}>
      <Paper sx={{ width: '90%', mb: 2 }}>
        <TableContainer>
          <Table
            sx={{ minWidth: 750 }}
            aria-labelledby="tableTitle"
          >
            <EnhancedTableHead
            />
            <TableBody>
              {rows && rows.map((row, index) => {
                const labelId = `enhanced-table-checkbox-${index}`;

                return (
                  <TableRow
                    key={row.id}
                    sx={{ '&:last-child td, &:last-child th': { border: 0 } }}
                  >
                    <TableCell
                      component="th"
                      id={labelId}
                      scope="row"
                    >
                      {row.id}
                    </TableCell>
                    <TableCell
                      component="th"
                      scope="row">{
                        row.type === DeviceType.Train && <TrainControl train={row}/>
                      }</TableCell>
                  </TableRow>
                );
              })}
            </TableBody>
          </Table>
        </TableContainer>
      </Paper>
    </Box>
  );
}
