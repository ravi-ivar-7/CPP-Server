const express = require('express');
const os = require('os');
const cors = require('cors');

const app = express();
const port = 3000;

app.use(cors());

app.get('/sys-server-info', (req, res) => {
    try {
        const queryParams = req.query;
        const sysInfo = queryParams.sysInfo;

        let response = {
            Server: 'Healthy'
        };

        if (sysInfo === 'true') {
            response["CPU-INFO"] = getCpuInfo();
            response["MEMORY-INFO"] = getMemoryInfo();
            response["UP-TIME"] = getUpTime();
            response["CORES"] = getCoresNum();
        }

        res.status(200).json(response);
    } catch (ex) {
        console.error('Internal Server Error:', ex.message);
        res.status(500).send('Internal Server Error: ' + ex.message);
    }
});

function getCpuInfo() {
    const cpus = os.cpus();
    return cpus.map((cpu, index) => {
        return {
            model: cpu.model,
            speed: cpu.speed,
            times: cpu.times
        };
    });
}

function getMemoryInfo() {
    return {
        totalMemory: os.totalmem(),
        freeMemory: os.freemem()
    };
}

function getUpTime() {
    return os.uptime();
}

function getCoresNum() {
    return os.cpus().length;
}

app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});


// curl "http://localhost:3000/sys-server-info?sysInfo=true"
