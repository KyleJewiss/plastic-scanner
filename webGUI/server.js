const express = require('express');
const SerialPort = require('serialport');


const app = express();
const port = 3000;

app.use(express.static('public'));

// Get a list of available COM ports
app.get('/ports', (req, res) => {
    SerialPort.list()
        .then(ports => {
            res.json(ports);
        })
        .catch(error => {
            console.error(error);
            res.status(500).send('Error fetching COM ports');
        });
});

// Connect to a specified COM port
app.get('/connect', (req, res) => {
    const portName = req.query.port;

    const port = new SerialPort(portName, { baudRate: 9600 });

    port.on('open', () => {
        console.log(`Connected to ${portName}`);
        res.send('Connected');
    });

    port.on('error', error => {
        console.error(`Error opening ${portName}:`, error);
        res.status(500).send('Error connecting to COM port');
    });
});

// Define a simple response for the root path ("/")
app.get('/', (req, res) => {
    res.send('Welcome to the Serial Port Connector');
});

app.listen(port, () => {
    console.log(`Server is running on port ${port}`);
});
