import http from 'k6/http';
import { check, sleep } from 'k6';

export default function () {
    let res = http.get('http://127.0.0.1:8000/sys-server-info');
    check(res, {
        // 'is status 200': (r) => r.status === 200,
        'response time is less than 200ms': (r) => r.timings.duration < 200,
    });
    sleep(1);
}

// k6 run --out 'web-dashboard' temp.js