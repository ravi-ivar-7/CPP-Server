import http from 'k6/http';
import { check, sleep } from 'k6';
import { htmlReport } from 'https://raw.githubusercontent.com/benc-uk/k6-reporter/main/dist/bundle.js';

// Load files globally
const fileData = open('rest.http', 'b');

export let options = {
    stages: [
        { duration: '30s', target: 50 },  // Ramp-up to 50 users over 30 seconds
        { duration: '1m', target: 50 },   // Stay at 50 users for 1 minute
        { duration: '10s', target: 0 },   // Ramp-down to 0 users
    ],
};

export function apiTest() {
    let res = http.get('http://localhost:7000/sys-server-info?sysInfo=true');
    check(res, {
        'is status 200': (r) => r.status === 200,
        'response time is less than 200ms': (r) => r.timings.duration < 200,
    });
    sleep(1);
}

export function fileApiTest() {
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

export function handleSummary(data) {
    return {
        'summary.html': htmlReport(data),
    };
}

export default function () {
    const testType = __ENV.TEST_TYPE;

    if (testType === 'apiTest') {
        apiTest();
    } else if (testType === 'fileApiTest') {
        fileApiTest();
    } else {
        apiTest();
        fileApiTest();
    }
}


// TEST_TYPE=apiTest k6 run k6_http.js
// Running all tests: k6 run k6_http.js

// with output files:
// TEST_TYPE=apiTest k6 run --out html=apiTest_summary.html k6_http.js
// TEST_TYPE=apiTest k6 run --out 'web-dashboard' k6_http.js

// TEST_TYPE=apiTest k6 run --out influxdb=http://localhost:8086/k6 k6_http.js
