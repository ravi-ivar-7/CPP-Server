import { check, group } from 'k6';
import ws from 'k6/ws';
import { Counter } from 'k6/metrics';
import { htmlReport } from 'https://raw.githubusercontent.com/benc-uk/k6-reporter/main/dist/bundle.js';

export let errorCounter = new Counter('errors');

export const options = {
    vus: 10, 
    duration: '30s',  
    thresholds: {
        errors: ['count<10'],
    },
};

const wsUrl = 'ws://localhost:7000/stream-text?userId=ID_PLACEHOLDER&authToken=StreamTextSecretToken';

export  function cppWsTest () {
    const userId = `U-${__VU}-${__ITER}`;  // Generate unique user ID for each virtual user and iteration
    const url = wsUrl.replace('ID_PLACEHOLDER', encodeURIComponent(userId));

    group('WebSocket Test', function () {
        const res = ws.connect(url, {}, function (socket) {
            socket.on('open', function () {
                console.log(`VU: ${__VU} - Connected`);
                socket.send('Hello, world!');
            });

            socket.on('message', function (message) {
                console.log(`VU: ${__VU} - Received message: ${message}`);
            });

            socket.on('error', function (e) {
                console.error(`VU: ${__VU} - WebSocket error: ${e.error()}`);
                errorCounter.add(1);
            });

            socket.on('close', function () {
                console.log(`VU: ${__VU} - Disconnected`);
            });
            socket.setTimeout(function () {
                socket.close();
            }, 10000);
        });

        check(res, {
            'Connected successfully': (r) => r && r.status === 101,
        });
    });
}

export function handleSummary(data) {
    const date = new Date().toISOString().slice(0, 10); 
    const fileNamePrefix = __ENV.TEST_TYPE || 'summary'; 
    const fileName = `${fileNamePrefix}-${date}.html`; 

    return {
        [fileName]: htmlReport(data), 
    };
}


export default function () {
    const testType = __ENV.TEST_TYPE;

    if (testType === 'cppWsTest') {
        cppWsTest();
    }  else {
       console.log("No test name provided.")
    }
}

// TEST_TYPE=cppWsTest k6 run --out 'web-dashboard' k6_ws.js   
// http://localhost:5665/ui/?endpoint=/