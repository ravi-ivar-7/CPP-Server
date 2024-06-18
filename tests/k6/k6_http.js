import http from 'k6/http';
import { check, sleep } from 'k6';
import { htmlReport } from 'https://raw.githubusercontent.com/benc-uk/k6-reporter/main/dist/bundle.js';

// Load files globally
const fileData = open('../instructions.md', 'b');

export let options = {
    stages: [
        { duration: '30s', target: 5000 },  // Ramp-up to 50 users over 30 seconds
        { duration: '1m', target: 5000 },   // Stay at 50 users for 1 minute
        { duration: '10s', target: 0 },   // Ramp-down to 0 users
    ],
};

export function cppApiTest() {
    let res = http.get('http://localhost:7000/sys-server-info?sysInfo=true');
    check(res, {
        'is status 200': (r) => r.status === 200,
        'response time is less than 1000ms': (r) => r.timings.duration < 1000,
    });
    sleep(1);
}

export function cppFileApiTest() {
    const url = 'http://localhost:7000/upload-file';
    const payload = {
        file: http.file(fileData, 'rest.http'),
    };

    const params = {
        headers: {
            'Content-Type': 'multipart/form-data',
        },
    };

    let res = http.post(url, payload, params);
    check(res, {
        'is status 200': (r) => r.status === 200,
        'response time is less than 200ms': (r) => r.timings.duration < 200,
    });
}


export function fastapiTest() {
    let res = http.get('http://localhost:8000/sys-server-info');
    check(res, {
        'is status 200': (r) => r.status === 200,
        'response time is less than 200ms': (r) => r.timings.duration < 200,
    });
    sleep(1);
}

export function nodejsTest() {
    let res = http.get('http://localhost:3000/sys-server-info?sysInfo=true');
    check(res, {
        'is status 200': (r) => r.status === 200,
        'response time is less than 200ms': (r) => r.timings.duration < 200,
    });
    sleep(1);
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

    if (testType === 'cppApiTest') {
        cppApiTest();
    } else if (testType === 'cppFileApiTest') {
        cppFileApiTest();
    }
    else if (testType === 'nodejsTest') {
        nodejsTest();
    }
    else if (testType === 'fastapiTest') {
        fastapiTest();
    } else {
       // run all test 
       console.log("No test name provided");
    }
}



// with output files:
// TEST_TYPE=cppApiTest k6 run --out html=apiTest_summary.html k6_http.js
// TEST_TYPE=cppApiTest k6 run --out 'web-dashboard' k6_http.js   // http://localhost:5665/ui/?endpoint=/

// TEST_TYPE=cppApiTest k6 run --out influxdb=http://localhost:8086/k6 k6_http.js
